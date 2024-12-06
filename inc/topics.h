#ifndef __TOPICS__H__
#define __TOPICS__H__


#include <zephyr/net/mqtt.h>


#define PUZZLE_TYPE_TOPIC       "puzzleType"
extern struct mqtt_topic puzzleType_topic;
#define BUILT_IN_LED_TOPIC      "sub/builtInLed"
extern struct mqtt_topic builtInLed_topic;


#define INTRO_ROOM_CONSOLE_LCD1_TOPIC                                         "introRoom/console/lcd1"
extern struct mqtt_topic introRoom_console_lcd1_topic;
#define INTRO_ROOM_CONSOLE_LCD2_TOPIC                                         "introRoom/console/lcd2"
extern struct mqtt_topic introRoom_console_lcd2_topic;

#define INTRO_ROOM_PLATFORM_POSITION_TOPIC                                    "introRoom/platform/position"
extern struct mqtt_topic introRoom_platform_position_topic;


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


#define CODE_RED_DOOR_KEYPAD_DISPLAY_TOPIC                                       "codeRed/doorKeypad/display" 
extern struct mqtt_topic codeRed_doorKeypad_display_topic;


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



#define CODE_RED_FRIDGE_DISPLAY_TOPIC                                       "codeRed/fridge/display" 
extern struct mqtt_topic codeRed_fridge_display_topic;
#define CODE_RED_FRIDGE_WS2811A_TOPIC                                       "codeRed/fridge/ws2811a" 
extern struct mqtt_topic codeRed_fridge_ws2811a_topic;
#define CODE_RED_FRIDGE_WS2811B_TOPIC                                       "codeRed/fridge/ws2811b" 
extern struct mqtt_topic codeRed_fridge_ws2811b_topic;
#define CODE_RED_FRIDGE_WS2811C_TOPIC                                       "codeRed/fridge/ws2811c" 
extern struct mqtt_topic codeRed_fridge_ws2811c_topic;
#define CODE_RED_FRIDGE_WS2811D_TOPIC                                       "codeRed/fridge/ws2811d" 
extern struct mqtt_topic codeRed_fridge_ws2811d_topic;
#define CODE_RED_FRIDGE_WS2811E_TOPIC                                       "codeRed/fridge/ws2811e" 
extern struct mqtt_topic codeRed_fridge_ws2811e_topic;
#define CODE_RED_FRIDGE_WS2811F_TOPIC                                       "codeRed/fridge/ws2811f" 
extern struct mqtt_topic codeRed_fridge_ws2811f_topic;
#define CODE_RED_FRIDGE_WS2811G_TOPIC                                       "codeRed/fridge/ws2811g" 
extern struct mqtt_topic codeRed_fridge_ws2811g_topic;
#define CODE_RED_FRIDGE_WS2811H_TOPIC                                       "codeRed/fridge/ws2811h" 
extern struct mqtt_topic codeRed_fridge_ws2811h_topic;
#define CODE_RED_FRIDGE_RELAY1_TOPIC                                       "codeRed/fridge/relay1" 
extern struct mqtt_topic codeRed_fridge_relay1_topic;
#define CODE_RED_FRIDGE_RELAY2_TOPIC                                       "codeRed/fridge/relay2" 
extern struct mqtt_topic codeRed_fridge_relay2_topic;



#define CODE_RED_BLINDS_SERVO1_TOPIC                                       "codeRed/blinds/servo1" 
extern struct mqtt_topic codeRed_blinds_servo1_topic;
#define CODE_RED_BLINDS_SERVO2_TOPIC                                       "codeRed/blinds/servo2" 
extern struct mqtt_topic codeRed_blinds_servo2_topic;
#define CODE_RED_BLINDS_SERVO3_TOPIC                                       "codeRed/blinds/servo3" 
extern struct mqtt_topic codeRed_blinds_servo3_topic;
#define CODE_RED_BLINDS_SERVO4_TOPIC                                       "codeRed/blinds/servo4" 
extern struct mqtt_topic codeRed_blinds_servo4_topic;


