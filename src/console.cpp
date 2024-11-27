#include "console.h"

LOG_MODULE_REGISTER(configDevice, LOG_LEVEL_INF);


#define LCD1_NODE DT_NODELABEL(lcd1)
#define LCD2_NODE DT_NODELABEL(lcd2)

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),


//  //const struct gpio_dt_spec spec = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(leds), gpios, 1);
static const struct gpio_dt_spec config_puzzle_relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(config_puzzle_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

static const char roomName[] = "introRoom";
static const char puzzleTypeName[] = "console";

static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_buttons));
// static const struct device *relays = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_outputs));

static const struct device *const qdecLang = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_qdec_lang));
static const struct device *const qdecRoom = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_qdec_room));

static void buttonsHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        if(val->code == INPUT_BTN_6)
        {
            sprintf(msg.topic, "%s/%s/switch1", roomName, puzzleTypeName);
            val->value ? sprintf(msg.msg, "TRUE"): sprintf(msg.msg, "FALSE");
        }
        else if((val->code == INPUT_BTN_7) && (val->value))
        {
            sprintf(msg.topic, "%s/%s/langButton", roomName, puzzleTypeName);
            sprintf(msg.msg, "TRUE");
        }
        else if((val->code == INPUT_BTN_8) && (val->value))
        {
            sprintf(msg.topic, "%s/%s/roomButton", roomName, puzzleTypeName);
            sprintf(msg.msg, "TRUE");
        }
        else
        {
            if(val->value)
            {
                sprintf(msg.topic, "%s/%s/button%d", roomName, puzzleTypeName, (val->code - 0x100) + 1);
                sprintf(msg.msg, "TRUE");
            }
        }
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
}


void qdecLangHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_REL)
    {
        struct MqttMsg msg = {0};
        strcpy(msg.topic, "introRoom/console/langEncoder");
        // strcpy(msg.topic, (char *)topic);
        if(val->code == INPUT_REL_WHEEL)
        {
            if(val->value == 1)
            {
                sprintf(msg.msg, "cw");
            }
            else
                sprintf(msg.msg, "ccw");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
    }
    else
    {
        LOG_INF("lang nof of theem");
    } 
}


void qdecRoomHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_REL)
    {
        struct MqttMsg msg = {0};
        strcpy(msg.topic, "introRoom/console/roomEncoder");
        if(val->code == INPUT_REL_WHEEL)
        {
            if(val->value == 1)
            {
                sprintf(msg.msg, "cw");
            }
            else
                sprintf(msg.msg, "ccw");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
    }
    else
    {
        LOG_INF("room nof of theem");
    } 
}

Console:: Console(const char * room, const char *type): Puzzle(room, type)
{
    struct MqttMsg msg = {0};
    sprintf(msg.topic, "%s/%s", roomName, puzzleTypeName);
    strcat(msg.topic, deviceId);

    device_init(qdecLang);
    device_init(qdecRoom);
    
    INPUT_CALLBACK_DEFINE(qdecLang, qdecLangHandler, NULL);
    INPUT_CALLBACK_DEFINE(qdecRoom, qdecRoomHandler, NULL);

	device_init(DEVICE_DT_GET(LCD1_NODE));
	lcd1 = new Lcd(DEVICE_DT_GET(LCD1_NODE), 0, 2, 3, 4, 5, 6, 7);
    lcd1->firstLine((const char *)"    Language    ");
    // sprintf(msg.msg, "lcd1 is configured");
    // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);

	device_init(DEVICE_DT_GET(LCD2_NODE));
	lcd2 = new Lcd(DEVICE_DT_GET(LCD2_NODE), 0, 2, 3, 4, 5, 6, 7);
    lcd2->firstLine("      Room      ");

    // sprintf(msg.msg, "lcd2 is configured");
    // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);


    // int ret;
	// device_init(DEVICE_DT_GET(DT_NODELABEL(expander)));
    // for(unsigned int i = 0; i < ARRAY_SIZE(config_puzzle_relays); i++){
    //     if (!device_is_ready(config_puzzle_relays[i].port)) {
	// 	    // return -1;
	//     }
    //     ret = gpio_pin_configure_dt(&config_puzzle_relays[i], GPIO_OUTPUT_INACTIVE);
	//     if (ret < 0) {
	// 	    // return -1;
	//     }
    // }
    // sprintf(msg.msg, "expander is configured");
    // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);

    device_init(buttons);
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandler, NULL);

    creatingMqttList();
}

void Console:: creatingMqttList()
{
    mqttList[0] = introRoom_console_lcd1_topic;
    mqttList[1] = introRoom_console_lcd2_topic;
    mqttCount = 2;
}


void Console:: messageHandler(struct MqttMsg *msg)
{

    if(strcmp(msg->topic, INTRO_ROOM_CONSOLE_LCD1_TOPIC) == 0)
    {
        lcd1->secondLine(msg->msg);
    }
    else if(strcmp(msg->topic, INTRO_ROOM_CONSOLE_LCD2_TOPIC) == 0)
    {
        lcd2->secondLine(msg->msg);
    }

    // else if (strcmp(msg->topic, RELAY0_TOPIC) == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[0], 1): gpio_pin_set_dt(&config_puzzle_relays[0], 0);
    // }

    // else if (strcmp(msg->topic, RELAY1_TOPIC) == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[1], 1): gpio_pin_set_dt(&config_puzzle_relays[1], 0);
    // }

    // else if (strcmp(msg->topic, "sub/relay2") == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[2], 1): gpio_pin_set_dt(&config_puzzle_relays[2], 0);
    // }
    
    // else if (strcmp(msg->topic, "sub/relay3") == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[3], 1): gpio_pin_set_dt(&config_puzzle_relays[3], 0);
    // }

    // else if (strcmp(msg->topic, "sub/relay4") == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[4], 1): gpio_pin_set_dt(&config_puzzle_relays[4], 0);
    // }

    // else if (strcmp(msg->topic, "sub/relay5") == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[5], 1): gpio_pin_set_dt(&config_puzzle_relays[5], 0);
    // }

    // else if (strcmp(msg->topic, "sub/relay6") == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[6], 1): gpio_pin_set_dt(&config_puzzle_relays[6], 0);
    // }

    // else if (strcmp(msg->topic, "sub/relay7") == 0)
    // {
    //     int val = atoi(msg->msg);
    //     val ? gpio_pin_set_dt(&config_puzzle_relays[7], 1): gpio_pin_set_dt(&config_puzzle_relays[7], 0);
    // }

    else
    {
        char log[] = "Command is not recognized.";
        MqttMsg msg = {0};
        strcpy(msg.topic, LOGS_TOPIC);
        strcpy(msg.msg, log);
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        LOG_INF("%s", log);
    }
}


