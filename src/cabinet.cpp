#include "cabinet.h"



LOG_MODULE_REGISTER(cabient, LOG_LEVEL_INF);



static const char roomName[] = "introRoom";
static const char puzzleTypeName[] = "cabinet";

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec cabinet_puzzle_relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(cabinet_puzzle_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};


static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(cabinet_puzzle_input));

static void buttonsHandler(struct input_event *val, void* topic)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};

        if(val->value)
        {
            sprintf(msg.topic, "%s/%s/input%d", roomName, puzzleTypeName, (val->code - 0x100) + 1);
            sprintf(msg.msg, "TRUE");
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
      
    }
}

Cabinet:: Cabinet()
{
    device_init(buttons);
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandler, NULL);
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
}


void Cabinet::alive()
{
    struct MqttMsg msg = {0};
    sprintf(msg.topic, "%s/%s/alive", roomName, puzzleTypeName);
    sprintf(msg.msg, "TRUE");
    k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
}



void Cabinet:: messageHandler(struct MqttMsg *msg)
{
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