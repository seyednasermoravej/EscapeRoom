
#include "drawers.h"


LOG_MODULE_REGISTER(drawers, LOG_LEVEL_DBG);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
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
        sprintf(msg.topic, "%sswitch%d", mqttCommand, (val->code - INPUT_BTN_0 + 1));
        val->value ? sprintf(msg.msg, "true"): sprintf(msg.msg, "false");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
}
Drawers:: Drawers(const char *room, const char *type): Puzzle(room, type)
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
    creatingMqttList(8);
    instance = this;
    device_init(buttons);

    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void *)this);
}

void Drawers:: creatingMqttList(uint16_t _mqttCount)
{
    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }
    mqttCount = ARRAY_SIZE(allRelays);

}


void Drawers:: messageHandler(struct MqttMsg *msg)
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
            if(strcmp(msg->msg, "on") == 0)
            {
                gpio_pin_set_dt(&allRelays[relayIdx], 1);
                k_msleep(1000);
                gpio_pin_set_dt(&allRelays[relayIdx], 0);
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
        else
        {
            LOG_ERR("Not a valid index");
        }
    }
}