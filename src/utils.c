#include "utils.h"

LOG_MODULE_REGISTER(utils, LOG_LEVEL_ERR);
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


void gpioInit(const struct gpio_dt_spec *gpio, char *message)
{
    device_init(gpio->port);
    char buf[200];
    strcpy(buf, message);
    strcat(buf, gpio->port->name);
	if (!gpio_is_ready_dt(gpio))
    {
		LOG_ERR("%s", buf);
	}
    
}

void servoInit(const struct pwm_dt_spec *pwm, char *message)
{
    device_init(pwm->dev);
    char buf[200];
    strcpy(buf, message);
    strcat(buf, pwm->dev->name);
    if (!pwm_is_ready_dt(pwm)) {
        LOG_ERR("%s\n", buf);
    }

}

