#ifndef __LCD__H__
#define __LCD__H__

// /*
//  * SPDX-License-Identifier: Apache-2.0
//  */

// /**
//  * @file Display text strings on HD44780 based 20x4 LCD controller
//  * using GPIO for parallel interface on Arduino Due.
//  *
//  * Datasheet: http://lcd-linux.sourceforge.net/pdfdocs/hd44780.pdf
//  *
//  * LCD Wiring
//  * ----------
//  *
//  * The wiring for the LCD is as follows:
//  * 1 : GND
//  * 2 : 5V
//  * 3 : Contrast (0-5V)*
//  * 4 : RS (Register Select)
//  * 5 : R/W (Read Write)       - GROUND THIS PIN
//  * 6 : Enable or Strobe
//  * 7 : Data Bit 0             - NOT USED
//  * 8 : Data Bit 1             - NOT USED
//  * 9 : Data Bit 2             - NOT USED
//  * 10: Data Bit 3             - NOT USED
//  * 11: Data Bit 4
//  * 12: Data Bit 5
//  * 13: Data Bit 6
//  * 14: Data Bit 7
//  * 15: LCD Backlight +5V**
//  * 16: LCD Backlight GND
//  *
//  *
//  * Arduino Due
//  * -----------
//  *
//  * On Arduino Due:
//  * 1. IO_3 is PC28
//  * 2. IO_5 is PC25
//  * 3. IO_6 is PC24
//  * 4. IO_7 is PC23
//  * 5. IO_8 is PC22
//  * 6. IO_9 is PC21
//  *
//  * The gpio_atmel_sam3 driver is being used.
//  *
//  * This sample app display text strings per line & page wise.
//  *
//  * Every 3 second you should see this repeatedly
//  * on display:
//  * "
//  *     *********************
//  *     Arduino Due
//  *     16x2 LCD Display
//  *     *********************
//  *
//  *      ------------------
//  *     - Zephyr Rocks!
//  *     - My super RTOS
//  *      ------------------
//  *
//  *     --------HOME--------
//  *     I am home!
//  *
//  *	--------------------
//  */

#include <zephyr/kernel.h>

#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>
#include "messageQueues.h"
#include <zephyr/logging/log.h>

#define LCD1_NODE DT_NODELABEL(lcd1)
#define LCD2_NODE DT_NODELABEL(lcd2)


#define LCD_PRIORITY            8
#define LCD_STACK_SIZE       1024


// #define GPIO_PIN_PC24_D4		4	/* PC24 - pin 6 */
// #define GPIO_PIN_PC23_D5		5	/* PC23 - pin 7 */
// #define GPIO_PIN_PC22_D6		6	/* PC22 - pin 8 */
// #define GPIO_PIN_PC21_D7		7	/* PC21 - pin 9 */
// #define GPIO_PIN_PC28_RS		0	/* PC28 - pin 3 */
// #define GPIO_PIN_PC24_BL		3	/* PC24 - pin 6 */
// #define GPIO_PIN_PC25_E			2	/* PC25 - pin 5 */
#define GPIO_NAME			"GPIO_"


/* Commands */
#define LCD_CLEAR_DISPLAY		0x01
#define LCD_RETURN_HOME			0x02
#define LCD_ENTRY_MODE_SET		0x04
#define LCD_DISPLAY_CONTROL		0x08
#define LCD_CURSOR_SHIFT		0x10
#define LCD_FUNCTION_SET		0x20
#define LCD_SET_CGRAM_ADDR		0x40
#define LCD_SET_DDRAM_ADDR		0x80

/* Display entry mode */
#define LCD_ENTRY_RIGHT			0x00
#define LCD_ENTRY_LEFT			0x02
#define LCD_ENTRY_SHIFT_INCREMENT	0x01
#define LCD_ENTRY_SHIFT_DECREMENT	0x00

/* Display on/off control */
#define LCD_DISPLAY_ON			0x04
#define LCD_DISPLAY_OFF			0x00
#define LCD_CURSOR_ON			0x02
#define LCD_CURSOR_OFF			0x00
#define LCD_BLINK_ON			0x01
#define LCD_BLINK_OFF			0x00

