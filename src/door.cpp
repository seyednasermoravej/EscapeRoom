#include "door.h"

LOG_MODULE_REGISTER(door, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(door_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


Door:: Door(const char *room, const char *type): Puzzle(room, type)
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
}

void Door:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_door_relay1_topic;
	mqttList[1] = codeRed_door_relay2_topic;
    mqttCount = _mqttCount;

}


void Door:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_DOOR_RELAY1_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[0], 1);
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
    else if(strcmp(msg->topic, CODE_RED_DOOR_RELAY2_TOPIC) == 0)
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