#include "hx711Weight.h"


LOG_MODULE_REGISTER(hx711, LOG_LEVEL_INF);



Hx711:: Hx711(const struct device *_hx711_dev):
hx711_dev(_hx711_dev)
{
    int calibration_weight = 100; // grams
	__ASSERT(hx711_dev == NULL, "Failed to get device binding");

	LOG_INF("Device is %p, name is %s", hx711_dev, hx711_dev->name);
	LOG_INF("Calculating offset...");
	avia_hx711_tare(hx711_dev, 5);

	LOG_INF("Waiting for known weight of %d grams...",
		calibration_weight);

	avia_hx711_calibrate(hx711_dev, calibration_weight, 5);

    
}

void Hx711:: set_rate(enum hx711_rate rate)
{
	static struct sensor_value rate_val;

	rate_val.val1 = rate;
	sensor_attr_set(hx711_dev,
			SENSOR_CHAN_PRIV_START,
			SENSOR_ATTR_SAMPLING_FREQUENCY,
			&rate_val);
}

struct sensor_value Hx711:: measure(void)
{
	static struct sensor_value weight;
	int ret;

	ret = sensor_sample_fetch(hx711_dev);
	if (ret != 0) {
		LOG_ERR("Cannot take measurement: %d", ret);
	} else {
		sensor_channel_get(hx711_dev, SENSOR_CHAN_PRIV_START, &weight);
		LOG_INF("Weight: %d.%06d grams", weight.val1, weight.val2);
        return weight;   
	}
}
