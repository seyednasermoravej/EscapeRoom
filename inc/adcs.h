#ifndef __ADCS__H
#define __ADCS__H


/*adc*/
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

class Adcs
{
public:
    Adcs(const struct adc_dt_spec*, uint8_t _numOfAdcs);
    uint16_t readAdc(uint8_t channel);
private:
    const struct adc_dt_spec *adc_channels;
    uint8_t numOfAdcs;
};

#endif