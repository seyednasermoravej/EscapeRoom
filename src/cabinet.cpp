#include "cabinet.h"



LOG_MODULE_REGISTER(cabient, LOG_LEVEL_INF);


static Cabinet *instance = nullptr;

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec cabinet_puzzle_relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(cabinet_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(cabinet_input));


void Cabinet:: buttonsHandlerWrapper(struct input_event *val, void *userData)
{
    instance->buttonsHandler(val);
}


void Cabinet:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};

        if(val->value)
        {
            sprintf(msg.topic, "%s/%s/input%d", roomName, puzzleTypeName, (val->code - INPUT_BTN_0) + 1);
            sprintf(msg.msg, "TRUE");
            LOG_INF("%s, %s", msg.topic, msg.msg);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
      
    }

}


Cabinet:: Cabinet(const char * room, const char *type): Puzzle(room, type)
{
    LOG_INF("%s/%s", room, type);
    LOG_INF("%s/%s", roomName, puzzleTypeName);
    device_init(buttons);
    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(cabinet_puzzle_relays); i++){
        if (!device_is_ready(cabinet_puzzle_relays[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&cabinet_puzzle_relays[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    creatingMqttList(4);
    instance = this;
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void*)this);
}

void Cabinet:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = introRoom_cabinet_relay1_topic;
	mqttList[1] = introRoom_cabinet_relay2_topic;
	mqttList[2] = introRoom_cabinet_relay3_topic;
	mqttList[3] = introRoom_cabinet_relay4_topic;
    mqttCount = _mqttCount;

}
void Cabinet:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received");
    if(strcmp(msg->topic, INTRO_ROOM_CABINET_RELAY1_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[0], 1);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[0], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, INTRO_ROOM_CABINET_RELAY2_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[1], 1);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[1], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, INTRO_ROOM_CABINET_RELAY3_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[2], 1);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[2], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else if(strcmp(msg->topic, INTRO_ROOM_CABINET_RELAY4_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[3], 1);
        }
        else if(strcmp(msg->msg, "off") == 0)
        {
            gpio_pin_set_dt(&cabinet_puzzle_relays[3], 0);
        }
        else
        {
            LOG_INF("The command is not valid");
        }
    }
    else
        LOG_INF("the command is not valid");
}