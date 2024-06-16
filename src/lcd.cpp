#include "lcd.h"


K_THREAD_STACK_DEFINE(lcdInStackArea, LCD_STACK_SIZE);
K_THREAD_STACK_DEFINE(lcdOutStackArea, LCD_STACK_SIZE);

void Lcd:: _set_row_offsets(int8_t row0, int8_t row1, int8_t row2, int8_t row3)
{
	lcd_data.row_offsets[0] = row0;
	lcd_data.row_offsets[1] = row1;
	lcd_data.row_offsets[2] = row2;
	lcd_data.row_offsets[3] = row3;
}


void Lcd:: _pi_lcd_toggle_enable(const struct device *gpio_dev)
{
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_E, LOW);
	k_msleep(ENABLE_DELAY);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_E, HIGH);
	k_msleep(ENABLE_DELAY);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_E, LOW);
	k_msleep(ENABLE_DELAY);
}


void Lcd:: _pi_lcd_4bits_wr(const struct device *gpio_dev, uint8_t bits)
{
	/* High bits */
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D4, LOW);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D5, LOW);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D6, LOW);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D7, LOW);
	if ((bits & BIT(4)) == BIT(4)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D4, HIGH);
	}
	if ((bits & BIT(5)) == BIT(5)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D5, HIGH);
	}
	if ((bits & BIT(6)) == BIT(6)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D6, HIGH);
	}
	if ((bits & BIT(7)) == BIT(7)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D7, HIGH);
	}

	/* Toggle 'Enable' pin */
	_pi_lcd_toggle_enable(gpio_dev);

	/* Low bits */
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D4, LOW);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D5, LOW);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D6, LOW);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D7, LOW);
	if ((bits & BIT(0)) == BIT(0)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D4, HIGH);
	}
	if ((bits & BIT(1)) == BIT(1)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D5, HIGH);
	}
	if ((bits & BIT(2)) == BIT(2)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D6, HIGH);
	}
	if ((bits & BIT(3)) == BIT(3)) {
		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D7, HIGH);
	}

	/* Toggle 'Enable' pin */
	_pi_lcd_toggle_enable(gpio_dev);
}

// void _pi_lcd_8bits_wr(const struct device *gpio_dev, uint8_t bits)
// {
// 	/* High bits */
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D7, LOW);
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D6, LOW);
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D5, LOW);
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_D4, LOW);
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC15_D3, LOW);
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC14_D2, LOW);
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC13_D1, LOW);
// 	GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC12_D0, LOW);

// 	/* Low bits */
// 	if ((bits & BIT(0)) == BIT(0)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC12_D0, HIGH);
// 	}
// 	if ((bits & BIT(1)) == BIT(1)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC13_D1, HIGH);
// 	}
// 	if ((bits & BIT(2)) == BIT(2)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC14_D2, HIGH);
// 	}
// 	if ((bits & BIT(3)) == BIT(3)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_PC15_D3, HIGH);
// 	}
// 	if ((bits & BIT(4)) == BIT(4)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D4, HIGH);
// 	}
// 	if ((bits & BIT(5)) == BIT(5)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D5, HIGH);
// 	}
// 	if ((bits & BIT(6)) == BIT(6)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D6, HIGH);
// 	}
// 	if ((bits & BIT(7)) == BIT(7)) {
// 		GPIO_PIN_WR(gpio_dev, GPIO_PIN_D7, HIGH);
// 	}

// 	/* Toggle 'Enable' pin */
// 	_pi_lcd_toggle_enable(gpio_dev);
// }

void Lcd:: _pi_lcd_data(const struct device *gpio_dev, uint8_t bits)
{
	// if (lcd_data.disp_func & LCD_8BIT_MODE) {
	// 	_pi_lcd_8bits_wr(gpio_dev, bits);
	// } else {
		_pi_lcd_4bits_wr(gpio_dev, bits);
	// }
}

void Lcd:: _pi_lcd_command(const struct device *gpio_dev, uint8_t bits)
{
	/* mode = False for command */
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_RS, LOW);
	_pi_lcd_data(gpio_dev, bits);
}

void Lcd:: _pi_lcd_write(const struct device *gpio_dev, uint8_t bits)
{
	/* mode = True for character */
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_RS, HIGH);
	_pi_lcd_data(gpio_dev, bits);
}


