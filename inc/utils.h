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
#include <zephyr/drivers/pwm.h>

#ifdef __cplusplus
extern "C" {
#endif




#define GPIO_NAME			"GPIO_"

#define GPIO_PIN_CFG(dev, pin, dir)						\
	do {									\
		if (gpio_pin_configure((dev), (pin), (dir))) {			\
			LOG_ERR("Err cfg " GPIO_NAME "%d! %x\n", (pin), (dir));	\
		}								\
	} while (0)




#define GPIO_PIN_WR(dev, pin, bit)						\
	do {									\
		if (gpio_pin_set_raw((dev), (pin), (bit))) {			\
			LOG_ERR("Err set " GPIO_NAME "%d! %x\n", (pin), (bit));	\
		}								\
	} while (0)								\



void hex2stringMac(const uint8_t *hex, int length, char *buf);

void gpioInit(const struct gpio_dt_spec *gpio, char *message);

void pwmInit(const struct pwm_dt_spec *pwm, char *message);

#define HIGH    1
#define LOW     0

#ifdef __cplusplus
}
#endif
#endif