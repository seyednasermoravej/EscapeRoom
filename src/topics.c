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

struct mqtt_topic lcd_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)LCD_TOPIC,
            .size = strlen(LCD_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic lcd1_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)LCD1_TOPIC,
            .size = strlen(LCD1_TOPIC)
        },
    .qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic lcd2_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)LCD2_TOPIC,
            .size = strlen(LCD2_TOPIC)
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

struct mqtt_topic stepperPosition_topic = {
.topic =
{

    .utf8 = (uint8_t*)SET_STEPPER_POSITION,
    .size = strlen(SET_STEPPER_POSITION)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic stepperSpeed_topic = {
.topic =
{
    .utf8 = (uint8_t*)SET_STEPPER_TIME_POSITION,
    .size = strlen(SET_STEPPER_TIME_POSITION)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};


struct mqtt_topic stepperGoToStartPosition_topic = {
.topic =
{

    .utf8 = (uint8_t*)SET_STEPPER_GO_TO_START_POSITION,
    .size = strlen(SET_STEPPER_GO_TO_START_POSITION)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};

struct mqtt_topic stepperStop_topic = {
.topic =
{

    .utf8 = (uint8_t*)SET_STEPPER_STOP,
    .size = strlen(SET_STEPPER_STOP)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};


struct mqtt_topic relay0_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY0_TOPIC,
    .size = strlen(RELAY0_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic relay1_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY1_TOPIC,
    .size = strlen(RELAY1_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic relay2_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY2_TOPIC,
    .size = strlen(RELAY2_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic relay3_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY3_TOPIC,
    .size = strlen(RELAY3_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic relay4_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY4_TOPIC,
    .size = strlen(RELAY4_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic relay5_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY5_TOPIC,
    .size = strlen(RELAY5_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic relay6_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY6_TOPIC,
    .size = strlen(RELAY6_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};
struct mqtt_topic relay7_topic = {
.topic =
{

    .utf8 = (uint8_t*)RELAY7_TOPIC,
    .size = strlen(RELAY7_TOPIC)
},
.qos = MQTT_QOS_1_AT_LEAST_ONCE};