#define CODE_RED_POWER_PANEL_SERVO1_TOPIC                                       "codeRed/powerPanel/servo1" 
extern struct mqtt_topic codeRed_powerPanel_servo1_topic;
#define CODE_RED_POWER_PANEL_SERVO2_TOPIC                                       "codeRed/powerPanel/servo2" 
extern struct mqtt_topic codeRed_powerPanel_servo2_topic;
#define CODE_RED_POWER_PANEL_SERVO3_TOPIC                                       "codeRed/powerPanel/servo3" 
extern struct mqtt_topic codeRed_powerPanel_servo3_topic;
#define CODE_RED_POWER_PANEL_SERVO4_TOPIC                                       "codeRed/powerPanel/servo4" 
extern struct mqtt_topic codeRed_powerPanel_servo4_topic;
#define CODE_RED_POWER_PANEL_SERVO5_TOPIC                                       "codeRed/powerPanel/servo5" 
extern struct mqtt_topic codeRed_powerPanel_servo5_topic;
#define CODE_RED_POWER_PANEL_SERVO6_TOPIC                                       "codeRed/powerPanel/servo6" 
extern struct mqtt_topic codeRed_powerPanel_servo6_topic;
#define CODE_RED_POWER_PANEL_SERVO7_TOPIC                                       "codeRed/powerPanel/servo7" 
extern struct mqtt_topic codeRed_powerPanel_servo7_topic;
#define CODE_RED_POWER_PANEL_SERVO8_TOPIC                                       "codeRed/powerPanel/servo8" 
extern struct mqtt_topic codeRed_powerPanel_servo8_topic;
#define CODE_RED_POWER_PANEL_DISPLAY1_TOPIC                                       "codeRed/powerPanel/display1" 
extern struct mqtt_topic codeRed_powerPanel_display1_topic;
#define CODE_RED_POWER_PANEL_DISPLAY2_TOPIC                                       "codeRed/powerPanel/display2" 
extern struct mqtt_topic codeRed_powerPanel_display2_topic;
#define CODE_RED_POWER_PANEL_DISPLAY3_TOPIC                                       "codeRed/powerPanel/display3" 
extern struct mqtt_topic codeRed_powerPanel_display3_topic;
#define CODE_RED_POWER_PANEL_DISPLAY4_TOPIC                                       "codeRed/powerPanel/display4" 
extern struct mqtt_topic codeRed_powerPanel_display4_topic;
#define CODE_RED_POWER_PANEL_DISPLAY5_TOPIC                                       "codeRed/powerPanel/display5" 
extern struct mqtt_topic codeRed_powerPanel_display5_topic;
#define CODE_RED_POWER_PANEL_DISPLAY6_TOPIC                                       "codeRed/powerPanel/display6" 
extern struct mqtt_topic codeRed_powerPanel_display6_topic;
#define CODE_RED_POWER_PANEL_DISPLAY7_TOPIC                                       "codeRed/powerPanel/display7" 
extern struct mqtt_topic codeRed_powerPanel_display7_topic;
#define CODE_RED_POWER_PANEL_DISPLAY8_TOPIC                                       "codeRed/powerPanel/display8" 
extern struct mqtt_topic codeRed_powerPanel_display8_topic;
#define CODE_RED_POWER_PANEL_DISPLAY9_TOPIC                                       "codeRed/powerPanel/display9" 
extern struct mqtt_topic codeRed_powerPanel_display9_topic;

#define CODE_RED_SCALE_RELAY1_TOPIC                                             "codeRed/scale/relay1" 
extern struct mqtt_topic codeRed_scale_relay1_topic;

#define CODE_RED_COLOR_TUBES_COLOR_POSITION1_TOPIC                              "codeRed/colorTubes/colorPosition1" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition1_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION2_TOPIC                              "codeRed/colorTubes/colorPosition2" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition2_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION3_TOPIC                              "codeRed/colorTubes/colorPosition3" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition3_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION4_TOPIC                              "codeRed/colorTubes/colorPosition4" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition4_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION5_TOPIC                              "codeRed/colorTubes/colorPosition5" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition5_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION6_TOPIC                              "codeRed/colorTubes/colorPosition6" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition6_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION7_TOPIC                              "codeRed/colorTubes/colorPosition7" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition7_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION8_TOPIC                              "codeRed/colorTubes/colorPosition8" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition8_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION9_TOPIC                              "codeRed/colorTubes/colorPosition9" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition9_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION10_TOPIC                             "codeRed/colorTubes/colorPosition10" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition10_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION11_TOPIC                             "codeRed/colorTubes/colorPosition11" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition11_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION12_TOPIC                             "codeRed/colorTubes/colorPosition12" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition12_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION13_TOPIC                             "codeRed/colorTubes/colorPosition13" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition13_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION14_TOPIC                             "codeRed/colorTubes/colorPosition14" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition14_topic;
#define CODE_RED_COLOR_TUBES_COLOR_POSITION15_TOPIC                             "codeRed/colorTubes/colorPosition15" 
extern struct mqtt_topic codeRed_colorTubes_colorPosition15_topic;

#define CODE_RED_ENTRANCE_DOOR_RELAY1_TOPIC                                      "codeRed/entranceDoor/relay1" 
extern struct mqtt_topic codeRed_entranceDoor_relay1_topic;

#define CODE_RED_EXIT_DOOR_RELAY1_TOPIC                                           "codeRed/exitDoor/relay1" 
extern struct mqtt_topic codeRed_exitDoor_relay1_topic;