/*************************
 * USER can use these APIs
 *************************/
/** Home */
void Lcd:: pi_lcd_home(const struct device *gpio_dev)
{
	_pi_lcd_command(gpio_dev, LCD_RETURN_HOME);
	k_sleep(K_MSEC(2));			/* wait for 2ms */
}

/** Set cursor position */
void Lcd:: pi_lcd_set_cursor(const struct device *gpio_dev, uint8_t col,
		       uint8_t row)
{
	size_t max_lines;

	max_lines = ARRAY_SIZE(lcd_data.row_offsets);
	if (row >= max_lines) {
		row = max_lines - 1;	/* Count rows starting w/0 */
	}
	if (row >= lcd_data.cfg_rows) {
		row = lcd_data.cfg_rows - 1;    /* Count rows starting w/0 */
	}
	_pi_lcd_command(gpio_dev, (LCD_SET_DDRAM_ADDR | (col + lcd_data.row_offsets[row])));
}


/** Clear display */
void Lcd:: pi_lcd_clear(const struct device *gpio_dev)
{
	_pi_lcd_command(gpio_dev, LCD_CLEAR_DISPLAY);
	k_sleep(K_MSEC(2));			/* wait for 2ms */
}


/** Display ON */
void Lcd:: pi_lcd_display_on(const struct device *gpio_dev)
{
	lcd_data.disp_cntl |= LCD_DISPLAY_ON;
	_pi_lcd_command(gpio_dev,
			LCD_DISPLAY_CONTROL | lcd_data.disp_cntl);
}

/** Display OFF */
void Lcd:: pi_lcd_display_off(const struct device *gpio_dev)
{
	lcd_data.disp_cntl &= ~LCD_DISPLAY_ON;
	_pi_lcd_command(gpio_dev,
			LCD_DISPLAY_CONTROL | lcd_data.disp_cntl);
}


/** Turns cursor off */
void Lcd:: pi_lcd_cursor_off(const struct device *gpio_dev)
{
	lcd_data.disp_cntl &= ~LCD_CURSOR_ON;
	_pi_lcd_command(gpio_dev,
			LCD_DISPLAY_CONTROL | lcd_data.disp_cntl);
}

/** Turn cursor on */
void Lcd:: pi_lcd_cursor_on(const struct device *gpio_dev)
{
	lcd_data.disp_cntl |= LCD_CURSOR_ON;
	_pi_lcd_command(gpio_dev,
			LCD_DISPLAY_CONTROL | lcd_data.disp_cntl);
}


/** Turn off the blinking cursor */
void Lcd:: pi_lcd_blink_off(const struct device *gpio_dev)
{
	lcd_data.disp_cntl &= ~LCD_BLINK_ON;
	_pi_lcd_command(gpio_dev,
			LCD_DISPLAY_CONTROL | lcd_data.disp_cntl);
}

/** Turn on the blinking cursor */
void Lcd:: pi_lcd_blink_on(const struct device *gpio_dev)
{
	lcd_data.disp_cntl |= LCD_BLINK_ON;
	_pi_lcd_command(gpio_dev,
			LCD_DISPLAY_CONTROL | lcd_data.disp_cntl);
}

/** Scroll the display left without changing the RAM */
void Lcd:: pi_lcd_scroll_left(const struct device *gpio_dev)
{
	_pi_lcd_command(gpio_dev, LCD_CURSOR_SHIFT |
			LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);
}

/** Scroll the display right without changing the RAM */
void Lcd:: pi_lcd_scroll_right(const struct device *gpio_dev)
{
	_pi_lcd_command(gpio_dev, LCD_CURSOR_SHIFT |
			LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);
}

/** Text that flows from left to right */
void Lcd:: pi_lcd_left_to_right(const struct device *gpio_dev)
{
	lcd_data.disp_mode |= LCD_ENTRY_LEFT;
	_pi_lcd_command(gpio_dev,
			LCD_ENTRY_MODE_SET | lcd_data.disp_cntl);
}

/** Text that flows from right to left */
void Lcd:: pi_lcd_right_to_left(const struct device *gpio_dev)
{
	lcd_data.disp_mode &= ~LCD_ENTRY_LEFT;
	_pi_lcd_command(gpio_dev,
			LCD_ENTRY_MODE_SET | lcd_data.disp_cntl);
}

