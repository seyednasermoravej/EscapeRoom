#ifndef __TOPICS__H__
#define __TOPICS__H__


#include <zephyr/net/mqtt.h>

#define STATUS_TOPIC            "pub/status"

#define SERVO0_TOPIC            "sub/servo0"
#define K3_TOPIC                "sub/relaysControl"
#define LCD_TOPIC               "sub/lcd"
#define LCD1_TOPIC              "introRoom/console/lcd1"
#define LCD2_TOPIC              "introRoom/console/lcd2"
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
#define KEYPAD_TOPIC                       "pub/keypad"

#define SET_STEPPER_TIME_POSITION          "sub/stepperTimePosition"
#define SET_STEPPER_GO_TO_START_POSITION          "sub/stepperGoToStartPosition"
#define SET_STEPPER_STOP          "sub/stepperStop"
#define SET_STEPPER_POSITION       "sub/stepperPosition"


// #define RELAY1_TOPIC                                       "sub/relay1" 
// #define RELAY2_TOPIC                                       "sub/relay2" 
// #define RELAY3_TOPIC                                       "sub/relay3" 
// #define RELAY4_TOPIC                                       "sub/relay4" 
// #define RELAY5_TOPIC                                       "sub/relay5" 
// #define RELAY6_TOPIC                                       "sub/relay6" 
// #define RELAY7_TOPIC                                       "sub/relay7" 
// #define RELAY8_TOPIC                                       "sub/relay8" 


#define RELAY1_TOPIC                                       "introRoom/console/relay1" 
#define RELAY2_TOPIC                                       "introRoom/console/relay2" 
#define RELAY3_TOPIC                                       "introRoom/console/relay3" 
#define RELAY4_TOPIC                                       "introRoom/console/relay4" 
#define RELAY5_TOPIC                                       "introRoom/console/relay5" 
#define RELAY6_TOPIC                                       "introRoom/console/relay6" 
#define RELAY7_TOPIC                                       "introRoom/console/relay7" 
#define RELAY8_TOPIC                                       "introRoom/console/relay8" 


#define INTRO_ROOM_CABINET_RELAY1_TOPIC                                       "introRoom/cabinet/relay1" 
#define INTRO_ROOM_CABINET_RELAY2_TOPIC                                       "introRoom/cabinet/relay2" 
#define INTRO_ROOM_CABINET_RELAY3_TOPIC                                       "introRoom/cabinet/relay3" 
#define INTRO_ROOM_CABINET_RELAY4_TOPIC                                       "introRoom/cabinet/relay4" 






#ifdef __cplusplus
extern "C" {
#endif

extern char deviceIdPub[32];


extern struct mqtt_topic servo_topic;
extern struct mqtt_topic k3_topic;
extern struct mqtt_topic lcd_topic;
extern struct mqtt_topic lcd1_topic;
extern struct mqtt_topic lcd2_topic;
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
extern struct mqtt_topic stepperSpeed_topic;
extern struct mqtt_topic stepperPosition_topic;
extern struct mqtt_topic stepperGoToStartPosition_topic;
extern struct mqtt_topic stepperStop_topic;
extern struct mqtt_topic deviceId_topic;

extern struct mqtt_topic relay0_topic;
extern struct mqtt_topic relay1_topic;
extern struct mqtt_topic relay2_topic;
extern struct mqtt_topic relay3_topic;
extern struct mqtt_topic relay4_topic;
extern struct mqtt_topic relay5_topic;
extern struct mqtt_topic relay6_topic;
extern struct mqtt_topic relay7_topic;


#ifdef __cplusplus
}
#endif
#endif