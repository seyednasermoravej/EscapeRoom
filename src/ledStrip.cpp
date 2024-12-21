#include "ledStrip.h"

LOG_MODULE_REGISTER(ledStrip, LOG_LEVEL_INF);


#define DELAY_TIME K_MSEC(50)
//#define DELAY_TIME K_MSEC(CONFIG_SAMPLE_LED_UPDATE_DELAY)

const struct led_rgb colors[4] = {
	RGB(0x0f, 0x00, 0x00), /* red */
	RGB(0x00, 0x0f, 0x00), /* green */
	RGB(0x00, 0x00, 0x0f), /* blue */
	RGB(0x00, 0x00, 0x00),
};
//static struct led_rgb pixels[16];


LedStrip:: LedStrip(const struct device *_strip, uint8_t _numPixels): strip(_strip), numPixels(_numPixels)
{
	

    pixels = new led_rgb[numPixels];
    if (!pixels) {
        LOG_ERR("Failed to allocate memory for pixels!");
        numPixels = 0;
    }

    memset(pixels, 0, sizeof(struct led_rgb) * numPixels);

	

	if (device_is_ready(strip)) {
		LOG_INF("Found LED strip device %s", strip->name);
	} else {
		LOG_ERR("LED strip device %s is not ready", strip->name);
	}


	int rc = led_strip_update_rgb(strip, pixels, numPixels);
	if (rc) {
		LOG_ERR("couldn't initialize strip: %d", rc);
	}

}


int LedStrip:: update(struct led_rgb &rgb, uint8_t number)
{
	pixels[number] = rgb;
	int rc = led_strip_update_rgb(strip, pixels, numPixels);
	if (rc) {
		LOG_ERR("couldn't update strip: %d", rc);
	}
	return rc;
}