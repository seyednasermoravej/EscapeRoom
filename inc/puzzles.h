#ifndef __PUZZLES__H__
#define __PUZZLES__H__


// #ifdef __cplusplus
// extern "C" {
// #endif
#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/data/json.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
enum PuzzleTypes
{
    SERVO_DEVICE = 0,

};

#define HINT_NODE	DT_ALIAS(hintbutton)
#if !DT_NODE_HAS_STATUS(HINT_NODE, okay)
#error "Unsupported board: HINT devicetree alias is not defined"
#endif

enum MessageTypes
{
    RESET = 0,
    STATUS,
    ADD_SUB_TOP,
    ADD_PUB_TOP,
    REMOVE_SUB_TOP,
    REMOVE_PUB_TOP,
    HINT,
    RFID,
    ACTION,
    STATUS_REQUEST,
    BUILT_IN_LED,
};

// enum Servos
// {
//     SERVO0 = 0,
//     SERVO1,
//     SERVO2,
//     SERVO
// }

#define MAX_NUMBER_OF_SERVO_MOTORS          8
#define PUZZLE_STACK_SIZE                   2048
#define PUZZLE_PRIORITY                     8
#define STEP PWM_USEC(100)
struct MessageFromServer
{
    uint32_t timestamp;
    // uint32_t puzzleType;
    // int validPubTopic;
    // const char *pubTopic;
    // int validSubTopic;
    // const char *subTopic;
    // bool builtInLed;
    // bool validDoors;
    // bool doors[2];
    // bool validInLcd;
    size_t numOfServos;
    int servos[MAX_NUMBER_OF_SERVO_MOTORS];
};

struct MessageToServer
{
    int timestamp;
};

struct HintMessage
{
    int timestamp;
    int type;
    bool status;
};

class Puzzle
{
public:
    Puzzle();
    void mqttInMessageHandler(char *message);


private:
    PuzzleTypes puzzleType;


};

// #ifdef __cplusplus
// }
// #endif
#endif