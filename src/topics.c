#include "topics.h"


struct mqtt_topic servo_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)SERVO0_TOPIC,
            .size = strlen(SERVO0_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic k3_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)"sub/k3",
            .size = strlen("sub/k3")
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};