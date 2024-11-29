#include "topics.h"



struct mqtt_topic servo_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)SERVO0_TOPIC,
            .size = strlen(SERVO0_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic k3_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)K3_TOPIC,
            .size = strlen(K3_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};


struct mqtt_topic builtInLed_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)BUILT_IN_LED_TOPIC,
            .size = strlen(BUILT_IN_LED_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};


struct mqtt_topic puzzleType_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)PUZZLE_TYPE_TOPIC,
            .size = strlen(PUZZLE_TYPE_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};




////////////////////intro room console begin\\\\\\\\\\

struct mqtt_topic introRoom_console_lcd1_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)INTRO_ROOM_CONSOLE_LCD1_TOPIC,
            .size = strlen(INTRO_ROOM_CONSOLE_LCD1_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic introRoom_console_lcd2_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)INTRO_ROOM_CONSOLE_LCD2_TOPIC,
            .size = strlen(INTRO_ROOM_CONSOLE_LCD2_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};
////////////////////intro room console end\\\\\\\\\\


struct mqtt_topic devAddr_topic = {
.topic =
{

    .utf8 = (uint8_t*)DEV_ADDR_TOPIC,
    .size = strlen(DEV_ADDR_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led1_topic = {
.topic =
    {

        .utf8 = (uint8_t*)LED1_TOPIC,
        .size = strlen(LED1_TOPIC)
    },
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led2_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED2_TOPIC,
    .size = strlen(LED2_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led3_topic = {
.topic =
    {

        .utf8 = (uint8_t*)LED3_TOPIC,
        .size = strlen(LED3_TOPIC)
    },
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led4_topic = {
.topic =
    {

        .utf8 = (uint8_t*)LED4_TOPIC,
        .size = strlen(LED4_TOPIC)
    },
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led5_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED5_TOPIC,
    .size = strlen(LED5_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led6_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED6_TOPIC,
    .size = strlen(LED6_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led7_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED7_TOPIC,
    .size = strlen(LED7_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic led8_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED8_TOPIC,
    .size = strlen(LED8_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg1_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG1_TOPIC,
    .size = strlen(SEG1_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg2_topic = {
.topic =
    {

        .utf8 = (uint8_t*)SEG2_TOPIC,
        .size = strlen(SEG2_TOPIC)
    },
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg3_topic = {
.topic =
    {

        .utf8 = (uint8_t*)SEG3_TOPIC,
        .size = strlen(SEG3_TOPIC)
    },
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg4_topic = {
.topic =
    {

        .utf8 = (uint8_t*)SEG4_TOPIC,
        .size = strlen(SEG4_TOPIC)
    },
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg5_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG5_TOPIC,
    .size = strlen(SEG5_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg6_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG6_TOPIC,
    .size = strlen(SEG6_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg7_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG7_TOPIC,
    .size = strlen(SEG7_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic seg8_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG8_TOPIC,
    .size = strlen(SEG8_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

////////////////////intro room platfrom begin\\\\\\\\\\

struct mqtt_topic introRoom_platform_position_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_PLATFORM_POSITION_TOPIC,
    .size = strlen(INTRO_ROOM_PLATFORM_POSITION_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

////////////////////intro room platfrom end\\\\\\\\\/

////////////////////intro room cabinet begin\\\\\\\\\/

struct mqtt_topic introRoom_cabinet_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY1_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY1_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic introRoom_cabinet_relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY2_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY2_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic introRoom_cabinet_relay3_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY3_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY3_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic introRoom_cabinet_relay4_topic = {
.topic =
{

    .utf8 = (uint8_t*)INTRO_ROOM_CABINET_RELAY4_TOPIC,
    .size = strlen(INTRO_ROOM_CABINET_RELAY4_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

////////////////////intro room cabinet end\\\\\\\\\\
//////////////////////code red door begin///////////

struct mqtt_topic codeRed_door_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_DOOR_RELAY1_TOPIC,
    .size = strlen(CODE_RED_DOOR_RELAY1_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic codeRed_door_relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_DOOR_RELAY2_TOPIC,
    .size = strlen(CODE_RED_DOOR_RELAY2_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
////////////////////code red door end////////////////

//////////////////////code red door keypad begin///////////

struct mqtt_topic codeRed_doorKeypad_display_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_DOOR_KEYPAD_DISPLAY_TOPIC,
    .size = strlen(CODE_RED_DOOR_KEYPAD_DISPLAY_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
////////////////////code red door keypad end////////////////
//////////////////////code red door ventilator///////////

struct mqtt_topic codeRed_ventilator_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY1_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY1_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic codeRed_ventilator_relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY2_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY2_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic codeRed_ventilator_relay3_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY3_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY3_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic codeRed_ventilator_relay4_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_VENTILATOR_RELAY4_TOPIC,
    .size = strlen(CODE_RED_VENTILATOR_RELAY4_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
////////////////////code red ventilator end////////////////
////////////////////code red fridge begin////////////////
struct mqtt_topic codeRed_fridge_display_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_FRIDGE_DISPLAY_TOPIC,
    .size = strlen(CODE_RED_FRIDGE_DISPLAY_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic codeRed_fridge_relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_FRIDGE_RELAY1_TOPIC,
    .size = strlen(CODE_RED_FRIDGE_RELAY1_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic codeRed_fridge_relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_FRIDGE_RELAY2_TOPIC,
    .size = strlen(CODE_RED_FRIDGE_RELAY2_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
////////////////////code red fridge end////////////////


