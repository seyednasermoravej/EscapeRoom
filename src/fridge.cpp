#include "fridge.h"

LOG_MODULE_REGISTER(fridge, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(fridge_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


Fridge:: Fridge(const char *room, const char *type): Puzzle(room, type)
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
    creatingMqttList(3);
    char topic[64];
    sprintf(topic, "%s/%s/", roomName, puzzleTypeName);
    keypad = new Keypad(topic);
}

void Fridge:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_fridge_display_topic;
	mqttList[1] = codeRed_fridge_relay1_topic;
	mqttList[2] = codeRed_fridge_relay2_topic;
    mqttCount = _mqttCount;

}


void Fridge:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received");
    if(strcmp(msg->topic, CODE_RED_FRIDGE_DISPLAY_TOPIC) == 0)
    {
        //??????????????????logic is unknown
        // if(strcmp(msg->msg, "on") == 0)
        // {
        //     gpio_pin_set_dt(&relays[0], 1);
        // }
        // else if(strcmp(msg->msg, "off") == 0)
        // {
        //     gpio_pin_set_dt(&relays[0], 0);
        // }
        // else
        // {
        //     LOG_INF("The command is not valid");
        // }
        //??????????????????logic is unknown
    }
    else if(strcmp(msg->topic, CODE_RED_FRIDGE_RELAY1_TOPIC) == 0)
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
    else if(strcmp(msg->topic, CODE_RED_FRIDGE_RELAY2_TOPIC) == 0)
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