#ifndef __FRIDGE__H__
#define __FRIDGE__H__



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
#include "keypad43.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>
#include "ledStrip.h"




#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <lvgl.h>
#include "lvgl_display.h"
#include "lvgl_common_input.h"
#include "lvgl_zephyr.h"







class Fridge: public Puzzle 
{
public:
    Fridge(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;

private:
    Keypad43 *keypad;
    LedStrip *ledStrip;
    const struct device *display_dev;


};

#endif 