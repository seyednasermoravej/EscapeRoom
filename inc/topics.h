#ifndef __TOPICS__H__
#define __TOPICS__H__


#include <zephyr/net/mqtt.h>
#define SERVO0_TOPIC    "sub/servo0"
#ifdef __cplusplus
extern "C" {
#endif

extern struct mqtt_topic servo_topic;
extern struct mqtt_topic k3_topic;


#ifdef __cplusplus
}
#endif
#endif