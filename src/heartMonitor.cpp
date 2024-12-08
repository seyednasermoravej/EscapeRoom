#include "heartMonitor.h"

LOG_MODULE_REGISTER(heartMonitor, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),

static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(heart_monitor_relays), gpios, DT_SPEC_AND_COMMA_GATE)
};
static const struct gpio_dt_spec multiplexer[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(heart_monitor_analog_multiplexer), gpios, DT_SPEC_AND_COMMA_GATE)
};
#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),


/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};





HeartMonitor:: HeartMonitor(const char *room, const char *type): Puzzle(room, type)
{
    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(multiplexer); i++){
        if (!device_is_ready(multiplexer[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&multiplexer[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    adcs = new Adcs(adc_channels, 1);
    creatingMqttList(1);
    for(uint8_t i = 0; i < ARRAY_SIZE(analog); i++)
    {
        analog[i] = readAdc(i);
    }

    k_work_init(&analogWork, analogWorkHanlder);
    k_timer_init(&analogTimer, analogTimerHandler, NULL);
    k_timer_start(&analogTimer, K_SECONDS(3), K_SECONDS(5));
}

void HeartMonitor:: analogTimerHandler(struct k_timer *timer)
{
    HeartMonitor *instance = CONTAINER_OF(timer, HeartMonitor, analogTimer);
    k_work_submit(&instance -> analogWork);
}
void HeartMonitor:: analogWorkHanlder(struct k_work *work) 
{
    // Cast work to the correct type
    HeartMonitor *instance = CONTAINER_OF(work, HeartMonitor, analogWork);
    // Call the actual calibration function
    instance->updateAnalog(); // Pass appropriate parameters


}

void HeartMonitor:: updateAnalog()
{
    uint16_t temp;
    struct MqttMsg msg = {0};
    for (uint8_t i = 0; i < ARRAY_SIZE(analog); i++)
    {
        temp = readAdc(i);
        if((temp > (analog[i] + 50)) || (temp < analog[i] - 50))
        {
            LOG_INF("for the channel %u, previous value is: %u, new value: %u", i, analog[i], temp);
            analog[i] = temp;
            sprintf(msg.topic,"%s/%s/analog%d", roomName, puzzleTypeName, i + 1);
            sprintf(msg.msg, "%d", temp);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT); // Assuming k_msgq_put is defined elsewhere
        }
    }
}
void HeartMonitor:: creatingMqttList(uint16_t _mqttCount)
{
	mqttList[0] = codeRed_heartMonitor_relay1_topic;
    mqttCount = _mqttCount;

}


void HeartMonitor:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_HEART_MONITOR_RELAY1_TOPIC) == 0)
    {
        if(strcmp(msg->msg, "on") == 0)
        {
            gpio_pin_set_dt(&relays[0], 1);
            k_msleep(1000);
            gpio_pin_set_dt(&relays[0], 0);
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


uint16_t HeartMonitor:: readAdc(uint8_t channel)
{
    uint8_t val;
    for(uint32_t i = 0; i < 3; i++)
    {
        val = channel & BIT(i) ? 1: 0;
        gpio_pin_set_dt(&multiplexer[i], channel & BIT(i) ? 1: 0);
    }
    return adcs->readAdc(0);
}


void HeartMonitor:: test()
{
    struct MqttMsg msg = {0};
    sprintf(msg.topic, CODE_RED_HEART_MONITOR_RELAY1_TOPIC);
    sprintf(msg.msg, "on");
    while (1)
    {
        messageHandler(&msg);
        k_msleep(3000);
    }
    
}