#define CODE_RED_PNEUMA_POST_RELAY1_TOPIC                                       "codeRed/pneumaPost/relay1" 
extern struct mqtt_topic codeRed_pneumaPost_relay1_topic;
#define CODE_RED_PNEUMA_POST_RELAY2_TOPIC                                       "codeRed/pneumaPost/relay2" 
extern struct mqtt_topic codeRed_pneumaPost_relay2_topic;

#define CODE_RED_DEFIB_DISPLAY1_TOPIC                                            "codeRed/defib/display1" 
extern struct mqtt_topic codeRed_defib_display1_topic;

#define CODE_RED_DRAWERS_RELAY1_TOPIC                                            "codeRed/drawers/relay1" 
extern struct mqtt_topic codeRed_drawers_relay1_topic;
#define CODE_RED_DRAWERS_RELAY2_TOPIC                                            "codeRed/drawers/relay2" 
extern struct mqtt_topic codeRed_drawers_relay2_topic;
#define CODE_RED_DRAWERS_RELAY3_TOPIC                                            "codeRed/drawers/relay3" 
extern struct mqtt_topic codeRed_drawers_relay3_topic;
#define CODE_RED_DRAWERS_RELAY4_TOPIC                                            "codeRed/drawers/relay4" 
extern struct mqtt_topic codeRed_drawers_relay4_topic;
#define CODE_RED_DRAWERS_RELAY5_TOPIC                                            "codeRed/drawers/relay5" 
extern struct mqtt_topic codeRed_drawers_relay5_topic;
#define CODE_RED_DRAWERS_RELAY6_TOPIC                                            "codeRed/drawers/relay6" 
extern struct mqtt_topic codeRed_drawers_relay6_topic;
#define CODE_RED_DRAWERS_RELAY7_TOPIC                                            "codeRed/drawers/relay7" 
extern struct mqtt_topic codeRed_drawers_relay7_topic;
#define CODE_RED_DRAWERS_RELAY8_TOPIC                                            "codeRed/drawers/relay8" 
extern struct mqtt_topic codeRed_drawers_relay8_topic;


#define CODE_RED_HEART_BOX_DISPLAY1_TOPIC                                          "codeRed/heartBox/display1" 
extern struct mqtt_topic codeRed_heartBox_display1_topic;
#define CODE_RED_HEART_BOX_DISPLAY2_TOPIC                                          "codeRed/heartBox/display2" 
extern struct mqtt_topic codeRed_heartBox_display2_topic;
#define CODE_RED_HEART_BOX_RELAY1_TOPIC                                           "codeRed/heartBox/relay1" 
extern struct mqtt_topic codeRed_heartBox_relay1_topic;
#define CODE_RED_HEART_BOX_WS2811A_TOPIC                                          "codeRed/heartBox/ws2811a" 
extern struct mqtt_topic codeRed_heartBox_ws2811a_topic;
#define CODE_RED_HEART_BOX_WS2811B_TOPIC                                          "codeRed/heartBox/ws2811b" 
extern struct mqtt_topic codeRed_heartBox_ws2811b_topic;
#define CODE_RED_HEART_BOX_WS2811C_TOPIC                                          "codeRed/heartBox/ws2811c" 
extern struct mqtt_topic codeRed_heartBox_ws2811c_topic;
#define CODE_RED_HEART_BOX_WS2811D_TOPIC                                          "codeRed/heartBox/ws2811d" 
extern struct mqtt_topic codeRed_heartBox_ws2811d_topic;
#define CODE_RED_HEART_BOX_WS2811E_TOPIC                                          "codeRed/heartBox/ws2811e" 
extern struct mqtt_topic codeRed_heartBox_ws2811e_topic;
#define CODE_RED_HEART_BOX_WS2811F_TOPIC                                          "codeRed/heartBox/ws2811f" 
extern struct mqtt_topic codeRed_heartBox_ws2811f_topic;
#define CODE_RED_HEART_BOX_WS2811G_TOPIC                                          "codeRed/heartBox/ws2811g" 
extern struct mqtt_topic codeRed_heartBox_ws2811g_topic;
#define CODE_RED_HEART_BOX_WS2811H_TOPIC                                          "codeRed/heartBox/ws2811h" 
extern struct mqtt_topic codeRed_heartBox_ws2811h_topic;

#define CODE_RED_HEART_SERVO_TOPIC                                             "codeRed/heart/servo" 
extern struct mqtt_topic codeRed_heart_servo_topic;


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

#define K3_TOPIC                "sub/relaysControl"

#ifdef __cplusplus
extern "C" {
#endif

extern char deviceIdPub[32];







extern struct mqtt_topic led1_topic;
extern struct mqtt_topic led2_topic;
extern struct mqtt_topic led3_topic;
extern struct mqtt_topic led4_topic;
extern struct mqtt_topic led5_topic;
extern struct mqtt_topic led6_topic;
extern struct mqtt_topic led7_topic;
extern struct mqtt_topic led8_topic;

extern struct mqtt_topic k3_topic;
#ifdef __cplusplus
}
#endif
#endif