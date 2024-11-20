#ifndef __ROTATING_PLATFORM__H__
#define __ROTATING_PLATFORM__H__

#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include "messageQueues.h"
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>

#include <semaphore.h>
#include "topics.h"
#include "AccelStepper.h"
// #include "aasdDriver.h"

#define DT_SPEC_AND_COMMA_CONFIG_DEVICE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),

class RotatingPlatform 
{
public:
    RotatingPlatform();
    void messageHandler(struct MqttMsg *msg);
    static void homeSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    static void calibrateSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    static void calibrationWorkHandler(struct k_work *work);
    static void iStopIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    void iStopWorkHandler(struct k_work *work);
    static void buttonsIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    void buttonsIrq(const struct device *dev, uint32_t pins);
    void goToStartPos();
    void stop();

private:
    AccelStepper *stepper = NULL;
    struct k_work calibrationWork;
    struct k_work iStopWork;
    int stepperInit();
    int buttonsInit();
    int relaysInit();
    int iStopInit();
    int homeSwitchInit(); 
    int calibrateSwitchInit();
    struct gpio_callback homeSwitch_cb_data;
    struct gpio_callback calibrateSwitch_cb_data;
    struct gpio_callback iStop_cb_data;
    struct gpio_callback buttons_cb_data;
    void calibration();
    void goToHome();
    bool isHome = false;
    double stepsPerDegree = 0;
    void goToPosition(int pos);
    bool calibrated = false;
    int startPos = -30000;

};











#endif