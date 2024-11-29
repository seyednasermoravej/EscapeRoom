#include "blinds.h"

LOG_MODULE_REGISTER(blinds, LOG_LEVEL_INF);
#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	PWM_DT_SPEC_GET_BY_IDX(node_id, idx),
static const struct pwm_dt_spec allServos[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(blinds_servos), pwms, DT_SPEC_AND_COMMA)
};

#define STEP PWM_USEC(100)
static const uint32_t servoMinPulse = DT_PROP(DT_NODELABEL(blinds_servos), min_pulse);
static const uint32_t servoMaxPulse = DT_PROP(DT_NODELABEL(blinds_servos), max_pulse);

Blinds:: Blinds(const char *room, const char *type): Puzzle(room, type)
{
    // int ret;
    servos = new Servos(allServos, ARRAY_SIZE(allServos));
    creatingMqttList(4);
}

void Blinds:: creatingMqttList(uint16_t _mqttCount)
{

	mqttList[0] = codeRed_door_relay1_topic;
	mqttList[1] = codeRed_door_relay2_topic;
    mqttCount = _mqttCount;

}


void Blinds:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received");

    if(strcmp(msg->topic, SERVO0_TOPIC) == 0)
    {
        int val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
    }
    // if(strcmp(msg->topic, CODE_RED_DOOR_RELAY1_TOPIC) == 0)
    // {
    //     if(strcmp(msg->msg, "on") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[0], 1);
    //     }
    //     else if(strcmp(msg->msg, "off") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[0], 0);
    //     }
    //     else
    //     {
    //         LOG_INF("The command is not valid");
    //     }
    // }
    // else if(strcmp(msg->topic, CODE_RED_DOOR_RELAY2_TOPIC) == 0)
    // {
    //     if(strcmp(msg->msg, "on") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[1], 1);
    //     }
    //     else if(strcmp(msg->msg, "off") == 0)
    //     {
    //         gpio_pin_set_dt(&relays[1], 0);
    //     }
    //     else
    //     {
    //         LOG_INF("The command is not valid");
    //     }
    // } 
    else
        LOG_INF("the command is not valid");
}