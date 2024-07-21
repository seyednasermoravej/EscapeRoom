#include "servos.h"

LOG_MODULE_REGISTER(servos, LOG_LEVEL_INF);

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	PWM_DT_SPEC_GET_BY_IDX(node_id, idx),



static const uint32_t servoMinPulse = DT_PROP(DT_NODELABEL(servos), min_pulse);
static const uint32_t servoMaxPulse = DT_PROP(DT_NODELABEL(servos), max_pulse);
static const struct pwm_dt_spec allServos[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(servos), pwms, DT_SPEC_AND_COMMA)
};

Servos:: Servos()
{
    device_init(allServos->dev);
    for (size_t i = 0; i < ARRAY_SIZE(allServos); i++) {
        if (!pwm_is_ready_dt(&allServos[i])) {
            printk("Error: servo device %s is not ready\n",
                allServos[i].dev->name);
        }
    }
}


void Servos:: messageHandler(struct MqttMsg *msg)
{

    if(strcmp(msg->topic, SERVO0_TOPIC) == 0)
    {
        int val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        pwm_set_pulse_dt(&allServos[0], val);
    }
}