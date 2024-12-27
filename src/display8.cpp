#include "display8.h"

static char c = segment_map[0x20];

Display8::Display8(const struct gpio_dt_spec *_display): display(_display)
{
    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(display); i++){
        if (!device_is_ready(display[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&display[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }
    k_timer_init(&displayTimer, Display8:: displayTimerHandler, NULL);
    k_work_init(&displayWork, Display8:: displayWorkHandler);
}


void Display8:: displayTimerHandler(struct k_timer *timer)
{
    Display8 *instance = CONTAINER_OF(timer, Display8, displayTimer);
    k_work_submit(&instance->displayWork);
}

void Display8:: displayWorkHandler(k_work *work)
{
    Display8 *instance = CONTAINER_OF(work, Display8, displayWork);
    instance->displayRefresh();

}
void Display8:: displayRefresh()
{
    static uint8_t pos = 0;
    c = str[strLen - 1 - pos];
    displayChar(pos, c);
    pos++;
    if(pos >= strLen)
    {
        pos = 0;
    }
}
void Display8:: displayStr(const char *_str)
{
    k_timer_stop(&displayTimer);
    strLen = strlen(_str);
    if(strLen < 9)
    {
        strcpy(str, _str);
    }
    else
    {
        strncpy(str, _str, 8);
        strLen = 8;
    }
    k_timer_start(&displayTimer, K_SECONDS(1), K_MSEC(2));
}


void Display8:: displayChar(uint8_t pos, char c)
{
    uint8_t buf[2];
    buf[0] = 0xff;
    buf[1] = 0xff;

    // gpio_port_clear_bits(display->port, 0xffff);
    reg_595_port_clear_bits_raw(display->port, buf);
    uint16_t data = (1<< pos) | segment_map[(uint8_t)c] << 8;
    uint8_t buf2[2];
    buf2[0] = (data >> 8) & 0x00ff;
    buf2[1] = (data)  & 0x00ff;
    reg_595_port_set_bits_raw(display->port, buf2);
    // gpio_port_set_bits(display->port, data);

}

const uint8_t segment_map[128] = {
    // Control characters (0–31)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x00–0x07
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x08–0x0F
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x10–0x17
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x18–0x1F

    // Printable characters (32–48)
    0xFF,                   // 0x20 (Space)
    0xFF, 0xFF, 0xFF, 0xFF, // 0x21–0x24
    0xFF, 0xFF, 0xFF, 0xFF, // 0x25–0x28
    0xFF, 0xFF, 0xFF, 0xFF, // 0x29–0x2C
    0xFF, 0xFF, 0xFF,       // 0x2D–0x2C

    // Numeric digits (0–9, ASCII 49–57)
    0xC0, // 0x30 (0) => A, B, C, D, E, F
    0xF9, // 0x31 (1) => B, C
    0xA4, // 0x32 (2) => A, B, D, E, G
    0xB0, // 0x33 (3) => A, B, C, D, G
    0x99, // 0x34 (4) => B, C, F, G
    0x92, // 0x35 (5) => A, C, D, F, G
    0x82, // 0x36 (6) => A, C, D, E, F, G
    0xF8, // 0x37 (7) => A, B, C
    0x80, // 0x38 (8) => A, B, C, D, E, F, G
    0x90, // 0x39 (9) => A, B, C, D, F, G

    0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF,
    0xFF, 

    // Characters 'A'–'Z', ASCII 65–90
    0x88, // 0x41 (A) => A, B, C, E, F, G
    0x83, // 0x42 (B) => C, D, E, F, G
    0xC6, // 0x43 (C) => A, D, E, F
    0xA1, // 0x44 (D) => B, C, D, E, G
    0x86, // 0x45 (E) => A, D, E, F, G
    0x8E, // 0x46 (F) => A, E, F, G
    0xC2, // 0x47 (G) => A, B, C, D, E, F (fallback for lowercase 'g')
    0x89, // 0x48 (H) => B, C, E, F, G
    0xF9, // 0x49 (I) => B, C
    0xF1, // 0x4A (J) => B, C, D, E
    0xFF, // 0x4B (K) => Unsupported
    0xC7, // 0x4C (L) => D, E, F
    0xFF, // 0x4D (M) => Unsupported
    0xab, // 0x4E (N) => Unsupported
    0xC0, // 0x4F (O) => A, B, C, D, E, F
    0x8C, // 0x50 (P) => A, B, E, F, G
    0x98, // 0x51 (Q) => A, B, C, D, F, G
    0xaf, // 0x52 (R) => A, B, E, F, G
    0x92, // 0x53 (S) => A, C, D, F, G
    0x87, // 0x54 (T) => D, E, F, G
    0xC1, // 0x55 (U) => B, C, D, E, F
    0xE3, // 0x56 (V) => C, D, E
    0xFF, // 0x57 (W) => Unsupported
    0xFF, // 0x58 (X) => Unsupported
    0x91, // 0x59 (Y) => B, C, D, F, G
    0xFF, // 0x5A (Z) => Unsupported

    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,// Unsupported

    // Lowercase letters 'a'–'z', ASCII 97–122
    0x88, // 0x61 (a) => Same as A
    0x83, // 0x62 (b) => Same as B
    0xC6, // 0x63 (c) => Same as C
    0xA1, // 0x64 (d) => Same as D
    0x86, // 0x65 (e) => Same as E
    0x8E, // 0x66 (f) => Same as F
    0xC2, // 0x67 (g) => Same as G
    0x89, // 0x68 (h) => Same as H
    0xF9, // 0x69 (i) => Same as I
    0xF1, // 0x6A (j) => Same as J
    0xFF, // 0x6B (k) => Unsupported
    0xC7, // 0x6C (l) => Same as L
    0xFF, // 0x6D (m) => Unsupported
    0xab, // 0x6E (n) => Unsupported
    0xC0, // 0x6F (o) => Same as O
    0x8C, // 0x70 (p) => Same as P
    0x98, // 0x71 (q) => Same as Q
    0xaf, // 0x72 (r) => Same as R
    0x92, // 0x73 (s) => Same as S
    0x87, // 0x74 (t) => Same as T
    0xC1, // 0x75 (u) => Same as U
    0xE3, // 0x76 (v) => Same as V
    0xFF, // 0x77 (w) => Unsupported
    0xFF, // 0x78 (x) => Unsupported
    0x91, // 0x79 (y) => Same as Y
    0xFF, // 0x7A (z) => Unsupported

    // Remaining characters (symbols, etc.)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF// Unsupported
};