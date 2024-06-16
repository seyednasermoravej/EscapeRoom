#include "util.h"

void hex2stringMac(const uint8_t *hex, int length, char *buf)
{
    for(int i = 0; i < length - 1; i++)
    {
        hex2char((*(hex + i) >> 4), (buf + (3 * i)));
        hex2char((*(hex + i) & 0x0f), (buf + (3 * i) + 1));
        buf[3 * i + 2] = ' ';
    }
    hex2char(*(hex + (length - 1)) >> 4, (buf + (3 * (length - 1))));
    hex2char(*(hex + (length - 1)) & 0xf, (buf + (3 * (length - 1) + 1)));
    *(buf + (3 * (length - 1) + 2)) = '\0';
}