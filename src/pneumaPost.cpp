#include "pneumaPost.h"


LOG_MODULE_REGISTER(pneuma_post, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(pneuma_post_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(pneuma_post_buttons));

static PneumaPost *instance = nullptr;

void PneumaPost:: buttonsHandlerWrapper(struct input_event *val, void *userData)
{
    instance->buttonsHandler(val);
}
void PneumaPost:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        if(val->code == INPUT_BTN_0)
        {
            sprintf(msg.topic, "%s/%s/switch1", roomName, puzzleTypeName);
            val->value ? sprintf(msg.msg, "true"): sprintf(msg.msg, "false");
        }
        else
        {
            if(val->value)
            {
                sprintf(msg.topic, "%s/%s/button%d", roomName, puzzleTypeName, (val->code - INPUT_BTN_0));
                sprintf(msg.msg, "true");
            }
        }
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
}
PneumaPost:: PneumaPost(const char *room, const char *type): Puzzle(room, type)
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
    creatingMqttList(2);
    instance = this;
    device_init(buttons);
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void *)this);
}

void PneumaPost:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_pneumaPost_relay1_topic;
	mqttList[1] = codeRed_pneumaPost_relay2_topic;
    mqttCount = _mqttCount;

}


void PneumaPost:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_PNEUMA_POST_RELAY1_TOPIC) == 0)
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
    else if(strcmp(msg->topic, CODE_RED_PNEUMA_POST_RELAY2_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[1], 1);
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
    else
        LOG_INF("the command is not valid");
}