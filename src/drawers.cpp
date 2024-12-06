
#include "drawers.h"


LOG_MODULE_REGISTER(drawers, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(drawers_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(drawers_buttons));

static Drawers *instance = nullptr;

void Drawers:: buttonsHandlerWrapper(struct input_event *val, void *userData)
{
    instance->buttonsHandler(val);
}
void Drawers:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        sprintf(msg.topic, "%s/%s/switch%d", roomName, puzzleTypeName, (val->code - INPUT_BTN_0 + 1));
        val->value ? sprintf(msg.msg, "true"): sprintf(msg.msg, "false");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
}
Drawers:: Drawers(const char *room, const char *type): Puzzle(room, type)
{
    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(relays); i++){
        if (!device_is_ready(relays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&relays[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    creatingMqttList(8);
    instance = this;
    device_init(buttons);
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void *)this);
}

void Drawers:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_drawers_relay1_topic;
	mqttList[1] = codeRed_drawers_relay2_topic;
	mqttList[2] = codeRed_drawers_relay3_topic;
	mqttList[3] = codeRed_drawers_relay4_topic;
	mqttList[4] = codeRed_drawers_relay5_topic;
	mqttList[5] = codeRed_drawers_relay6_topic;
	mqttList[6] = codeRed_drawers_relay7_topic;
	mqttList[7] = codeRed_drawers_relay8_topic;
    mqttCount = _mqttCount;

}


void Drawers:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY1_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[0], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[0], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[0], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY2_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[1], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[1], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[1], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY3_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[2], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[2], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[2], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY4_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[3], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[3], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[3], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY5_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[4], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[4], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[4], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY6_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[5], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[5], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[5], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY7_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[6], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[6], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[6], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, CODE_RED_DRAWERS_RELAY8_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[7], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[7], 0);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&relays[7], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else
        LOG_INF("the command is not valid");
}