#include "display4ht16k33.h"


LOG_MODULE_REGISTER(display4ht16k33, LOG_LEVEL_INF);

Display4:: Display4(const device *_display): display(_display)
{
    device_init(display);
    if (!device_is_ready(display)) {
		LOG_ERR("LED device not ready");
		return;
    }
    	// for (uint8_t i = 0; i < 128; i++) {
		// 	led_on(display, i);
		// 	k_sleep(K_MSEC(100));
		// }
}

void Display4:: displayStr(char *text)
{
    for(uint8_t i = 0; i < strlen(text); i++)    
    {
        if(i == 4)
        {
            break;
        }
        else
        {
            displayChar(text[i], i);
        }
    }
}

void Display4:: displayChar(char c, uint8_t pos)
{
    uint8_t code = segment_map[c];
    uint8_t base = 8;
    for(uint8_t i = 0; i < 8; i++)
    {
        ((c >> i) & 0x01) ? led_on(display, base + (pos * 8) + i): led_off(display, base + (pos * 8) + i);
    }

}