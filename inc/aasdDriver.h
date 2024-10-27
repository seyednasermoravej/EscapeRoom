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

#define RAMP_DURATION_MS 5000  

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
    int getSpeed();
    void setPosition(int pos);
    static void positionHandler(struct k_timer *timer);
    void forceSpeedZero();

    static Aasd *instance;
    struct k_timer positionTimer;
    struct k_timer speedTimer;

private:
    const struct pwm_dt_spec *ppP;
    const struct gpio_dt_spec *pdP;
    const struct gpio_dt_spec *pdN;
    const struct gpio_dt_spec *setZeroSpeedPin, *readZeroSpeedPin, *enable, *servoReady;
    struct k_msgq *queueIn;
    struct k_msgq *queueOut;
    void setDirection(bool dir);
    void setDirection(int difPos);
    bool getDirection();
    int position = 0;
    bool direction = true;
    void servoInit();
    void commonGpiosInit();
    void aasdGpioInit();
    uint16_t pr;
    uint32_t maxPulseFreq, minPulseFreq, minPulseWidth, deviationInSmoothStart;
    int currentPulseFreq;
    void commonInits();
    bool stop = false;
    uint32_t minStepFreq;
    uint32_t stepNum = 0;
    void movePositionStep(uint32_t timeMS, int movement);
    void accelStep(uint32_t timerDelay, int speedChange, bool forward);
};
#endif
#endif