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
#define LED1_TOPIC              "sub/led1"
#define LED2_TOPIC              "sub/led2"
#define LED3_TOPIC              "sub/led3"
#define LED4_TOPIC              "sub/led4"
#define LED5_TOPIC              "sub/led5"
#define LED6_TOPIC              "sub/led6"
#define LED7_TOPIC              "sub/led7"
#define LED8_TOPIC              "sub/led8"


#define SEG1_TOPIC              "pub/seg1"
#define SEG2_TOPIC              "pub/seg2"
#define SEG3_TOPIC              "pub/seg3"
#define SEG4_TOPIC              "pub/seg4"
#define SEG5_TOPIC              "pub/seg5"
#define SEG6_TOPIC              "pub/seg6"
#define SEG7_TOPIC              "pub/seg7"
#define SEG8_TOPIC              "pub/seg8"

#define DEV_ADDR_TOPIC          "pub/devAddr"
#define RFID_IN_TOPIC           "pub/rfidIn" 
#define ROTARY_ENCODER_TOPIC    "pub/rotaryEncoder"
#define LOGS_TOPIC              "pub/logs"
#define KEYPAD_TOPIC            "pub/keypad"

#define SET_AASD_SPEED          "sub/aasdSpeed"
#define SET_STEPPER_POSITION    "sub/stepperPosition"
#define GET_AASD_SPEED          "pub/aasdSpeed" 
#define GET_AASD_POSITION       "pub/aasdPosition"             



#ifdef __cplusplus
extern "C" {
#endif

extern struct mqtt_topic servo_topic;
extern struct mqtt_topic k3_topic;
extern struct mqtt_topic lcd_topic;
extern struct mqtt_topic builtInLed_topic;
extern struct mqtt_topic puzzleType_topic;
extern struct mqtt_topic led1_topic;
extern struct mqtt_topic led2_topic;
extern struct mqtt_topic led3_topic;
extern struct mqtt_topic led4_topic;
extern struct mqtt_topic led5_topic;
extern struct mqtt_topic led6_topic;
extern struct mqtt_topic led7_topic;
extern struct mqtt_topic led8_topic;
extern struct mqtt_topic aasdSpeed_topic;
extern struct mqtt_topic aasdPosition_topic;

#ifdef __cplusplus
}
#endif
#endif