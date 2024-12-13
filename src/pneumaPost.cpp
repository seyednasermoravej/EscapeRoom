#include "pneumaPost.h"


LOG_MODULE_REGISTER(pneuma_post, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
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
    for(unsigned int i = 0; i < ARRAY_SIZE(allRelays); i++){
        if (!device_is_ready(allRelays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&allRelays[i], GPIO_OUTPUT_INACTIVE);
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

    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%s/%s/relay%d", roomName, puzzleTypeName, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }
    mqttCount = _mqttCount;

}


void PneumaPost:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);

    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        char field[] = "relay";
        int commandIdx = peripheralIdx(field, command);
        int relayIdx = commandIdx - 1;
        if((commandIdx > 0 ) && (relayIdx < ARRAY_SIZE(allRelays)))
        {
            if(!relayIdx)
            {
                if(strcmp(msg->msg, "on") == 0)
                {
                    gpio_pin_set_dt(&allRelays[relayIdx], 1);
                    k_msleep(1000);
                    gpio_pin_set_dt(&allRelays[relayIdx], 0);
                }
                else
                {
                    LOG_INF("The command is not valid");
                }
            }
            else 
            {
                if(strcmp(msg->msg, "on") == 0)
                {
                    gpio_pin_set_dt(&allRelays[relayIdx], 1);
                }
                else if(strcmp(msg->msg, "off") == 0)
                {
                    gpio_pin_set_dt(&allRelays[relayIdx], 0);
                }
                else
                {
                    LOG_INF("The command is not valid");
                }
            }
        }
    }
    else
    {
        LOG_INF("the command is not valid");
    }
}