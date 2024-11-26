#ifndef __PUZZLE__H__
#define __PUZZLE__H__



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


class Puzzle 
{
protected:
    const char *roomName;
    const char *puzzleTypeName;

public:
    // Constructor to initialize roomName and puzzleTypeName
    Puzzle(const char* room, const char* type);

    // Virtual method to be implemented by subclasses
    virtual void messageHandler(MqttMsg *msg) = 0;

    // Common method for all subclasses
    void alive();


    // Virtual destructor for proper cleanup of derived classes
    virtual ~Puzzle() {}
};

#endif