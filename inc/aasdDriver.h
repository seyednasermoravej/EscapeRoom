#ifndef __AASD__H__
#define __AASD__H__

#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
// #include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>

#include "topics.h"



#define STEP PWM_USEC(100)
#ifdef __cplusplus
extern "C" {
#endif

struct AasdMsg
{
    bool stop;
    bool zeroPosition;
    bool setSpeed;
    uint16_t speed;
};



#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
class Aasd 
{
public:
    Aasd( 
    // Aasd(struct k_msgq *_queueIn, struct k_msgq *_queueOut, 
    const struct pwm_dt_spec *_ppP,
    const struct gpio_dt_spec *_pdP, 
    const struct gpio_dt_spec *_pdN, 
    const struct gpio_dt_spec *_setZeroSpeedPin,
    const struct gpio_dt_spec *_readZeroSpeedPin,
    const struct gpio_dt_spec *_enable,
    const struct gpio_dt_spec *_servoReady,
    const uint16_t _pr, const uint32_t _maxPulseFreq, const uint32_t _minPulseFreq,
    const uint32_t _minPulseWidth);

    Aasd(
    const struct pwm_dt_spec *_ppP,
    const struct gpio_dt_spec *_pdP, 
    const struct gpio_dt_spec *_pdN, 
    const struct gpio_dt_spec *_enable,
    const uint16_t _pr, const uint32_t _maxPulseFreq, const uint32_t _minPulseFreq,
    const uint32_t _minPulseWidth);

    void setZeroPosition();
    void setSpeed(float speed);
    int getPosition();
    void setPosition(int pos);

private:
    const struct pwm_dt_spec *ppP;
    const struct gpio_dt_spec *pdP;
    const struct gpio_dt_spec *pdN;
    const struct gpio_dt_spec *setZeroSpeedPin, *readZeroSpeedPin, *enable, *servoReady;
    struct k_msgq *queueIn;
    struct k_msgq *queueOut;
    void setDirection(bool dir);
    int position = 0;
    bool direction = true;
    void servoInit();
    void commonGpiosInit();
    void aasdGpioInit();
    uint16_t pr;
    uint32_t maxPulseFreq, minPulseFreq, minPulseWidth, deviationInSmoothStart;
    void smoothStart(bool dir);
    struct k_timer rampTimer;
};
#endif
#endif