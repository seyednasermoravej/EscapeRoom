#ifndef __TOPICS__H__
#define __TOPICS__H__


#include <zephyr/net/mqtt.h>
#define SERVO0_TOPIC            "sub/servo0"
#define K3_TOPIC                "sub/relaysControl"
#define LCD_TOPIC               "sub/lcd"
#define BUILT_IN_LED_TOPIC      "sub/builtInLed"
#define PUZZLE_TYPE_TOPIC       "sub/puzzleType"
#define DEV_ADDR_TOPIC          "sub/devAddr"
#ifdef __cplusplus
extern "C" {
#endif

extern struct mqtt_topic servo_topic;
extern struct mqtt_topic k3_topic;
extern struct mqtt_topic lcd_topic;
extern struct mqtt_topic builtInLed_topic;
extern struct mqtt_topic puzzleType_topic;

#ifdef __cplusplus
}
#endif
#endif