/** Right justify text from the cursor location */
void Lcd:: pi_lcd_auto_scroll_right(const struct device *gpio_dev)
{
	lcd_data.disp_mode |= LCD_ENTRY_SHIFT_INCREMENT;
	_pi_lcd_command(gpio_dev,
			LCD_ENTRY_MODE_SET | lcd_data.disp_cntl);
}

/** Left justify text from the cursor location */
void Lcd:: pi_lcd_auto_scroll_left(const struct device *gpio_dev)
{
	lcd_data.disp_mode &= ~LCD_ENTRY_SHIFT_INCREMENT;
	_pi_lcd_command(gpio_dev,
			LCD_ENTRY_MODE_SET | lcd_data.disp_cntl);
}

void Lcd:: pi_lcd_string(const struct device *gpio_dev, char *msg)
{
	int i;
	int len = 0;
	uint8_t data;

	len = strlen(msg);
	if (len > LCD_WIDTH) {
		printk("Too long message! len %d %s\n", len, msg);
	}

	for (i = 0; i < len; i++) {
		data = msg[i];
		_pi_lcd_write(gpio_dev, data);
	}
}


/** LCD initialization function */
void Lcd:: pi_lcd_init(const struct device *gpio_dev, uint8_t cols, uint8_t rows,
		 uint8_t dotsize)
{
	if (rows > 1) {
		lcd_data.disp_func |= LCD_2_LINE;
	}
	lcd_data.cfg_rows = rows;

	_set_row_offsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

	/* For 1 line displays, a 10 pixel high font looks OK */
	if ((dotsize != LCD_5x8_DOTS) && (rows == 1U)) {
		lcd_data.disp_func |= LCD_5x10_DOTS;
	}

	/* SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	 * according to datasheet, we need at least 40ms after power rises
	 * above 2.7V before sending commands. Arduino can turn on way
	 * before 4.5V so we'll wait 50
	 */
	k_sleep(K_MSEC(50));

	/* this is according to the hitachi HD44780 datasheet
	 * figure 23/24, pg 45/46 try to set 4/8 bits mode
	 */
	if (lcd_data.disp_func & LCD_8BIT_MODE) {
		/* 1st try */
		_pi_lcd_command(gpio_dev, 0x30);
		k_sleep(K_MSEC(5));			/* wait for 5ms */

		/* 2nd try */
		_pi_lcd_command(gpio_dev, 0x30);
		k_sleep(K_MSEC(5));			/* wait for 5ms */

		/* 3rd try */
		_pi_lcd_command(gpio_dev, 0x30);
		k_sleep(K_MSEC(1));			/* wait for 1ms */

		/* Set 4bit interface */
		_pi_lcd_command(gpio_dev, 0x30);
	} else {
		/* 1st try */
		_pi_lcd_command(gpio_dev, 0x03);
		k_sleep(K_MSEC(5));			/* wait for 5ms */

		/* 2nd try */
		_pi_lcd_command(gpio_dev, 0x03);
		k_sleep(K_MSEC(5));			/* wait for 5ms */

		/* 3rd try */
		_pi_lcd_command(gpio_dev, 0x03);
		k_sleep(K_MSEC(1));			/* wait for 1ms */

		/* Set 4bit interface */
		_pi_lcd_command(gpio_dev, 0x02);
	}

	/* finally, set # lines, font size, etc. */
	_pi_lcd_command(gpio_dev, (LCD_FUNCTION_SET | lcd_data.disp_func));

	/* turn the display on with no cursor or blinking default */
	lcd_data.disp_cntl = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
	pi_lcd_display_on(gpio_dev);

	/* clear it off */
	pi_lcd_clear(gpio_dev);

	/* Initialize to default text direction */
	lcd_data.disp_mode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	/* set the entry mode */
	_pi_lcd_command(gpio_dev, LCD_ENTRY_MODE_SET | lcd_data.disp_mode);
}

