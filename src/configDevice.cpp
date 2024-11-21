#include "configDevice.h"

LOG_MODULE_REGISTER(configDevice, LOG_LEVEL_INF);


#define LCD1_NODE DT_NODELABEL(lcd1)
#define LCD2_NODE DT_NODELABEL(lcd2)


#define DT_SPEC_AND_COMMA_CONFIG_DEVICE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),



static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(config_puzzle_buttons));

ConfigDevice:: ConfigDevice()
{
    // inputsInit();
	device_init(DEVICE_DT_GET(LCD1_NODE));
	// device_init(DEVICE_DT_GET(LCD2_NODE));
	lcd1 = new Lcd(DEVICE_DT_GET(LCD1_NODE), 0, 2, 3, 4, 5, 6, 7);
	// lcd2 = new Lcd(DEVICE_DT_GET(LCD2_NODE), 0, 2, 3, 4, 5, 6, 7);
}

void rotaryChange(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
    struct MqttMsg msg = {0};
    k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
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
        lcd1->firstLine(msg->msg);
    }
    else if(strcmp(msg->topic, LCD2_TOPIC) == 0)
    {
        lcd2->firstLine(msg->msg);
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



static void buttonsHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        strcpy(msg.topic, "pub/end time");
        // strcpy(msg.topic, (char *)topic);
        if((val->code == INPUT_BTN_0) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_1) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_2) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_3) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_4) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_5) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_6) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_7) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_8) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        if((val->code == INPUT_BTN_9) && (val->value))
        {

            LOG_INF("End time pressed");
            sprintf(msg.msg, "End time pressed");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
    }


}



int ConfigDevice:: inputsInit()
{
    device_init(buttons);
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandler, NULL);
    int ret;
	return ret;
}