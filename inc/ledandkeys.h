#ifndef __LED_AND_KEYS__H__
#define __LED_AND_KEYS__H__

#include <zephyr/kernel.h>
#include "myArduino.h"
#include <zephyr/logging/log.h>
#include "util.h"
#include "topics.h"
#include "messageQueues.h"


#define INSTRUCTION_WRITE_DATA 0x40
#define INSTRUCTION_READ_KEY 0x42
#define INSTRUCTION_ADDRESS_AUTO 0x40
#define INSTRUCTION_ADDRESS_FIXED 0x44
#define INSTRUCTION_NORMAL_MODE 0x40
#define INSTRUCTION_TEST_MODE 0x48

#define FIRST_DISPLAY_ADDRESS 0xC0

#define DISPLAY_TURN_OFF 0x80
#define DISPLAY_TURN_ON 0x88


#define LED_AND_KEYS_STACK_SIZE        2048
#define LED_AND_KEYS_PRIORITY            8

    enum BitOrder{
     LSBFIRST,
     MSBFIRST
    };

    enum pulse_t{
      PULSE1_16,
      PULSE2_16,
      PULSE4_16,
      PULSE10_16,
      PULSE11_16,
      PULSE12_16,
      PULSE13_16,
      PULSE14_16
    };

    enum button_t{
      S1,S2,S3,S4,
      S5,S6,S7,S8
    };

class ledandkeys
{
public:
    ledandkeys(struct k_msgq *_queue);
    void ledandkeysinit();
    void messageHandler(struct MqttMsg *msg);


    /**
    * fn getButton
    * param s num of button (S1-S8)
    * return state of button
    */
    bool getButton(button_t s);
    /**
    * fn getButtons
    * return state of 8 buttons
    */
    uint8_t getButtons();

    /**
    * fn writeLed
    * brief put led ON or OFF
    * param num num of led(1-8)
    * param state (true or false)
    */
    void writeLed(uint8_t num, bool state);

    /**
    * fn writeLeds
    * brief set all 8 leds ON or OFF
    * param val 8bits
    */
    void writeLeds(uint8_t val);

    /**
    * fn displayVal
    * brief put value on 7 segment display
    * param digitId num of digit(0-7)
    * param val value(0->F)
    */
    void displayVal(uint8_t digitId, uint8_t val);
        
    
    /**
    * fn displayDig
    * brief set 7 segment display + dot
    * param digitId num of digit(0-7)
    * param val value 8 bits
    */
    void displayDig(uint8_t digitId, uint8_t pgfedcba);

    /**
    * fn displayClear
    * brief switch off all leds and segment display
    */
    void displayClear();

    /**
    * fn displayTurnOff
    * brief turn on lights
    */
    void displayTurnOff();

    /**
    * fn displayTurnOn
    * brief turn off lights
    */
    void displayTurnOn();

    /**
    * fn displaySetBrightness
    * brief set display brightness
    * param pulse_t (0-7)
    */
    void displaySetBrightness(pulse_t pulse);

    /**
    * fn reset
    * brief switch off all displays-leds
    */
    void reset();

    /**
    * fn test
    * brief blink all displays and leds
    */
    void test();

    uint8_t bit_read(uint32_t value, uint8_t bit);
    uint8_t shiftIn(uint8_t bitOrder);
    void shiftOut(uint8_t bitOrder, uint8_t val);
    uint8_t check_zero_one(char val_char);
    void increase_segments(uint8_t button_val);
    void send_7seg_val(uint8_t segment_val, char pub_topic[30]);

    const struct gpio_dt_spec *_clk_pin;
    const struct gpio_dt_spec *_stb_pin;
    const struct gpio_dt_spec *_dio_pin;

private:
        
    struct k_msgq *queue;
    
    uint8_t _digits[16]={
      0b00111111,0b00000110,0b01011011,0b01001111,
      0b01100110,0b01101101,0b01111101,0b00000111,
      0b01111111,0b01101111,0b01110111,0b01111100,
      0b00111001,0b01011110,0b01111001,0b01110001
    };



    uint8_t _buttons;
    uint8_t _pulse;
    bool _isOn;

    void writeData(uint8_t data);
    void writeDataAt(uint8_t displayAddress, uint8_t data);
    void setDisplayMode(uint8_t displayMode);
    void setDataInstruction(uint8_t dataInstruction);
};

#endif
