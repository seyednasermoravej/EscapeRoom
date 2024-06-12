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

            .utf8 = (uint8_t*)K3_TOPIC,
            .size = strlen(K3_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic lcd_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)LCD_TOPIC,
            .size = strlen(LCD_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};