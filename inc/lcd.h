#ifndef __LCD__H__
#define __LCD__H__
void lcdInit();
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
//  *     20x4 LCD Display
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

// #include <zephyr/kernel.h>

// #include <zephyr/sys/printk.h>
// #include <zephyr/drivers/gpio.h>
// #include <string.h>


// /* Commands */
// #define LCD_CLEAR_DISPLAY		0x01
// #define LCD_RETURN_HOME			0x02
// #define LCD_ENTRY_MODE_SET		0x04
// #define LCD_DISPLAY_CONTROL		0x08
// #define LCD_CURSOR_SHIFT		0x10
// #define LCD_FUNCTION_SET		0x20
// #define LCD_SET_CGRAM_ADDR		0x40
// #define LCD_SET_DDRAM_ADDR		0x80

// /* Display entry mode */
// #define LCD_ENTRY_RIGHT			0x00
// #define LCD_ENTRY_LEFT			0x02
// #define LCD_ENTRY_SHIFT_INCREMENT	0x01
// #define LCD_ENTRY_SHIFT_DECREMENT	0x00

// /* Display on/off control */
// #define LCD_DISPLAY_ON			0x04
// #define LCD_DISPLAY_OFF			0x00
// #define LCD_CURSOR_ON			0x02
// #define LCD_CURSOR_OFF			0x00
// #define LCD_BLINK_ON			0x01
// #define LCD_BLINK_OFF			0x00

// /* Display/cursor shift */
// #define LCD_DISPLAY_MOVE		0x08
// #define LCD_CURSOR_MOVE			0x00
// #define LCD_MOVE_RIGHT			0x04
// #define LCD_MOVE_LEFT			0x00

// /* Function set */
// #define LCD_8BIT_MODE			0x10
// #define LCD_4BIT_MODE			0x00
// #define LCD_2_LINE			0x08
// #define LCD_1_LINE			0x00
// #define LCD_5x10_DOTS			0x04
// #define LCD_5x8_DOTS			0x00

// /* Define some device constants */
// #define LCD_WIDTH			20	/* Max char per line */
// #define HIGH				1
// #define LOW				0
// /* in millisecond */
// #define	ENABLE_DELAY			10


// #define GPIO_PIN_WR(dev, pin, bit)						\
// 	do {									\
// 		if (gpio_pin_set_raw((dev), (pin), (bit))) {			\
// 			printk("Err set " GPIO_NAME "%d! %x\n", (pin), (bit));	\
// 		}								\
// 	} while (0)								\


// #define GPIO_PIN_CFG(dev, pin, dir)						\
// 	do {									\
// 		if (gpio_pin_configure((dev), (pin), (dir))) {			\
// 			printk("Err cfg " GPIO_NAME "%d! %x\n", (pin), (dir));	\
// 		}								\
// 	} while (0)


// struct pi_lcd_data {
// 	uint8_t	disp_func;	/* Display Function */
// 	uint8_t	disp_cntl;	/* Display Control */
// 	uint8_t disp_mode;	/* Display Mode */
// 	uint8_t	cfg_rows;
// 	uint8_t	row_offsets[4];
// };

// /* Default Configuration - User can update */
// struct pi_lcd_data lcd_data = {
// 	.disp_func = LCD_4BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS,
// 	.disp_cntl = 0,
// 	.disp_mode = 0,
// 	.cfg_rows = 0,
// 	.row_offsets = {0x00, 0x00, 0x00, 0x00}
// };



// class Lcd()
// {
//     public:

//     private:

// }





#endif