/* Display/cursor shift */
#define LCD_DISPLAY_MOVE		0x08
#define LCD_CURSOR_MOVE			0x00
#define LCD_MOVE_RIGHT			0x04
#define LCD_MOVE_LEFT			0x00

/* Function set */
#define LCD_8BIT_MODE			0x10
#define LCD_4BIT_MODE			0x00
#define LCD_2_LINE			0x08
#define LCD_1_LINE			0x00
#define LCD_5x10_DOTS			0x04
#define LCD_5x8_DOTS			0x00

/* Define some device constants */
#define LCD_WIDTH			16	/* Max char per line */
#define HIGH				1
#define LOW				0
/* in millisecond */
#define	ENABLE_DELAY			10


#define GPIO_PIN_WR(dev, pin, bit)						\
	do {									\
		if (gpio_pin_set_raw((dev), (pin), (bit))) {			\
			LOG_ERR("Err set " GPIO_NAME "%d! %x\n", (pin), (bit));	\
		}								\
	} while (0)								\


#define GPIO_PIN_CFG(dev, pin, dir)						\
	do {									\
		if (gpio_pin_configure((dev), (pin), (dir))) {			\
			LOG_ERR("Err cfg " GPIO_NAME "%d! %x\n", (pin), (dir));	\
		}								\
	} while (0)


struct pi_lcd_data {
	uint8_t	disp_func;	/* Display Function */
	uint8_t	disp_cntl;	/* Display Control */
	uint8_t disp_mode;	/* Display Mode */
	uint8_t	cfg_rows;
	uint8_t	row_offsets[4];
};

/* Default Configuration - User can update */




class Lcd
{
    public:
    Lcd(const struct device *const gpioDev, struct k_msgq *_queue, uint8_t RS, uint8_t E, uint8_t BL, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7);

    private:
    uint8_t GPIO_PIN_E, GPIO_PIN_RS, GPIO_PIN_BL, GPIO_PIN_D4, GPIO_PIN_D5, GPIO_PIN_D6, GPIO_PIN_D7;
    struct pi_lcd_data lcd_data;
    const struct device *const gpio_dev;
    struct k_msgq *queue;
    void _set_row_offsets(int8_t row0, int8_t row1, int8_t row2, int8_t row3);
    void _pi_lcd_toggle_enable(const struct device *gpio_dev);
    void _pi_lcd_4bits_wr(const struct device *gpio_dev, uint8_t bits);
    void _pi_lcd_data(const struct device *gpio_dev, uint8_t bits);
    void _pi_lcd_command(const struct device *gpio_dev, uint8_t bits);
    void _pi_lcd_write(const struct device *gpio_dev, uint8_t bits);
    void pi_lcd_home(const struct device *gpio_dev);
    void pi_lcd_set_cursor(const struct device *gpio_dev, uint8_t col, uint8_t row);
    void pi_lcd_clear(const struct device *gpio_dev);
    void pi_lcd_display_on(const struct device *gpio_dev);
    void pi_lcd_display_off(const struct device *gpio_dev);
    void pi_lcd_cursor_off(const struct device *gpio_dev);
    void pi_lcd_cursor_on(const struct device *gpio_dev);
    void pi_lcd_blink_off(const struct device *gpio_dev);
    void pi_lcd_blink_on(const struct device *gpio_dev);
    void pi_lcd_scroll_left(const struct device *gpio_dev);
    void pi_lcd_scroll_right(const struct device *gpio_dev);
    void pi_lcd_left_to_right(const struct device *gpio_dev);
    void pi_lcd_right_to_left(const struct device *gpio_dev);
    void pi_lcd_auto_scroll_right(const struct device *gpio_dev);
    void pi_lcd_auto_scroll_left(const struct device *gpio_dev);
    void pi_lcd_string(const struct device *gpio_dev, char *msg);
    void pi_lcd_init(const struct device *gpio_dev, uint8_t cols, uint8_t rows, uint8_t dotsize);
};





#endif