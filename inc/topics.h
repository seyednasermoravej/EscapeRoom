#ifndef __TOPICS__H__
#define __TOPICS__H__


#include <zephyr/net/mqtt.h>


#define PUZZLE_TYPE_TOPIC       "puzzleType"
extern struct mqtt_topic puzzleType_topic;
#define BUILT_IN_LED_TOPIC      "builtInLed"
extern struct mqtt_topic builtInLed_topic;

#define ERASE_TOPIC      "erase"
extern struct mqtt_topic erase_topic;

#define INTRO_ROOM_CONSOLE_LCD1_TOPIC                                         "introRoom/console/lcd1"
extern struct mqtt_topic introRoom_console_lcd1_topic;
#define INTRO_ROOM_CONSOLE_LCD2_TOPIC                                         "introRoom/console/lcd2"
extern struct mqtt_topic introRoom_console_lcd2_topic;

#define INTRO_ROOM_CABINET_RELAY1_TOPIC                                       "introRoom/cabinet/relay1" 
extern struct mqtt_topic introRoom_cabinet_relay1_topic;
#define INTRO_ROOM_CABINET_RELAY2_TOPIC                                       "introRoom/cabinet/relay2" 
extern struct mqtt_topic introRoom_cabinet_relay2_topic;
#define INTRO_ROOM_CABINET_RELAY3_TOPIC                                       "introRoom/cabinet/relay3" 
extern struct mqtt_topic introRoom_cabinet_relay3_topic;
#define INTRO_ROOM_CABINET_RELAY4_TOPIC                                       "introRoom/cabinet/relay4" 
extern struct mqtt_topic introRoom_cabinet_relay4_topic;


#define CODE_RED_DOOR_RELAY1_TOPIC                                       "codeRed/door/relay1" 
extern struct mqtt_topic codeRed_door_relay1_topic;
#define CODE_RED_DOOR_RELAY2_TOPIC                                       "codeRed/door/relay2" 
extern struct mqtt_topic codeRed_door_relay2_topic;

#define CODE_RED_HEART_MONITOR_RELAY1_TOPIC                                       "codeRed/heartMonitor/relay1" 
extern struct mqtt_topic codeRed_heartMonitor_relay1_topic;


#define CODE_RED_VENTILATOR_RELAY1_TOPIC                                       "codeRed/ventilator/relay1" 
extern struct mqtt_topic codeRed_ventilator_relay1_topic;
#define CODE_RED_VENTILATOR_RELAY2_TOPIC                                       "codeRed/ventilator/relay2" 
extern struct mqtt_topic codeRed_ventilator_relay2_topic;
#define CODE_RED_VENTILATOR_RELAY3_TOPIC                                       "codeRed/ventilator/relay3" 
extern struct mqtt_topic codeRed_ventilator_relay3_topic;
#define CODE_RED_VENTILATOR_RELAY4_TOPIC                                       "codeRed/ventilator/relay4" 
extern struct mqtt_topic codeRed_ventilator_relay4_topic;


#define CODE_RED_SCALE_RELAY1_TOPIC                                             "codeRed/scale/relay1" 
extern struct mqtt_topic codeRed_scale_relay1_topic;

#define CODE_RED_ENTRANCE_DOOR_RELAY1_TOPIC                                      "codeRed/entranceDoor/relay1" 
extern struct mqtt_topic codeRed_entranceDoor_relay1_topic;

#define CODE_RED_EXIT_DOOR_RELAY1_TOPIC                                           "codeRed/exitDoor/relay1" 
extern struct mqtt_topic codeRed_exitDoor_relay1_topic;

#define CODE_RED_DEFIB_DISPLAY1_TOPIC                                            "codeRed/defib/display1" 
extern struct mqtt_topic codeRed_defib_display1_topic;

#ifdef __cplusplus
extern "C" {
#endif

extern char deviceIdPub[32];

#ifdef __cplusplus
}
#endif
#endif