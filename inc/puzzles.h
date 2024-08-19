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
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>


#include "gate.h"
#include "servos.h"
#include "configDevice.h"
#include "unseen.h"
#include "numbersGuessing.h"
#include "laboratory.h"

enum PuzzleTypes
{
    UNSPECIFIED = 0,
    SERVOS_PUZZLE,
    GATE_PUZZLE,
    CONFIG_DEVICE_PUZZLE,
    NUMBERS_GUESSING_PUZZLE,
    UNSEEN_PUZZLE,
    LABORATORY_PUZZLE,

};


#define BUILT_IN_NODE	DT_ALIAS(built_in_led)
#if !DT_NODE_HAS_STATUS(BUILT_IN_NODE, okay)
#error "Unsupported board: BUILT_IN devicetree alias is not defined"
#endif

// enum Servos
// {
//     SERVO0 = 0,
//     SERVO1,
//     SERVO2,
//     SERVO
// }

#define PUZZLE_STACK_SIZE                   2048
#define PUZZLE_PRIORITY                     8


class Puzzle
{
public:
    Puzzle();
    void messageHandler(struct MqttMsg *msg);
    bool deviceSpecified = false;
    PuzzleTypes puzzleType = UNSPECIFIED;

private:

    Gate *gate;
    Servos *servos;
    ConfigDevice *configDevice;
    Unseen *unseen;
    NumbersGuessing *numbersGuessing;
    Laboratory *laboratory;

    int builtIntLedInit();


    void puzzleTypeSelection(char *type);

};

// #ifdef __cplusplus
// }
// #endif
#endif