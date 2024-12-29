#include "heartBox.h"

LOG_MODULE_REGISTER(heartBox, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(heart_box_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

const struct gpio_dt_spec display2[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(heart_box_display2), gpios, DT_SPEC_AND_COMMA_GATE)
};

#define DISPLAY4_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(holtek_ht16k33)


#define STRIP_NODE		DT_NODELABEL(ws2812)

#if DT_NODE_HAS_PROP(DT_NODELABEL(ws2812), chain_length)
static const uint8_t wsChainLength = DT_PROP(DT_NODELABEL(ws2812), chain_length);
#else
#error Unable to determine length of LED strip
#endif

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
    display4 = new Display4(DEVICE_DT_GET(DISPLAY4_NODE));
    creatingMqttList(11);

    // while(1)
    // {
    //     display4->displayStr("hi");
    // }




    char topic[64];
    sprintf(topic, "%s/%s/", roomName, puzzleTypeName);
    keypad = new Keypad(topic);

    static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);  
    device_init(strip); 
	if (!device_is_ready(strip)) {
		LOG_ERR("strip Device not ready, aborting test");
	}    
    ledStrip = new LedStrip(strip, wsChainLength);


    device_init(DEVICE_DT_GET(DT_NODELABEL(spi1)));
    display8 = new Display8(display2);
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
    mqttCount = wsChainLength + ARRAY_SIZE(allRelays) + numOfDisplays;
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
            // LOG_ERR("display logic");
        }
        else if(strstr(command, "relay") != NULL)
        {
            char field[] = "relay";
            int commandIdx = peripheralIdx(field, command);
            uint8_t relayIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (relayIdx < ARRAY_SIZE(allRelays)))
            {
                if(relayIdx == 1)
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
            if((commandIdx > 0 ) && (ws2811Idx < wsChainLength))
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
        else
        {
            LOG_INF("the command is not valid");
        }
    }
    else
        LOG_INF("the command is not valid");
}

struct led_rgb HeartBox:: retrieveColors(char *str)
{
    struct led_rgb rgb;
    char buf[4];
    uint8_t i = 0;
    while(str[i] != 'G')
    {
        buf[i - 1] = str[i];
        i++;
    }
    buf[i] = '\0';
    rgb.r = atoi(buf);
    memset(buf, 0, 4);
    while(str[i] != 'B')
    {
        buf[i - 1] = str[i];
        i++;
    }
    buf[i] = '\0';
    rgb.g = atoi(buf);
    memset(buf, 0, 4);
    while(str[i] != strlen(str))
    {
        buf[i - 1] = str[i];
        i++;
    }
    buf[i] = '\0';
    rgb.b = atoi(buf);
    return rgb;

}