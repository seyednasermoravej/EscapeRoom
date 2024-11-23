#include "configDevice.h"

LOG_MODULE_REGISTER(configDevice, LOG_LEVEL_INF);


#define LCD1_NODE DT_NODELABEL(lcd1)
#define LCD2_NODE DT_NODELABEL(lcd2)


#define RELAYS_NODE_ID	 DT_COMPAT_GET_ANY_STATUS_OKAY(config_puzzle_relays)


static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_buttons));
static const struct device *const relays = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_buttons));

static const struct device *const qdecLang = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_qdec_lang));
static const struct device *const qdecRoom = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_qdec_room));


static void buttonsHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        strcpy(msg.topic, "pub/configDevice/buttons");
        // strcpy(msg.topic, (char *)topic);
        if((val->code == INPUT_BTN_0) && (val->value))
        {

            LOG_INF("Button 0 pressed");
            sprintf(msg.msg, "Button 0 pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_1) && (val->value))
        {

            LOG_INF("Button 1 pressed");
            sprintf(msg.msg, "Button 1 pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_2) && (val->value))
        {

            LOG_INF("Button 2 pressed");
            sprintf(msg.msg, "Button 2 pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_3) && (val->value))
        {

            LOG_INF("Button 3 pressed");
            sprintf(msg.msg, "Button 3 pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_4) && (val->value))
        {

            LOG_INF("Button 4 pressed");
            sprintf(msg.msg, "Button 4 pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_5) && (val->value))
        {

            LOG_INF("Button 5 pressed");
            sprintf(msg.msg, "Button 5 pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if(val->code == INPUT_BTN_6)
        {

            LOG_INF("%s", val->value? "switch on": "switch off");
            sprintf(msg.msg, val->value? "switch on": "switch off");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_7) && (val->value))
        {

            LOG_INF("room pressed");
            sprintf(msg.msg, "room pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_8) && (val->value))
        {

            LOG_INF("language pressed");
            sprintf(msg.msg, "language pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
    }
}


void qdecLangHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_REL)
    {
        struct MqttMsg msg = {0};
        strcpy(msg.topic, "pub/language");
        // strcpy(msg.topic, (char *)topic);
        if(val->code == INPUT_REL_WHEEL)
        {
            if(val->value == 1)
            {
                LOG_INF("language, direction: clockwise.");
                sprintf(msg.msg, "language, direction: clockwise.");
            }
            else
            {
                LOG_INF("language, direction: counterclockwise.");
                sprintf(msg.msg, "language, direction: counterclockwise.");
            }
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
        strcpy(msg.topic, "pub/room");
        if(val->value == 1)
        {
            LOG_INF("room, direction: clockwise.");
            sprintf(msg.msg, "room, direction: clockwise.");
        }
        else
        {
            LOG_INF("room, direction: counterclockwise.");
            sprintf(msg.msg, "room, direction: counterclockwise.");
        }
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
    else
    {
        LOG_INF("room nof of theem");
    } 
}



ConfigDevice:: ConfigDevice()
{
    device_init(qdecLang);
    device_init(qdecRoom);
    
    INPUT_CALLBACK_DEFINE(qdecLang, qdecLangHandler, NULL);
    INPUT_CALLBACK_DEFINE(qdecRoom, qdecRoomHandler, NULL);

	device_init(DEVICE_DT_GET(LCD1_NODE));
	lcd1 = new Lcd(DEVICE_DT_GET(LCD1_NODE), 0, 2, 3, 4, 5, 6, 7);
    lcd1->firstLine("Language");

	device_init(DEVICE_DT_GET(LCD2_NODE));
	lcd2 = new Lcd(DEVICE_DT_GET(LCD2_NODE), 0, 2, 3, 4, 5, 6, 7);
    lcd2->firstLine("Room");


	device_init(relays);

    device_init(buttons);
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandler, NULL);
}

void ConfigDevice:: messageHandler(struct MqttMsg *msg)
{

    if(strcmp(msg->topic, LCD_TOPIC) == 0)
    {
        lcd1->firstLine(msg->msg);
        // lcd2->firstLine(msg->msg);
    }
    else if(strcmp(msg->topic, LCD1_TOPIC) == 0)
    {
        lcd1->secondLine(msg->msg);
    }
    else if(strcmp(msg->topic, LCD2_TOPIC) == 0)
    {
        lcd2->secondLine(msg->msg);
    }

    else if (strcmp(msg->topic, "sub/relay0") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 0): led_off(relays, 0);
    }

    else if (strcmp(msg->topic, "sub/relay1") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 1): led_off(relays, 1);
    }

    else if (strcmp(msg->topic, "sub/relay2") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 2): led_off(relays, 2);
    }
    
    else if (strcmp(msg->topic, "sub/relay3") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 3): led_off(relays, 3);
    }

    else if (strcmp(msg->topic, "sub/relay4") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 4): led_off(relays, 4);
    }

    else if (strcmp(msg->topic, "sub/relay5") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 5): led_off(relays, 5);
    }

    else if (strcmp(msg->topic, "sub/relay6") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 6): led_off(relays, 6);
    }

    else if (strcmp(msg->topic, "sub/relay7") == 0)
    {
        int val = atoi(msg->msg);
        val ? led_on(relays, 7): led_off(relays, 7);
    }

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


