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