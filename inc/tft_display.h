#ifndef __DISPLAY_tft_480x320__H__
#define __DISPLAY_tft_480x320__H__

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <lvgl_input_device.h>
#include "zephyr/logging/log.h"

enum LangSelection
{
    NONE = 0,
    ENGLISH,
    DUTCH,
    FRENCH,
};

class Display_tft
{
public:
    const struct device *display_dev;
    Display_tft(const struct device *_display_dev);
    void displayStr(char *str);
    void displayClear();
    bool langSelect(char *);

private:
    
    lv_obj_t * background;
    lv_obj_t * screen_text;
    lv_obj_t * icon_Stars[6];
    int32_t icon_Stars_pos[6]={10,90,170,250,330,410};
    lv_obj_t * label_digits[6];
    int32_t label_digits_pos[6]={25,105,185,265,345,425};
    
    
};

#endif 