Lcd:: Lcd(const struct device *const gpioDev, struct k_msgq *_queue, uint8_t RS, uint8_t E, uint8_t BL, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7): gpio_dev(gpioDev)
{
	queue = _queue;
	lcd_data.disp_func = LCD_4BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS;
	lcd_data.disp_cntl = 0;
	lcd_data.disp_mode = 0;
	lcd_data.cfg_rows = 0;
	lcd_data.row_offsets[0] = 0x00;
	lcd_data.row_offsets[1] = 0x00;
	lcd_data.row_offsets[2] = 0x00;
	lcd_data.row_offsets[3] = 0x00;

	if (!device_is_ready(gpio_dev)) {
		printk("Device %s not ready!\n", gpio_dev->name);

	}
	GPIO_PIN_E = E;
	GPIO_PIN_RS = RS;
	GPIO_PIN_D4 = D4;
	GPIO_PIN_D5 = D5;
	GPIO_PIN_D6 = D6;
	GPIO_PIN_D7 = D7;	
	GPIO_PIN_BL = BL;
	/* Setup GPIO output */
	GPIO_PIN_CFG(gpio_dev, GPIO_PIN_E, GPIO_OUTPUT);
	GPIO_PIN_CFG(gpio_dev, GPIO_PIN_RS, GPIO_OUTPUT);
	// GPIO_PIN_CFG(gpio_dev, GPIO_PIN_PC12_D0, GPIO_OUTPUT);
	// GPIO_PIN_CFG(gpio_dev, GPIO_PIN_PC13_D1, GPIO_OUTPUT);
	// GPIO_PIN_CFG(gpio_dev, GPIO_PIN_PC14_D2, GPIO_OUTPUT);
	// GPIO_PIN_CFG(gpio_dev, GPIO_PIN_PC15_D3, GPIO_OUTPUT);
	GPIO_PIN_CFG(gpio_dev, GPIO_PIN_D4, GPIO_OUTPUT);
	GPIO_PIN_CFG(gpio_dev, GPIO_PIN_D5, GPIO_OUTPUT);
	GPIO_PIN_CFG(gpio_dev, GPIO_PIN_D6, GPIO_OUTPUT);
	GPIO_PIN_CFG(gpio_dev, GPIO_PIN_D7, GPIO_OUTPUT);
	GPIO_PIN_CFG(gpio_dev, GPIO_PIN_BL, GPIO_OUTPUT);
	GPIO_PIN_WR(gpio_dev, GPIO_PIN_BL, HIGH);


	printk("LCD Init\n");
	pi_lcd_init(gpio_dev, 16, 2, LCD_5x8_DOTS);

	/* Clear display */
	pi_lcd_clear(gpio_dev);
	struct LcdMsg msg;
	while(1)
	{
	if(k_msgq_get(queue, &msg, K_NO_WAIT) == 0) {
		printk("Page 1: message\n");

		pi_lcd_clear(gpio_dev);
		pi_lcd_string(gpio_dev, msg.firstLine);
		pi_lcd_set_cursor(gpio_dev, 0, 1);
		pi_lcd_string(gpio_dev, msg.secondLine);
		k_msleep(MSEC_PER_SEC * 3U);
		memset(&msg, 0, sizeof(struct LcdMsg));

	}
	k_msleep(2000);
	}
}





struct k_thread lcdInThread;
struct k_thread lcdOutThread;



void lcdEntryPointLcdIn(void *, void *, void *)
{
	Lcd *lcd = new Lcd(DEVICE_DT_GET(LCD_IN_NODE), &msqLcdIn, 0, 2, 3, 4, 5, 6, 7);

}

void lcdEntryPointLcdOut(void *, void *, void *)
{
	Lcd *lcd = new Lcd(DEVICE_DT_GET(LCD_OUT_NODE), &msqLcdOut, 0, 2, 3, 4, 5, 6, 7);

}


extern "C" void lcdThreadCreate()
{
	// k_tid_t lcdTidIn = k_thread_create(&lcdInThread, lcdInStackArea,
	// 								K_THREAD_STACK_SIZEOF(lcdInStackArea),
	// 								lcdEntryPointLcdIn, NULL, NULL, NULL,
	// 								LCD_PRIORITY, 0, K_NO_WAIT);

	k_tid_t lcdTidOut = k_thread_create(&lcdOutThread, lcdOutStackArea,
									K_THREAD_STACK_SIZEOF(lcdOutStackArea),
									lcdEntryPointLcdOut, NULL, NULL, NULL,
									LCD_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(lcdTidOut, "LCD IN");
}