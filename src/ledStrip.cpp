#include "ledStrip.h"

LOG_MODULE_REGISTER(ledStrip, LOG_LEVEL_INF);


#define DELAY_TIME K_MSEC(50)

#define RGB(_r, _g, _b) { .r = (_r), .g = (_g), .b = (_b) }

const struct led_rgb colors[3] = {
	RGB(0x0f, 0x00, 0x00), /* red */
	RGB(0x00, 0x0f, 0x00), /* green */
	RGB(0x00, 0x00, 0x0f), /* blue */
};

LedStrip:: LedStrip(const struct device *_strip, uint8_t _numPixels): strip(_strip), numPixels(_numPixels)
{

    device_init(strip);
    struct led_rgb pixels[numPixels];
    size_t color = 0;
	int rc;
    while (1) 
    {
		for (size_t cursor = 0; cursor < numPixels; cursor++) {
			memset(&pixels, 0x00, numPixels);
			memcpy(&pixels[cursor], &colors[color], sizeof(struct led_rgb));

			rc = led_strip_update_rgb(strip, pixels, numPixels);
			if (rc) {
				LOG_ERR("couldn't update strip: %d", rc);
			}

			k_sleep(DELAY_TIME);
		}

		color = (color + 1) % ARRAY_SIZE(colors);
        k_msleep(1000);
	}
}