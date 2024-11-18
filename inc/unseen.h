#ifndef __UNSEEN__H__
#define __UNSEEN__H__



#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include "keypad.h"
#include "tftLcd.h"


class Unseen
{

public:
    Unseen();
    void messageHandler(struct MqttMsg *msg);

private:

    TftLcd *display;


};

#endif 