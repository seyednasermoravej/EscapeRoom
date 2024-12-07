#include "FourBitLedDigitalTube.h"
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <string.h>

// Static variables
uint8_t TM74HC595LedTube::_displaysCount = 0;
uint8_t TM74HC595LedTube::_dspPtrArrLngth = 10;
TM74HC595LedTube** TM74HC595LedTube::_instancesLstPtr = nullptr;
bool TM74HC595LedTube::_intRfrshSrvcStrtd = false;

// Timer for refreshing displays
static struct k_timer refresh_timer;

// Timer callback for refreshing displays
void refresh_callback(struct k_timer *timer_id) {
    for (uint8_t i = 0; i < TM74HC595LedTube::_dspPtrArrLngth; i++) {
        if (TM74HC595LedTube::_instancesLstPtr[i] != nullptr) {
            TM74HC595LedTube::_instancesLstPtr[i]->fastRefresh();
        }
    }
}

TM74HC595LedTube::TM74HC595LedTube(const struct device *sclk_dev, gpio_pin_t sclk_pin,
                                   const struct device *rclk_dev, gpio_pin_t rclk_pin,
                                   const struct device *dio_dev, gpio_pin_t dio_pin,
                                   bool commAnode, uint8_t dspDigits)
    : sclk_dev(sclk_dev), rclk_dev(rclk_dev), dio_dev(dio_dev),
      sclk_pin(sclk_pin), rclk_pin(rclk_pin), dio_pin(dio_pin),
      comm_anode(commAnode), dsp_digits(dspDigits) {
    // Configure GPIO pins
    gpio_pin_configure(sclk_dev, sclk_pin, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure(rclk_dev, rclk_pin, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure(dio_dev, dio_pin, GPIO_OUTPUT_ACTIVE);

    // Initialize digit buffers
    digit_pos_ptr = new uint8_t[dspDigits];
    digit_ptr = new uint8_t[dspDigits];
    blink_mask_ptr = new bool[dspDigits];

    set_attributes();
    clear();
}

// Destructor
TM74HC595LedTube::~TM74HC595LedTube() {
    stop();
    delete[] digit_ptr;
    delete[] digit_pos_ptr;
    delete[] blink_mask_ptr;
}

// Set attributes
void TM74HC595LedTube::set_attributes() {
    // Calculate the minimum and maximum values displayable with the available digits
    if (dsp_digits > 1) {
        _dspValMin = 1;
        for (uint8_t i = 0; i < (dsp_digits - 1); i++) {
            _dspValMin *= 10;
        }
        _dspValMin = (-1) * (_dspValMin - 1);
    } else {
        _dspValMin = 0;
    }

    _dspValMax = 1;
    for (uint8_t i = 0; i < dsp_digits; i++) {
        _dspValMax *= 10;
        _zeroPadding += "0";
        _spacePadding += " ";
        digit_pos_ptr[i] = i;  // Initialize digit positions
        blink_mask_ptr[i] = true;  // Initialize blink mask
    }
    _dspValMax -= 1;

    // Adjust character set for common cathode or anode displays
    if (!comm_anode) {
        _waitChar = ~_waitChar;
        _space = ~_space;
        _dot = ~_dot;
        for (int i = 0; i < 45; i++) {
            char_leds[i] = ~char_leds[i];
        }
    }
}

// Start the refresh timer
bool TM74HC595LedTube::begin() {
    if (!_intRfrshSrvcStrtd) {
        k_timer_init(&refresh_timer, refresh_callback, nullptr);
        k_timer_start(&refresh_timer, K_MSEC(2), K_MSEC(2));
        _intRfrshSrvcStrtd = true;
    }

    if (_instancesLstPtr == nullptr) {
        _instancesLstPtr = new TM74HC595LedTube*[_dspPtrArrLngth];
        memset(_instancesLstPtr, 0, _dspPtrArrLngth * sizeof(TM74HC595LedTube*));
    }

    for (uint8_t i = 0; i < _dspPtrArrLngth; i++) {
        if (_instancesLstPtr[i] == nullptr) {
            _instancesLstPtr[i] = this;
            return true;
        }
    }

    return false;
}

// Stop the refresh timer
bool TM74HC595LedTube::stop() {
    for (uint8_t i = 0; i < _dspPtrArrLngth; i++) {
        if (_instancesLstPtr[i] == this) {
            _instancesLstPtr[i] = nullptr;
            break;
        }
    }

    for (uint8_t i = 0; i < _dspPtrArrLngth; i++) {
        if (_instancesLstPtr[i] != nullptr) {
            return true;
        }
    }

    k_timer_stop(&refresh_timer);
    delete[] _instancesLstPtr;
    _instancesLstPtr = nullptr;
    _intRfrshSrvcStrtd = false;

    return true;
}

// Clear the display
void TM74HC595LedTube::clear() {
    for (uint8_t i = 0; i < dsp_digits; i++) {
        digit_ptr[i] = comm_anode ? 0xFF : 0x00;
    }
    fastRefresh();
}

// Fast refresh
void TM74HC595LedTube::fastRefresh() {
    for (uint8_t i = 0; i < dsp_digits; i++) {
        fastSend(digit_ptr[i], 1 << digit_pos_ptr[i]);
    }
}

// Send data to the display
void TM74HC595LedTube::fastSend(uint8_t content) {
    for (int i = 7; i >= 0; i--) {
        gpio_pin_set(dio_dev, dio_pin, (content & 0x80) ? 1 : 0);
        content <<= 1;
        gpio_pin_set(sclk_dev, sclk_pin, 0);
        gpio_pin_set(sclk_dev, sclk_pin, 1);
    }
}

// Send data to a specific digit
void TM74HC595LedTube::fastSend(const uint8_t &segments, const uint8_t &port) {
    gpio_pin_set(rclk_dev, rclk_pin, 0);
    fastSend(segments);
    fastSend(port);
    gpio_pin_set(rclk_dev, rclk_pin, 1);
}

// Print a string to the display
bool TM74HC595LedTube::print(const char *text) {
    size_t len = strlen(text);
    if (len > dsp_digits) {
        return false;  // Text too long for the display
    }

    // Temporary buffers for 7-segment data
    uint8_t temp7SegData[dsp_digits];
    uint8_t tempDpData[dsp_digits];

    // Initialize buffers with blank spaces
    for (uint8_t i = 0; i < dsp_digits; i++) {
        temp7SegData[i] = comm_anode ? 0xFF : 0x00;  // Blank character
        tempDpData[i] = comm_anode ? 0xFF : 0x00;   // No decimal point
    }

    // Process the input string and map characters to 7-segment data
    for (size_t i = 0; i < len; i++) {
        char c = text[len - i - 1];  // Reverse order for right alignment
        const char *pos = strchr(char_set, c);  // Find character in char_set
        if (pos) {
            temp7SegData[i] = char_leds[pos - char_set];
        } else {
            return false;  // Unsupported character
        }
    }

    // Update the digit buffer
    for (uint8_t i = 0; i < dsp_digits; i++) {
        digit_ptr[i] = temp7SegData[i] & tempDpData[i];
    }

    return true;
}