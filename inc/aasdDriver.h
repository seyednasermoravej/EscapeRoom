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
    Aasd(struct k_msgq *_queueIn, struct k_msgq *_queueOut, const struct pwm_dt_spec *_servo, const struct gpio_dt_spec *_pdP, 
    const struct gpio_dt_spec *_pdN, const struct gpio_dt_spec *_ppP, 
    const struct gpio_dt_spec *_ppN);
    void messageHandler(struct AasdMsg *msg);

private:
    const struct pwm_dt_spec *servo;
    const struct gpio_dt_spec *pdP;
    const struct gpio_dt_spec *pdN;
    const struct gpio_dt_spec *ppP;
    const struct gpio_dt_spec *ppN;
    struct k_msgq *queueIn;
    struct k_msgq *queueOut;
    int getPosition();
    void setPosition(int pos);
    void setDirection(int8_t dir);
    int position = 0;
    void setZeroPosition();
    void setSpeed(uint16_t speed);
    void deviceInit();
    void servoInit();
    void gpiosInit();

};
#endif
#endif