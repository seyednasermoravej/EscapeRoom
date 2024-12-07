#include "powerPanel.h"

LOG_MODULE_REGISTER(powerPanel, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	PWM_DT_SPEC_GET_BY_IDX(node_id, idx),
static const struct pwm_dt_spec allServos[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(power_panel_servos), pwms, DT_SPEC_AND_COMMA)
};

#define STEP PWM_USEC(100)
static const uint32_t servoMinPulse = DT_PROP(DT_NODELABEL(power_panel_servos), min_pulse);
static const uint32_t servoMaxPulse = DT_PROP(DT_NODELABEL(power_panel_servos), max_pulse);
static const uint16_t servoMaxDegrees = DT_PROP(DT_NODELABEL(power_panel_servos), max_degrees);

PowerPanel:: PowerPanel(const char *room, const char *type): Puzzle(room, type)
{
    // int ret;
    servos = new Servos(allServos, ARRAY_SIZE(allServos), servoMinPulse, servoMaxPulse, servoMaxDegrees);
    creatingMqttList(17);
}

void PowerPanel:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_powerPanel_servo1_topic;
	mqttList[1] = codeRed_powerPanel_servo2_topic;
	mqttList[2] = codeRed_powerPanel_servo3_topic;
	mqttList[3] = codeRed_powerPanel_servo4_topic;
	mqttList[4] = codeRed_powerPanel_servo5_topic;
	mqttList[5] = codeRed_powerPanel_servo6_topic;
	mqttList[6] = codeRed_powerPanel_servo7_topic;
	mqttList[7] = codeRed_powerPanel_servo8_topic;

	mqttList[8] = codeRed_powerPanel_display1_topic;
	mqttList[9] = codeRed_powerPanel_display2_topic;
	mqttList[10] = codeRed_powerPanel_display3_topic;
	mqttList[11] = codeRed_powerPanel_display4_topic;
	mqttList[12] = codeRed_powerPanel_display5_topic;
	mqttList[13] = codeRed_powerPanel_display6_topic;
	mqttList[14] = codeRed_powerPanel_display7_topic;
	mqttList[15] = codeRed_powerPanel_display8_topic;
	mqttList[16] = codeRed_powerPanel_display9_topic;
    mqttCount = _mqttCount;

}


void PowerPanel:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);

    if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO1_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(0, val);
    }
    else if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO2_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(1, val);
    }
    else if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO3_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(2, val);
    }
    else if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO4_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(3, val);
    }
    else if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO5_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(4, val);
    }
    else if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO6_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(5, val);
    }
    else if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO7_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(6, val);
    }
    else if(strcmp(msg->topic, CODE_RED_POWER_PANEL_SERVO8_TOPIC) == 0)
    {
        uint32_t val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        servos->move(7, val);
    }
    else
        LOG_INF("the command is not valid");
}