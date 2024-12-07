#ifndef FOUR_BIT_LED_DIGITAL_TUBE_H
#define FOUR_BIT_LED_DIGITAL_TUBE_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <string>
#include <stdint.h>

class TM74HC595LedTube {
public:
    // Static variables
    static uint8_t _displaysCount;
    static uint8_t _dspPtrArrLngth;
    static TM74HC595LedTube** _instancesLstPtr;
    static bool _intRfrshSrvcStrtd;

protected:
    // GPIO pins and devices
    const struct device *sclk_dev;
    const struct device *rclk_dev;
    const struct device *dio_dev;
    gpio_pin_t sclk_pin;
    gpio_pin_t rclk_pin;
    gpio_pin_t dio_pin;

    // Display configuration
    bool comm_anode;
    uint8_t dsp_digits;
    uint8_t *digit_pos_ptr;
    uint8_t *digit_ptr;
    bool *blink_mask_ptr;

    // Blink state
    bool blinking = false;
    bool blink_show_on = false;
    uint32_t blink_off_rate = 500;
    uint32_t blink_on_rate = 500;
    uint32_t blink_timer = 0;

    // Wait state
    bool waiting = false;
    uint32_t wait_rate = 500;
    uint32_t wait_timer = 0;
    uint8_t wait_count = 0;

    // Display attributes
    long _dspValMin;           // Minimum value displayable
    long _dspValMax;           // Maximum value displayable
    std::string _zeroPadding;  // Padding for zero alignment
    std::string _spacePadding; // Padding for space alignment
    uint8_t _waitChar;         // Character to display during wait
    uint8_t _space;            // Character for blank space
    uint8_t _dot;              // Character for dot/decimal point

    // Character set for the display
    const char *char_set = "0123456789AabCcdEeFGHhIiJLlnOoPqrStUuY-_=~* .";
    uint8_t char_leds[45] = {
        0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0xA0, 0x83, 0xC6, 0xA7, 0xA1,
        0x86, 0x84, 0x8E, 0xC2, 0x89, 0x8B, 0xF9, 0xFB, 0xF1, 0xC7, 0xCF, 0xAB, 0xC0, 0xA3, 0x8C, 0x98,
        0xAF, 0x92, 0x87, 0xC1, 0xE3, 0x91, 0xBF, 0xF7, 0xB7, 0xB6, 0x9C, 0xFF, 0x7F
    };

    // Helper methods
    void set_attributes();
    void fastSend(uint8_t content);
    void fastSend(const uint8_t &segments, const uint8_t &port);
    void updBlinkState();
    void updWaitState();

public:
    TM74HC595LedTube(const struct device *sclk_dev, gpio_pin_t sclk_pin,
                     const struct device *rclk_dev, gpio_pin_t rclk_pin,
                     const struct device *dio_dev, gpio_pin_t dio_pin,
                     bool commAnode = true, uint8_t dspDigits = 4);
    ~TM74HC595LedTube();

    bool begin();
    bool stop();
    void clear();
    void fastRefresh();
    bool print(const char *text);
    bool blink();
};

// ClickCounter Class
class ClickCounter {
private:
    long count = 0;
    bool count_right_align;
    bool count_zero_pad;
    TM74HC595LedTube display;

public:
    ClickCounter(const struct device *cc_sclk_dev, gpio_pin_t cc_sclk_pin,
                 const struct device *cc_rclk_dev, gpio_pin_t cc_rclk_pin,
                 const struct device *cc_dio_dev, gpio_pin_t cc_dio_pin,
                 bool right_align = true, bool zero_pad = false, bool comm_anode = true, uint8_t dsp_digits = 4);
    ~ClickCounter();

    bool blink();
    bool blink(const uint32_t &on_rate, const uint32_t &off_rate);
    void clear();
    bool countBegin(long start_val);
    bool countUp(long qty);
    bool countDown(long qty);
    bool updateDisplay();
};

#endif