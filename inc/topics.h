#ifndef __TOPICS__H__
#define __TOPICS__H__


#include <zephyr/net/mqtt.h>


#define PUZZLE_TYPE_TOPIC       "puzzleType"
#define BUILT_IN_LED_TOPIC      "sub/builtInLed"


#define INTRO_ROOM_CONSOLE_LCD1_TOPIC                                         "introRoom/console/lcd1"
#define INTRO_ROOM_CONSOLE_LCD2_TOPIC                                         "introRoom/console/lcd2"

#define INTRO_ROOM_PLATFORM_POSITION_TOPIC                                    "introRoom/platform/position"


#define INTRO_ROOM_CABINET_RELAY1_TOPIC                                       "introRoom/cabinet/relay1" 
#define INTRO_ROOM_CABINET_RELAY2_TOPIC                                       "introRoom/cabinet/relay2" 
#define INTRO_ROOM_CABINET_RELAY3_TOPIC                                       "introRoom/cabinet/relay3" 
#define INTRO_ROOM_CABINET_RELAY4_TOPIC                                       "introRoom/cabinet/relay4" 


#define CODE_RED_DOOR_RELAY1_TOPIC                                       "codeRed/door/relay1" 
#define CODE_RED_DOOR_RELAY2_TOPIC                                       "codeRed/door/relay2" 


#define CODE_RED_DOOR_KEYPAD_DISPLAY_TOPIC                                       "codeRed/doorKeypad/display" 


#define CODE_RED_VENTILATOR_RELAY1_TOPIC                                       "codeRed/ventilator/relay1" 
#define CODE_RED_VENTILATOR_RELAY2_TOPIC                                       "codeRed/ventilator/relay2" 
#define CODE_RED_VENTILATOR_RELAY3_TOPIC                                       "codeRed/ventilator/relay3" 
#define CODE_RED_VENTILATOR_RELAY4_TOPIC                                       "codeRed/ventilator/relay4" 



#define CODE_RED_FRIDGE_DISPLAY_TOPIC                                       "codeRed/fridge/display" 
#define CODE_RED_FRIDGE_RELAY1_TOPIC                                       "codeRed/fridge/relay1" 
#define CODE_RED_FRIDGE_RELAY2_TOPIC                                       "codeRed/fridge/relay2" 



#define CODE_RED_BLINDS_SERVO1_TOPIC                                       "codeRed/blinds/servo1" 
#define CODE_RED_BLINDS_SERVO2_TOPIC                                       "codeRed/blinds/servo2" 
#define CODE_RED_BLINDS_SERVO3_TOPIC                                       "codeRed/blinds/servo3" 
#define CODE_RED_BLINDS_SERVO4_TOPIC                                       "codeRed/blinds/servo4" 


#define CODE_RED_POWER_PANEL_SERVO1_TOPIC                                       "codeRed/powerPanel/servo1" 
#define CODE_RED_POWER_PANEL_SERVO2_TOPIC                                       "codeRed/powerPanel/servo2" 
#define CODE_RED_POWER_PANEL_SERVO3_TOPIC                                       "codeRed/powerPanel/servo3" 
#define CODE_RED_POWER_PANEL_SERVO4_TOPIC                                       "codeRed/powerPanel/servo4" 
#define CODE_RED_POWER_PANEL_SERVO5_TOPIC                                       "codeRed/powerPanel/servo5" 
#define CODE_RED_POWER_PANEL_SERVO6_TOPIC                                       "codeRed/powerPanel/servo6" 
#define CODE_RED_POWER_PANEL_SERVO7_TOPIC                                       "codeRed/powerPanel/servo7" 
#define CODE_RED_POWER_PANEL_SERVO8_TOPIC                                       "codeRed/powerPanel/servo8" 
#define CODE_RED_POWER_PANEL_DISPLAY1_TOPIC                                       "codeRed/powerPanel/display1" 
#define CODE_RED_POWER_PANEL_DISPLAY2_TOPIC                                       "codeRed/powerPanel/display2" 
#define CODE_RED_POWER_PANEL_DISPLAY3_TOPIC                                       "codeRed/powerPanel/display3" 
#define CODE_RED_POWER_PANEL_DISPLAY4_TOPIC                                       "codeRed/powerPanel/display4" 
#define CODE_RED_POWER_PANEL_DISPLAY5_TOPIC                                       "codeRed/powerPanel/display5" 
#define CODE_RED_POWER_PANEL_DISPLAY6_TOPIC                                       "codeRed/powerPanel/display6" 
#define CODE_RED_POWER_PANEL_DISPLAY7_TOPIC                                       "codeRed/powerPanel/display7" 
#define CODE_RED_POWER_PANEL_DISPLAY8_TOPIC                                       "codeRed/powerPanel/display8" 
#define CODE_RED_POWER_PANEL_DISPLAY9_TOPIC                                       "codeRed/powerPanel/display9" 

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














#define SERVO0_TOPIC            "sub/servo0"
#define K3_TOPIC                "sub/relaysControl"


#ifdef __cplusplus
extern "C" {
#endif

extern char deviceIdPub[32];


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
extern struct mqtt_topic deviceId_topic;

extern struct mqtt_topic relay0_topic;
extern struct mqtt_topic relay1_topic;
extern struct mqtt_topic relay2_topic;
extern struct mqtt_topic relay3_topic;
extern struct mqtt_topic relay4_topic;
extern struct mqtt_topic relay5_topic;
extern struct mqtt_topic relay6_topic;
extern struct mqtt_topic relay7_topic;


extern struct mqtt_topic introRoom_platform_position_topic;


extern struct mqtt_topic introRoom_console_lcd1_topic;
extern struct mqtt_topic introRoom_console_lcd2_topic;

extern struct mqtt_topic introRoom_cabinet_relay1_topic;
extern struct mqtt_topic introRoom_cabinet_relay2_topic;
extern struct mqtt_topic introRoom_cabinet_relay3_topic;
extern struct mqtt_topic introRoom_cabinet_relay4_topic;


extern struct mqtt_topic codeRed_door_relay1_topic;
extern struct mqtt_topic codeRed_door_relay2_topic;


extern struct mqtt_topic codeRed_doorKeypad_display_topic;



extern struct mqtt_topic codeRed_ventilator_relay1_topic;
extern struct mqtt_topic codeRed_ventilator_relay2_topic;
extern struct mqtt_topic codeRed_ventilator_relay3_topic;
extern struct mqtt_topic codeRed_ventilator_relay4_topic;



extern struct mqtt_topic codeRed_fridge_display_topic;
extern struct mqtt_topic codeRed_fridge_relay1_topic;
extern struct mqtt_topic codeRed_fridge_relay2_topic;

#ifdef __cplusplus
}
#endif
#endif