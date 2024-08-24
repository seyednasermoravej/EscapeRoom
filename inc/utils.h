#ifndef __UTILS__H__
#define __UTILS__H__

#include <zephyr/logging/log.h>

#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>
#include <zephyr/drivers/gpio.h>

#include <string.h>
#include <errno.h>

#include <zephyr/sys/util.h>

#ifdef __cplusplus
extern "C" {
#endif

void hex2stringMac(const uint8_t *hex, int length, char *buf);

void gpioInit(const struct gpio_dt_spec *gpio, char *message);

#ifdef __cplusplus
}
#endif
#endif