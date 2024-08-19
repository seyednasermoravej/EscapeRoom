#ifndef __KEYPAD__H__
#define __KEYPAD__H__

#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input_kbd_matrix.h>
#include <zephyr/input/input_keymap.h>
#include "messageQueues.h"
#include <zephyr/kernel.h>

class Keypad
{
public:
    Keypad(const struct device *_devGpio, struct k_msgq *_queue);


private:
    struct device *devGpio; 
    struct k_msgq *queue;
    struct input_kbd_matrix_common_config *cfg;
    void gpio_kbd_scan_set_row(uint8_t row);
    void input_kbd_matrix_drive_column_hook(int col);
};



#endif