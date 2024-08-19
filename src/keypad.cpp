#include "keypad.h"





#define R0_PIN DT_GPIO_PIN_BY_IDX(KEYPAD_NODE, row_gpios, 0)
#define R1_PIN DT_GPIO_PIN_BY_IDX(KEYPAD_NODE, row_gpios, 1)
#define C0_PIN DT_GPIO_PIN_BY_IDX(KEYPAD_NODE, col_gpios, 0)
#define C1_PIN DT_GPIO_PIN_BY_IDX(KEYPAD_NODE, col_gpios, 1)

Keypad:: Keypad(const device *_devGpio, struct k_msgq *_queue)
{
    queue = _queue;
    devGpio = _devGpio;
    input_kbd_matrix_common_init(devGpio);
	*cfg = (const struct input_kbd_matrix_common_config *)devGpio->config;
}

// MATRIX_KEY(2, 2, INPUT_KEY_9)


// static const struct device *dev_gpio = DEVICE_DT_GET(DT_NODELABEL(gpio0));


#define KBD_DEV_COUNT (KBD_DEV_SCAN + 1)

#define COL_COUNT 2

// BUILD_ASSERT(DT_PROP_LEN(KEYPAD_NODE, col_gpios) == COL_COUNT);

static uint8_t test_rows[KBD_DEV_COUNT][COL_COUNT];
static int scan_set_count[KBD_DEV_COUNT];

void Keypad:: gpio_kbd_scan_set_row(uint8_t row)
{
    gpio_pin_configure(devGpio, R0_PIN, !(row & BIT(0)));
    gpio_pin_configure(devGpio, R1_PIN, !(row & BIT(1)));
}

void Keypad:: input_kbd_matrix_drive_column_hook(int col)
{
	gpio_flags_t flags0, flags1;

	if (col >= COL_COUNT) {
		return;
	}

    // scan_set_count[KBD_DEV_INTERRUPT]++;
    gpio_kbd_scan_set_row(test_rows[KBD_DEV_INTERRUPT][col]);

    /* Verify that columns are NOT driven. */
//     gpio_pin_get_config(devGpio, C0_PIN, &flags0);
//     gpio_pin_get_config(devGpio, C1_PIN, &flags1);
//         zassert_equal(flags0 & GPIO_DIR_MASK, GPIO_OUTPUT);
//         zassert_equal(flags1 & GPIO_DIR_MASK, GPIO_INPUT);
//         break;
//     case 1:
//         zassert_equal(flags0 & GPIO_DIR_MASK, GPIO_INPUT);
//         zassert_equal(flags1 & GPIO_DIR_MASK, GPIO_OUTPUT);
// // KEYPAD_NODE
//     case INPUT_KBD_MATRIX_COLUMN_DRIVE_NONE:
//         zassert_equal(flags0 & GPIO_DIR_MASK, GPIO_INPUT);
//         zassert_equal(flags1 & GPIO_DIR_MASK, GPIO_INPUT);
//         break;
//     case INPUT_KBD_MATRIX_COLUMN_DRIVE_ALL:
//         zassert_equal(flags0 & GPIO_DIR_MASK, GPIO_OUTPUT);
//         zassert_equal(flags1 & GPIO_DIR_MASK, GPIO_OUTPUT);


//     return;

}

/* support stuff */

static struct {
	int row;
	int col;
	int val;
	int event_count;
} test_event_data;

static int last_checked_event_count;


static void keypad_cb(struct input_event *evt)
{
	static int row, col, val;

	switch (evt->code) {
	case INPUT_ABS_X:
		col = evt->value;
		break;
	case INPUT_ABS_Y:
		row = evt->value;
		break;
	case INPUT_BTN_TOUCH:
		val = evt->value;
		break;
	}

	// if (evt->sync) {
	// 	test_event_data.row = row;
	// 	test_event_data.col = col;
	// 	test_event_data.val = val;
	// 	test_event_data.event_count++;
	// }
}
INPUT_CALLBACK_DEFINE(NULL, keypad_cb);

/* actual tests */

void  keypad()
{
	// uint8_t *rows = test_rows[KBD_DEV_INTERRUPT];
	// int *set_count = &scan_set_count[KBD_DEV_INTERRUPT];
	// int prev_count;
	gpio_flags_t flags;

	k_sleep(K_SECONDS(1));

	/* Verify that interrupts are enabled. */
	gpio_pin_get_config(devGpio, R0_PIN, &flags);
	gpio_pin_get_config(devGpio, R1_PIN, &flags);

	// rows[0] = BIT(0);
	gpio_kbd_scan_set_row(0x01);
	k_sleep(K_USEC(cfg->debounce_down_us * 1.5));

	// rows[1] = BIT(1);
	k_sleep(K_USEC(cfg->debounce_down_us * 1.5));

	// rows[0] = 0x00;
	k_sleep(K_USEC(cfg->debounce_up_us * 1.5));
	// rows[1] = 0x00;
	k_sleep(K_USEC(cfg->debounce_up_us * 1.5));

	k_sleep(K_MSEC(cfg->poll_timeout_ms * 1.5));

	// /* Check that scanning iKEYPAD_NODEning */
	// prev_count = *set_count;
	k_sleep(K_MSEC(cfg->poll_timeout_ms * 10));

	/* Verify that interrupts are still enabled. */
	// gpio_pin_get_config(devGpio, R0_PIN, &flags);
	// gpio_pin_get_config(devGpio, R1_PIN, &flags);
}
