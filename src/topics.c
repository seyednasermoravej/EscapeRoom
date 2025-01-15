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



////////////////////intro room cabinet begin\\\\\\\\\/

struct mqtt_topic introRoom_cabinet_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY1_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY1_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic introRoom_cabinet_relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY2_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY2_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic introRoom_cabinet_relay3_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY3_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY3_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic introRoom_cabinet_relay4_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY4_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY4_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

////////////////////intro room cabinet end\\\\\\\\\\
//////////////////////code red door begin///////////

struct mqtt_topic codeRed_door_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_DOOR_RELAY1_TOPIC,
    .size = strlen(CODE_RED_DOOR_RELAY1_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_door_relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_DOOR_RELAY2_TOPIC,
    .size = strlen(CODE_RED_DOOR_RELAY2_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
////////////////////code red door end////////////////

//////////////////////code red heart monitor begin///////////

struct mqtt_topic codeRed_heartMonitor_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_HEART_MONITOR_RELAY1_TOPIC,
    .size = strlen(CODE_RED_HEART_MONITOR_RELAY1_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
//////////////////////code red heart monitor end///////////
//////////////////////code red  ventilator///////////

struct mqtt_topic codeRed_ventilator_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY1_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY1_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_ventilator_relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY2_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY2_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_ventilator_relay3_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY3_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY3_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_ventilator_relay4_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY4_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY4_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
////////////////////code red ventilator end////////////////

////////////////////code red scale begin////////////////
struct mqtt_topic codeRed_scale_relay1_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_SCALE_RELAY1_TOPIC,
        .size = strlen(CODE_RED_SCALE_RELAY1_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
////////////////////code red scale end////////////////


//////////////////////code red entrance door begin///////////
struct mqtt_topic codeRed_entranceDoor_relay1_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_ENTRANCE_DOOR_RELAY1_TOPIC,
        .size = strlen(CODE_RED_ENTRANCE_DOOR_RELAY1_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
//////////////////////code red entrance door stop///////////

//////////////////////code red exit door begin///////////
struct mqtt_topic codeRed_exitDoor_relay1_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_EXIT_DOOR_RELAY1_TOPIC,
        .size = strlen(CODE_RED_EXIT_DOOR_RELAY1_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
//////////////////////code red exit door end///////////

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
