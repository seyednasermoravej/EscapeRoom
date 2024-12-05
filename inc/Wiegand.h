#ifndef _WIEGAND_H
#define _WIEGAND_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

class WIEGAND {
public:
    WIEGAND(const struct device* gpio_dev_d0, int pin_d0, const struct device* gpio_dev_d1, int pin_d1);  // Constructor initializes pins using overlay
    bool available();
    unsigned long getCode();
    int getWiegandType();
    unsigned long getCardId(volatile unsigned long* codehigh, volatile unsigned long* codelow, char bitlength);

private:
    static void gpioCallbackD0(const struct device* dev, struct gpio_callback* cb, uint32_t pins);
    static void gpioCallbackD1(const struct device* dev, struct gpio_callback* cb, uint32_t pins);
    void processBit(int bit);
    char translateEnterEscapeKeyPress(char originalKeyPress);
    bool doWiegandConversion();

    const struct device* gpio_dev_d0;
    const struct device* gpio_dev_d1;
    struct gpio_callback gpio_cb_d0;
    struct gpio_callback gpio_cb_d1;

    int pin_d0;
    int pin_d1;
    volatile unsigned long _cardTempHigh;
    volatile unsigned long _cardTemp;
    volatile unsigned long _lastWiegand;
    unsigned long _code;
    volatile int _bitCount;
    int _wiegandType;

    struct k_mutex lock;
};

#endif /* _WIEGAND_H */