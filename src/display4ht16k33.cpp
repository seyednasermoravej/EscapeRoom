#include "display4ht16k33.h"

#define DOUBLE_DOTS 36//16 + 16 + 1
LOG_MODULE_REGISTER(display4ht16k33, LOG_LEVEL_INF);

Display4:: Display4(const device *_display): display(_display)
{
    device_init(display);
    if (!device_is_ready(display)) {
		LOG_ERR("LED device not ready");
		return;
    }
    
    k_timer_init(&blinkTimer, Display4:: blinkTimerHandler, NULL);
    k_work_init(&blinkWork, Display4:: blinkWorkHandler);
}


void Display4:: blinkTimerHandler(struct k_timer *timer)
{
    Display4 *instance = CONTAINER_OF(timer, Display4, blinkTimer);
    k_work_submit(&instance->blinkWork);
}

void Display4:: blinkWorkHandler(k_work *work)
{
    static bool blink = false;
    Display4 *instance = CONTAINER_OF(work, Display4, blinkWork);
    if(blink)
    {
        led_on(instance->display, DOUBLE_DOTS);
    }
    else
    {
        led_off(instance->display, DOUBLE_DOTS);
    }
    blink = !blink;
    

}

void Display4:: displayStr(char *text)
{
    k_timer_start(&blinkTimer, K_SECONDS(1), K_SECONDS(1));
    for (uint8_t i = 0; i < 128; i++) {
        if(i == DOUBLE_DOTS)
        {
            continue;
        }
        led_off(display, i);
        // k_sleep(K_MSEC(100));
    }
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

void Display4:: displayChar(char c, uint8_t _pos)
{
    uint8_t code = ~segment_map[(uint8_t)c];
    uint8_t base = 16;
    uint8_t val = 0;
    uint8_t pos;
    if(_pos == 3)
    {
        _pos = 4;
    }
    if(_pos == 2)
    {
        _pos = 3;
    }
    for(uint8_t i = 0; i < 8; i++)
    {
        pos = ((_pos) * base) + i;
        // pos = ((_pos + 1) * base) + i;
        val = (code >> i) & 0x01;
        // ((c >> i) & 0x01) ? led_on(display, base + (pos * 8) + i): led_off(display, base + (pos * 8) + i);

        if(val)
        {
            led_on(display, pos);
        }
        else
        {
            led_off(display, pos);
        }
    }

}