#include "blinds.h"

LOG_MODULE_REGISTER(blinds, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	PWM_DT_SPEC_GET_BY_IDX(node_id, idx),
static const struct pwm_dt_spec allServos[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(blinds_servos), pwms, DT_SPEC_AND_COMMA)
};

static const uint32_t servoMinPulse = DT_PROP(DT_NODELABEL(blinds_servos), min_pulse);
static const uint32_t servoMaxPulse = DT_PROP(DT_NODELABEL(blinds_servos), max_pulse);
static const uint16_t servoMaxDegrees = DT_PROP(DT_NODELABEL(blinds_servos), max_degrees);

Blinds:: Blinds(const char *room, const char *type): Puzzle(room, type)
{
    // int ret;
    servos = new Servos(allServos, ARRAY_SIZE(allServos), servoMinPulse, servoMaxPulse, servoMaxDegrees);
    creatingMqttList(4);
}

void Blinds:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_blinds_servo1_topic;
	mqttList[1] = codeRed_blinds_servo2_topic;
	mqttList[3] = codeRed_blinds_servo3_topic;
	mqttList[4] = codeRed_blinds_servo4_topic;
    mqttCount = _mqttCount;

}


void Blinds:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);

    if(strcmp(msg->topic, CODE_RED_BLINDS_SERVO1_TOPIC) == 0)
    {
        uint32_t val = atoi(msg->msg) + 90;
        servos->move(0, val);
    }
    else if(strcmp(msg->topic, CODE_RED_BLINDS_SERVO2_TOPIC) == 0)
    {
        uint32_t val = atoi(msg->msg) + 90;
        servos->move(1, val);
    }
    else if(strcmp(msg->topic, CODE_RED_BLINDS_SERVO3_TOPIC) == 0)
    {
        uint32_t val = atoi(msg->msg) + 90;
        servos->move(2, val);
    }
    else if(strcmp(msg->topic, CODE_RED_BLINDS_SERVO3_TOPIC) == 0)
    {
        uint32_t val = atoi(msg->msg) + 90;
        servos->move(3, val);
    }
    else
        LOG_INF("the command is not valid");
}