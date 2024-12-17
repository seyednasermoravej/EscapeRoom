#include "topics.h"



struct mqtt_topic k3_topic = {
    .topic =
        {

            .utf8 = (uint8_t*)K3_TOPIC,
            .size = strlen(K3_TOPIC)
        },
    .qos = MQTT_QOS_2_EXACTLY_ONCE};


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


struct mqtt_topic devAddr_topic = {
.topic =
{

    .utf8 = (uint8_t*)DEV_ADDR_TOPIC,
    .size = strlen(DEV_ADDR_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led1_topic = {
.topic =
    {

        .utf8 = (uint8_t*)LED1_TOPIC,
        .size = strlen(LED1_TOPIC)
    },
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led2_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED2_TOPIC,
    .size = strlen(LED2_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led3_topic = {
.topic =
    {

        .utf8 = (uint8_t*)LED3_TOPIC,
        .size = strlen(LED3_TOPIC)
    },
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led4_topic = {
.topic =
    {

        .utf8 = (uint8_t*)LED4_TOPIC,
        .size = strlen(LED4_TOPIC)
    },
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led5_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED5_TOPIC,
    .size = strlen(LED5_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led6_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED6_TOPIC,
    .size = strlen(LED6_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led7_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED7_TOPIC,
    .size = strlen(LED7_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic led8_topic = {
.topic =
{

    .utf8 = (uint8_t*)LED8_TOPIC,
    .size = strlen(LED8_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg1_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG1_TOPIC,
    .size = strlen(SEG1_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg2_topic = {
.topic =
    {

        .utf8 = (uint8_t*)SEG2_TOPIC,
        .size = strlen(SEG2_TOPIC)
    },
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg3_topic = {
.topic =
    {

        .utf8 = (uint8_t*)SEG3_TOPIC,
        .size = strlen(SEG3_TOPIC)
    },
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg4_topic = {
.topic =
    {

        .utf8 = (uint8_t*)SEG4_TOPIC,
        .size = strlen(SEG4_TOPIC)
    },
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg5_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG5_TOPIC,
    .size = strlen(SEG5_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg6_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG6_TOPIC,
    .size = strlen(SEG6_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg7_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG7_TOPIC,
    .size = strlen(SEG7_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic seg8_topic = {
.topic =
{

    .utf8 = (uint8_t*)SEG8_TOPIC,
    .size = strlen(SEG8_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};


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

//////////////////////code red door keypad begin///////////

struct mqtt_topic codeRed_doorKeypad_display_topic = {
.topic =
{

    .utf8 = (uint8_t*)CODE_RED_DOOR_KEYPAD_DISPLAY_TOPIC,
    .size = strlen(CODE_RED_DOOR_KEYPAD_DISPLAY_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
////////////////////code red door keypad end////////////////
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

////////////////////code red power panel begin////////////////
struct mqtt_topic codeRed_powerPanel_servo1_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_SERVO1_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_SERVO1_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_servo2_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_SERVO2_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_SERVO2_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_servo3_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_SERVO3_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_SERVO3_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_servo4_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_SERVO4_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_SERVO4_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};

struct mqtt_topic codeRed_powerPanel_display1_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY1_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY1_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display2_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY2_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY2_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display3_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY3_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY3_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display4_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY4_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY4_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display5_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY5_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY5_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display6_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY6_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY6_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display7_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY7_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY7_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display8_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY8_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY8_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
struct mqtt_topic codeRed_powerPanel_display9_topic = {
.topic =
{
    .utf8 = (uint8_t*)CODE_RED_POWER_PANEL_DISPLAY9_TOPIC,
    .size = strlen(CODE_RED_POWER_PANEL_DISPLAY9_TOPIC)
},
.qos = MQTT_QOS_2_EXACTLY_ONCE};
////////////////////code red power panel end////////////////

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

////////////////////code red color tubes begin////////////////
struct mqtt_topic codeRed_colorTubes_colorPosition1_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION1_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION1_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition2_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION2_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION2_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition3_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION3_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION3_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition4_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION4_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION4_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition5_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION5_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION5_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition6_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION6_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION6_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition7_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION7_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION7_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition8_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION8_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION8_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition9_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION9_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION9_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition10_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION10_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION10_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition11_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION11_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION11_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition12_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION12_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION12_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition13_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION13_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION13_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition14_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION14_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION14_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_colorTubes_colorPosition15_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_COLOR_TUBES_COLOR_POSITION15_TOPIC,
        .size = strlen(CODE_RED_COLOR_TUBES_COLOR_POSITION15_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
////////////////////code red color tubes stop////////////////

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



/////////////////////////////////WiFi Pico////////////////////

//////////////////////code red heart box begin///////////
struct mqtt_topic codeRed_heartBox_display1_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_DISPLAY1_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_DISPLAY1_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_display2_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_DISPLAY2_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_DISPLAY2_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_relay1_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_RELAY1_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_RELAY1_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811a_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811A_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811A_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811b_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811B_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811B_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811c_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811C_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811C_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811d_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811D_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811D_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811e_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811E_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811E_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811f_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811F_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811F_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811g_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811G_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811G_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
struct mqtt_topic codeRed_heartBox_ws2811h_topic = {
    .topic =
    {
        .utf8 = (uint8_t*)CODE_RED_HEART_BOX_WS2811H_TOPIC,
        .size = strlen(CODE_RED_HEART_BOX_WS2811H_TOPIC)
    },
    .qos = MQTT_QOS_2_EXACTLY_ONCE
};
//////////////////////code red heart box end///////////

//////////////////////code red heart begin///////////
//////////////////////code red heart end///////////