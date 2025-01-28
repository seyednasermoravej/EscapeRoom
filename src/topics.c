#include "topics.h"

struct mqtt_topic builtInLed_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)BUILT_IN_LED_TOPIC,
            .size = strlen(BUILT_IN_LED_TOPIC)
        },
    .qos = MQTT_QOS_2_EXACTLY_ONCE};


struct mqtt_topic puzzleType_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)PUZZLE_TYPE_TOPIC,
            .size = strlen(PUZZLE_TYPE_TOPIC)
        },
    .qos = MQTT_QOS_2_EXACTLY_ONCE};


struct mqtt_topic erase_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)ERASE_TOPIC,
            .size = strlen(ERASE_TOPIC)
        },
    .qos = MQTT_QOS_2_EXACTLY_ONCE};


////////////////////intro room console begin\\\\\\\\\\

struct mqtt_topic introRoom_console_lcd1_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)INTRO_ROOM_CONSOLE_LCD1_TOPIC,
            .size = strlen(INTRO_ROOM_CONSOLE_LCD1_TOPIC)
        },
    .qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic introRoom_console_lcd2_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)INTRO_ROOM_CONSOLE_LCD2_TOPIC,
            .size = strlen(INTRO_ROOM_CONSOLE_LCD2_TOPIC)
        },
    .qos = MQTT_QOS_2_EXACTLY_ONCE};
////////////////////intro room console end\\\\\\\\\\


//////////////////////code red dfib begin///////////
struct mqtt_topic codeRed_defib_display1_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_DEFIB_DISPLAY1_TOPIC,
        .size = strlen(CODE_RED_DEFIB_DISPLAY1_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
//////////////////////code red dfib end///////////
