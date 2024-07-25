#ifndef __TOPICS__H__
#define __TOPICS__H__


#include <zephyr/net/mqtt.h>
#define SERVO0_TOPIC            "sub/servo0"
#define K3_TOPIC                "sub/relaysControl"
#define LCD_TOPIC               "sub/lcd"
#define LCD1_TOPIC              "sub/lcd1"
#define LCD2_TOPIC              "sub/lcd2"
#define BUILT_IN_LED_TOPIC      "sub/builtInLed"
#define PUZZLE_TYPE_TOPIC       "sub/puzzleType"


#define DEV_ADDR_TOPIC          "pub/devAddr"
#define RFID_IN_TOPIC           "pub/rfidIn" 
#define ROTARY_ENCODER_TOPIC    "pub/rotaryEncoder"
#define LOGS_TOPIC              "pub/logs"
#define KEYPAD_TOPIC            "pub/keypad"


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