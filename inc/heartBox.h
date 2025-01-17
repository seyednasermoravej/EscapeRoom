#ifndef __HEART_BOX__H__
#define __HEART_BOX__H__


#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/device.h>
#include "topics.h"
#include <zephyr/input/input.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include "puzzle.h"
#include "ledStrip.h"
#include "display4ht16k33.h"
#include "display8ht16k33.h"
#include "main.h"

class HeartBox: public Puzzle 
{
public:
    HeartBox(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    void keypadHandler(struct input_event *val);
    static void keypadHandlerWrapper(struct input_event *val, void *userData);
    void puzzleSolver(char input);

private:
    LedStrip *ledStrip;
    Display4 *display4;
    Display8 *display8;
    char password[PUZZLE_DISPLAY_LEN + 1];
    bool passwordSet = false;
    
};

#endif 