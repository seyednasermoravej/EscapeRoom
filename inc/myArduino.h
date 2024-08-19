#ifndef __MY_ARDUINO_H_
#define __MY_ARDUINO_H_

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/spi.h>

#define byte uint8_t

#ifdef __cplusplus
extern "C" {
#endif


#define delay(x)                    k_msleep(x)
#define delayMicroseconds(x)        k_usleep(x)
#define INPUT                       GPIO_INPUT
#define OUTPUT                      GPIO_OUTPUT
#define pinMode(x, y)               gpio_pin_configure_dt(x, y)
#define digitalWrite(x, y)          gpio_pin_set_dt(x, y)
#define digitalRead(x)              gpio_pin_get_dt(x)

#define LOW                         0 
#define HIGH                        1 

// #define i2c_dev->write(buf, n)      i2c_write_dt(i2c_dev, buf, n)

// #define ser_dev->readBytes(buf, n)  uart_rx_enable(ser_dev, buf, n, 0x100)
// #define ser_dev->write(buf, n)      uart_tx(ser_dev, buf, n, 0x100)





#ifdef __cplusplus
}
#endif

#endif