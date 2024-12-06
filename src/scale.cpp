#include "scale.h"

LOG_MODULE_REGISTER(scale, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(scale_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

const struct device *hx711_dev = DEVICE_DT_GET(DT_NODELABEL(scale_loadcell));

Scale:: Scale(const char *room, const char *type): Puzzle(room, type)
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
    creatingMqttList(1);
    hx711 = new Hx711(hx711_dev);
    k_timer_init(&loadcellTimer, loadcellTimerHandler, NULL);
    k_timer_start(&loadcellTimer, K_SECONDS(3), K_SECONDS(5));
}

void Scale:: loadcellTimerHandler(struct k_timer *timer)
{
    Scale *instance = CONTAINER_OF(timer, Scale, loadcellTimer);
    struct sensor_value weight = instance->hx711->measure();
    struct MqttMsg msg = {0};
    sprintf(msg.topic,"%s/%s/loadcell", instance->roomName, instance->puzzleTypeName);
    sprintf(msg.msg, "%d", weight.val1);
    k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
}
void Scale:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_scale_relay1_topic;
    mqttCount = _mqttCount;

}

void Scale:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_SCALE_RELAY1_TOPIC) == 0)
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









        // k_msleep(1000);
		// // LOG_INF("== Test measure ==");
		// // LOG_INF("= Setting sampling rate to 10Hz.");
		// // set_rate(HX711_RATE_10HZ);
		// measure();

		// // k_msleep(1000);
		// // LOG_INF("= Setting sampling rate to 80Hz.");
		// // set_rate(HX711_RATE_80HZ);
		// // measure();
