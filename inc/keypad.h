#ifndef __KEYPAD__H__
#define __KEYPAD__H__



#ifdef __cplusplus
extern "C" {
#endif


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>
#include "zephyr/logging/log.h"
#include "messageQueues.h"
// #define INPUT_DEV DT_NODELABEL(kbd_matrix)
void keypad();


#ifdef __cplusplus
}
#endif
#endif