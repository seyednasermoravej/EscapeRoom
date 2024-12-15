#include "xray.h"

LOG_MODULE_REGISTER(xray, LOG_LEVEL_INF);

const struct gpio_dt_spec irq = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid1), int_gpios, {0});
const struct gpio_dt_spec reset = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid1), reset_gpios, {0});
const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(DT_NODELABEL(rfid1));
// #define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	// PWM_DT_SPEC_GET_BY_IDX(node_id, idx),
// static const c allRfidIns[] = {
//     PWM_DT_SPEC_GET(DT_NODELABEL(heart_servos))
// };


Xray:: Xray(const char * room, const char *type): Puzzle(room, type)
{
	Adafruit_PN532 *rfid1 = new Adafruit_PN532(dev_i2c, irq, reset);
}