#include "scale.h"

LOG_MODULE_REGISTER(scale, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),
static const struct gpio_dt_spec allRelays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(scale_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};

const struct device *hx711_dev = DEVICE_DT_GET(DT_NODELABEL(scale_loadcell));

Scale:: Scale(const char *room, const char *type): Puzzle(room, type)
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
    hx711 = new Hx711(hx711_dev);
    weight = hx711->measure();
    k_timer_init(&loadcellTimer, loadcellTimerHandler, NULL);
    k_timer_start(&loadcellTimer, K_SECONDS(3), K_SECONDS(1));
}

void Scale:: loadcellTimerHandler(struct k_timer *timer)
{
    Scale *instance = CONTAINER_OF(timer, Scale, loadcellTimer);
    
    struct sensor_value newWeight = instance->hx711->measure();
    if(newWeight.val1 != instance->weight.val1)
    {
        struct MqttMsg msg = {0};
        sprintf(msg.topic,"%sloadcell", instance->mqttCommand);
        sprintf(msg.msg, "%d", newWeight.val1);
        instance->weight.val1 = newWeight.val1;
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        
    }
}
void Scale:: creatingMqttList()
{
    
    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allRelays); i++)
    {
        sprintf(topic, "%srelay%d", mqttCommand, i + 1);
        mqttList[i + systemTopicsNo] = *createMqttTopic(topic);
    }
    mqttCount = ARRAY_SIZE(allRelays) + systemTopicsNo;

}

void Scale:: messageHandler(struct MqttMsg *msg)
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
            relayOperation(msg->msg, &allRelays[relayIdx], false);
        }
        else
        {
            LOG_ERR("Not a valid index");
        }
    }
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
