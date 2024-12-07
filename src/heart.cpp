#include "heart.h"

LOG_MODULE_REGISTER(heart, LOG_LEVEL_INF);

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	PWM_DT_SPEC_GET_BY_IDX(node_id, idx),
static const struct pwm_dt_spec allServos[] = {
    PWM_DT_SPEC_GET(DT_NODELABEL(heart_servos))
};
// static const struct pwm_dt_spec allServos[] = {
//     DT_FOREACH_PROP_ELEM(DT_NODELABEL(heart_servos), pwms, DT_SPEC_AND_COMMA)
// };
static const uint32_t servoMinPulse = DT_PROP(DT_NODELABEL(heart_servos), min_pulse);
static const uint32_t servoMaxPulse = DT_PROP(DT_NODELABEL(heart_servos), max_pulse);
static const uint16_t servoMaxDegrees = DT_PROP(DT_NODELABEL(heart_servos), max_degrees);
Heart:: Heart(const char *room, const char *type): Puzzle(room, type)
{
    creatingMqttList(1);
    servos = new Servos(allServos, 1, servoMinPulse, servoMaxPulse, servoMaxDegrees);
}

void Heart:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_heart_servo_topic;
    mqttCount = _mqttCount;

}


void Heart:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(strcmp(msg->topic, CODE_RED_HEART_SERVO_TOPIC) == 0)
    {
        uint32_t val = atoi(msg->msg) + 90;
        servos->move(0, val);
    } 
    else
        LOG_INF("the command is not valid");
}