#include "heartBox.h"

LOG_MODULE_REGISTER(heartBox, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(heart_box_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

#define STRIP_NODE		DT_NODELABEL(ws2812)

#if DT_NODE_HAS_PROP(DT_NODELABEL(ws2812), chain_length)
static const uint8_t wsChainLength = DT_PROP(DT_NODELABEL(ws2812), chain_length);
#else
#error Unable to determine length of LED strip
#endif
static HeartBox *instance = nullptr;
HeartBox:: HeartBox(const char *room, const char *type): Puzzle(room, type)
{
    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(allRelays); i++){
        if (!device_is_ready(allRelays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&allRelays[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    device_init(DEVICE_DT_GET(DT_NODELABEL(i2c1)));
    display4 = new Display4(DEVICE_DT_GET(DT_NODELABEL(display4)), true);
    creatingMqttList(11);

    device_init(DEVICE_DT_GET(DT_NODELABEL(kbd_matrix34)));
    INPUT_CALLBACK_DEFINE(DEVICE_DT_GET(DT_NODELABEL(keypad34)), keypadHandlerWrapper, (void*)this);

    static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);  
    // device_init(strip); 
	if (!device_is_ready(strip)) {
		LOG_ERR("strip Device not ready, aborting test");
	}    
    ledStrip = new LedStrip(strip, wsChainLength);

    display8 = new Display8(DEVICE_DT_GET(DT_NODELABEL(display8)));

    instance = this;
}

void HeartBox:: creatingMqttList(uint16_t _mqttCount)
{
    uint8_t numOfDisplays = 2;
    char topic[128] = {0};
    for(uint8_t i = 0; i < wsChainLength; i++) /// chain length in overlay
    {
        sprintf(topic, "%sws2811_%d", mqttCommand, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }

    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[wsChainLength + i] = *createMqttTopic(topic);
    }

    for(uint8_t i = 0; i < numOfDisplays; i++)
    {
        sprintf(topic, "%sdisplay%d", mqttCommand, i + 1);
        mqttList[wsChainLength + ARRAY_SIZE(allRelays) + i] = *createMqttTopic(topic);
    }
    sprintf(topic, "%spassword", mqttCommand);
    mqttList[wsChainLength + ARRAY_SIZE(allRelays) + numOfDisplays] = *createMqttTopic(topic);
    mqttCount = wsChainLength + ARRAY_SIZE(allRelays) + numOfDisplays + 1;
}


void HeartBox:: messageHandler(struct MqttMsg *msg)
{
    int rc;
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if(strcmp(command, "display1") == 0)
        {
            display4->displayStr(msg->msg);
        }
        else if(strstr(command, "relay") != NULL)
        {
            char field[] = "relay";
            int commandIdx = peripheralIdx(field, command);
            uint8_t relayIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (relayIdx < ARRAY_SIZE(allRelays)))
            {
                if(commandIdx == 1)
                {
                    relayOperation(msg->msg, &allRelays[relayIdx], true);
                }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else if(strstr(command, "ws2811_") != NULL)
        {
            char field[] = "ws2811_";
            int commandIdx = peripheralIdx(field, command);
            uint8_t ws2811Idx = commandIdx - 1;
            if((commandIdx > 0 ) && (ws2811Idx < 8))
            {
                 //LOG_ERR("a valid index");
                struct led_rgb color_leds = retrieveColors(msg->msg);
                LOG_INF("r: %u g: %u b: %u", color_leds.r, color_leds.g, color_leds.b);

                rc = ledStrip->update(color_leds, ws2811Idx);
                if (rc) {
				    LOG_ERR("couldn't update strip: %d", rc);
			    }
            }
            else
            {
                LOG_ERR("Not a valid index");
            }
        }
        else if(strstr(command, "display2") != NULL)
        {
            LOG_INF("Display2");
            display8->displayStr(msg->msg);
        }
        else if(strcmp(command, "password") == 0)
        {
            strcpy(password, msg->msg);
            passwordSet = true;
        }
        else
        {
            LOG_INF("the command is not valid");
        }
    }
    else
        LOG_INF("the command is not valid");
}

void HeartBox:: keypadHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY) 
    {
        if(val->value)
        {
            if(passwordSet)
            {
                struct MqttMsg msg = {0};
                if((val->code < INPUT_KEY_0))
                {
                    sprintf(msg.topic, "%skeypad%d", mqttCommand, val->code - INPUT_KEY_1 + 1);
                    puzzleSolver(val->code - INPUT_KEY_1 + '0');
                }
                if(val->code == INPUT_KEY_0)
                {
                    sprintf(msg.topic, "%skeypad0", mqttCommand);
                    puzzleSolver('0');
                }
                if(val->code == INPUT_KEY_ENTER)
                {
                    sprintf(msg.topic, "%skeypadEnter", mqttCommand);
                }
                if(val->code == INPUT_KEY_ESC)
                {
                    sprintf(msg.topic, "%skeypadEsc", mqttCommand);
                }
                sprintf(msg.msg, "true");
                LOG_INF("%s",msg.topic);
                k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
            }
        }
    }
}

void HeartBox:: keypadHandlerWrapper(struct input_event *val, void *userData)
{

    instance->keypadHandler(val);
    // ((HeartBox *)userData)->keypadHandler(val);
}

void HeartBox:: puzzleSolver(char input)
{
    if(passwordSet)
    {
        static uint8_t pos = 0;
        static char guess[PUZZLE_DISPLAY_LEN + 1];
        memset(guess, 32, PUZZLE_DISPLAY_LEN);
        guess[PUZZLE_DISPLAY_LEN] = '\0';
        if(input == password[pos])
        {
            guess[pos] = input;
            display8->displayStr(guess);
            pos++;
            if(pos == PUZZLE_DISPLAY_LEN)
            {
                pos = 0;
            }

        }
        {
            memset(guess, 32, PUZZLE_DISPLAY_LEN);//32 = char space 
            guess[PUZZLE_DISPLAY_LEN] = ' ';
            pos = 0;

            char display[PUZZLE_DISPLAY_LEN + 1] = {0};
            display[PUZZLE_DISPLAY_LEN] = ' ';

            memset(display, 56, PUZZLE_DISPLAY_LEN);//56 = char 8
            display8->displayStr(display);
            k_msleep(1000);

            memset(display, 32, PUZZLE_DISPLAY_LEN);
            display8->displayStr(display);
            k_msleep(1000);

            memset(display, 56, PUZZLE_DISPLAY_LEN);//56 = char 8
            display8->displayStr(display);
            k_msleep(1000);

            memset(display, 32, PUZZLE_DISPLAY_LEN);
            display8->displayStr(display);
            k_msleep(1000);
        }

    }
}
