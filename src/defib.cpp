
#include "defib.h"


LOG_MODULE_REGISTER(defib, LOG_LEVEL_DBG);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(defib_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(defib_buttons));

static Defib *instance = nullptr;

void Defib:: buttonsHandlerWrapper(struct input_event *val, void *userData)
{
    instance->buttonsHandler(val);
}
void Defib:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};
        if(val->value)
        {
            sprintf(msg.topic, "%sbutton%d", instance ->mqttCommand, (val->code - INPUT_BTN_0));
            sprintf(msg.msg, "true");
            LOG_INF("%s, %s", msg.topic, msg.msg);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        } 
    }
}
Defib:: Defib(const char *room, const char *type): Puzzle(room, type)
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
    creatingMqttList();
    instance = this;
    device_init(buttons);

    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void *)this);
}

void Defib:: creatingMqttList()
{
    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[i + systemTopicsNo] = *createMqttTopic(topic);
    }
    mqttCount = ARRAY_SIZE(allRelays) + systemTopicsNo;

}


void Defib:: messageHandler(struct MqttMsg *msg)
{
    int rc;
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if(strstr(command, "relay") != NULL)
        {
            char field[] = "relay";
            int commandIdx = peripheralIdx(field, command);
            uint8_t relayIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (relayIdx < ARRAY_SIZE(allRelays)))
            {
                relayOperation(msg->msg, &allRelays[relayIdx], false);
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else
        {
            LOG_INF("the command is not valid");
        }
    }
    else
        LOG_INF("the command is not valid");
}