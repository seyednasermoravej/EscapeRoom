#include "Wiegand.h"
#include <zephyr/devicetree.h>
#include <zephyr/devicetree/gpio.h>
#include "myArduino.h"
// Macros to access pin definitions from overlay


WIEGAND::WIEGAND(const struct device* _gpio_dev_d0, int _pin_d0, const struct device* _gpio_dev_d1, int _pin_d1)
    : gpio_dev_d0(_gpio_dev_d0),
      pin_d0(_pin_d0),
      gpio_dev_d1(_gpio_dev_d1),
      pin_d1(_pin_d1),
      _cardTempHigh(0),
      _cardTemp(0),
      _lastWiegand(0),
      _code(0),
      _bitCount(0),
      _wiegandType(0) {



    if (!gpio_dev_d0 || !gpio_dev_d1) {
        printk("Error: GPIO devices not found.\n");
        return;
    }

    // Configure GPIO pins
    gpio_pin_configure(gpio_dev_d0, pin_d0, GPIO_INPUT | GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_configure(gpio_dev_d1, pin_d1, GPIO_INPUT | GPIO_INT_EDGE_TO_ACTIVE);

    gpio_init_callback(&gpio_cb_d0, gpioCallbackD0, BIT(pin_d0));
    gpio_init_callback(&gpio_cb_d1, gpioCallbackD1, BIT(pin_d1));

    gpio_add_callback(gpio_dev_d0, &gpio_cb_d0);
    gpio_add_callback(gpio_dev_d1, &gpio_cb_d1);

    gpio_pin_interrupt_configure(gpio_dev_d0, pin_d0, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure(gpio_dev_d1, pin_d1, GPIO_INT_EDGE_TO_ACTIVE);

    k_mutex_init(&lock);

    printk("Wiegand initialized on pins %d (D0) and %d (D1)\n", pin_d0, pin_d1);
}

bool WIEGAND::available() {
    k_mutex_lock(&lock, K_FOREVER);
    bool result = doWiegandConversion();
    k_mutex_unlock(&lock);
    return result;
}

unsigned long WIEGAND::getCode() {
    k_mutex_lock(&lock, K_FOREVER);
    unsigned long code = _code;
    _code = 0;  // Reset code after reading
    _bitCount = 0;
    k_mutex_unlock(&lock);
    return code;
}

int WIEGAND::getWiegandType() {
    return _wiegandType;
}

void WIEGAND::gpioCallbackD0(const struct device* dev, struct gpio_callback* cb, uint32_t pins) {
    WIEGAND* instance = CONTAINER_OF(cb, WIEGAND, gpio_cb_d0);
    instance->processBit(0);
}

void WIEGAND::gpioCallbackD1(const struct device* dev, struct gpio_callback* cb, uint32_t pins) {
    WIEGAND* instance = CONTAINER_OF(cb, WIEGAND, gpio_cb_d1);
    instance->processBit(1);
}

void WIEGAND::processBit(int bit) {
    k_mutex_lock(&lock, K_FOREVER);

    _bitCount++;
    if (_bitCount > 31) {
        _cardTempHigh |= ((_cardTemp & 0x80000000) >> 31);
        _cardTempHigh <<= 1;
        _cardTemp <<= 1;
    } else {
        _cardTemp <<= 1;
        if (bit) {
            _cardTemp |= 1;
        }
    }

    _lastWiegand = k_uptime_get();
    k_mutex_unlock(&lock);
}
unsigned long WIEGAND::getCardId(volatile unsigned long* codehigh, volatile unsigned long* codelow, char bitlength) {
    if (bitlength == 26) {
        return (*codelow & 0x1FFFFFE) >> 1;
    }
    if (bitlength == 34) {
        *codehigh = *codehigh & 0x03;
        *codehigh <<= 30;
        *codelow >>= 1;
        return *codehigh | *codelow;
    }
    return 0;  // Unsupported bit length
}
bool WIEGAND::doWiegandConversion() {
    unsigned long sysTick = k_uptime_get();

    if ((sysTick - _lastWiegand) > 25) {
        if (_bitCount == 26 || _bitCount == 34) {
            _cardTemp >>= 1;
            if (_bitCount > 32) {
                _cardTempHigh >>= 1;
            }
            _code = getCardId(&_cardTempHigh, &_cardTemp, _bitCount);
            _wiegandType = _bitCount;
            _bitCount = 0;
            _cardTemp = 0;
            _cardTempHigh = 0;
            return true;
        } else {
            _bitCount = 0;
            _cardTemp = 0;
            _cardTempHigh = 0;
        }
    }

    return false;
}

// extern "C" void wiegandFunc()
// {
//     WIEGAND wg;
//     while(1)
//     {
//     	if(wg.available())
//         {
//             printk("Wiegand HEX = ");
//             printk("%lx", wg.getCode());
//             printk(", DECIMAL = ");
//             printk("%lu", wg.getCode());
//             printk(", Type W");
//             printk("%d\n", wg.getWiegandType());    
//         }
//     k_msleep(1000);
//     }
// }