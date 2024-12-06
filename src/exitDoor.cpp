#include "exitDoor.h"

LOG_MODULE_REGISTER(exitDoor, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(exit_door_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),


//  //const struct gpio_dt_spec spec = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(leds), gpios, 1);
static const struct gpio_dt_spec wiegandIos[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(exit_door_rfid), gpios, DT_SPEC_AND_COMMA_GATE)
};

ExitDoor:: ExitDoor(const char *room, const char *type): Puzzle(room, type)
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
    wg = new WIEGAND(wiegandIos[0].port, wiegandIos[0].pin, wiegandIos[1].port, wiegandIos[1].pin);
    creatingMqttList(1);
    k_timer_init(&wiegandTimer, wiegandTimerHandler, NULL);
    k_timer_start(&wiegandTimer, K_SECONDS(2), K_SECONDS(1));
}

void ExitDoor:: wiegandTimerHandler(struct k_timer *timer)
{
    ExitDoor *instance = CONTAINER_OF(timer, ExitDoor, wiegandTimer);
    if(instance->wg->available())
    {
        unsigned long code = instance->wg->getCode();
        struct MqttMsg msg = {0};
        sprintf(msg.topic, "%s/%s/rfid1", instance->roomName, instance ->puzzleTypeName);
        sprintf(msg.msg, "%lx", code);
        k_msgq_put(&msqSendToMQTT, &msg, K_FOREVER);
        LOG_INF("New tag read: %lx", code);
    }
}
void ExitDoor:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_exitDoor_relay1_topic;
    mqttCount = _mqttCount;

}


void ExitDoor:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_EXIT_DOOR_RELAY1_TOPIC) == 0)
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
    else
        LOG_INF("the command is not valid");
}