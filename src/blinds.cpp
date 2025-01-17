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
    servos->move(0, 90);
    servos->move(1, 90);
    servos->move(2, 90);
    servos->move(3, 90);

    creatingMqttList(4);
}

void Blinds:: creatingMqttList(uint16_t _mqttCount)
{

    char topic[128] = {0};
    for(uint8_t i = 0; i < ARRAY_SIZE(allServos); i++)
    {
        sprintf(topic, "%sservo%d", mqttCommand, i + 1);
        mqttList[i] = *createMqttTopic(topic);
    }
    mqttCount = ARRAY_SIZE(allServos);
}


void Blinds:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);

    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        char field[] = "servo";
        int commandIdx = peripheralIdx(field, command);
        int servoIdx = commandIdx - 1;
        if((commandIdx > 0 ) && (servoIdx < ARRAY_SIZE(allServos)))
        {
            uint32_t val = atoi(msg->msg) + 90;
            if(val < 15)
            {
                val = 15;
            }
            if(val > 165)
            {
                val = 165;
            }
            servos->move(servoIdx, val);
        }
        else
        {
            LOG_ERR("Not a valid index");
        }
    }

}

void Blinds:: test()
{
    // struct MqttMsg msg = {0};
    while(1)
    {
        // sprintf(msg.topic, (char *)mqttList[0].topic.utf8);
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(1000);
        
        // sprintf(msg.msg, "0");
        // messageHandler(&msg);
        // k_msleep(1000);

        // sprintf(msg.msg, "90");
        // messageHandler(&msg);
        // k_msleep(1000);
        
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(2000);

        // sprintf(msg.topic, (char *)mqttList[1].topic.utf8);
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(1000);
        // sprintf(msg.msg, "0");
        // messageHandler(&msg);
        // k_msleep(1000);
        // sprintf(msg.msg, "90");
        // messageHandler(&msg);
        // k_msleep(1000);
        
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(2000);

        // sprintf(msg.topic, (char *)mqttList[2].topic.utf8);
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(1000);
        // sprintf(msg.msg, "0");
        // messageHandler(&msg);
        // k_msleep(1000);
        // sprintf(msg.msg, "90");
        // messageHandler(&msg);
        // k_msleep(1000);
        
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(2000);

        // sprintf(msg.topic, (char *)mqttList[3].topic.utf8);
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(1000);
        // sprintf(msg.msg, "0");
        // messageHandler(&msg);
        // k_msleep(1000);
        // sprintf(msg.msg, "90");
        // messageHandler(&msg);
        // k_msleep(1000);
        
        // sprintf(msg.msg, "-90");
        // messageHandler(&msg);
        // k_msleep(2000);
    }
}