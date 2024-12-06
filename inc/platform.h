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
#include <zephyr/input/input.h>
#include "puzzle.h"

#define DT_SPEC_AND_COMMA_CONFIG_DEVICE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),

class Platform: public Puzzle
{
public:
    Platform(const char * room, const char *type);
    void messageHandler(struct MqttMsg *msg) override;
    void creatingMqttList(uint16_t) override;
    static void homeSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    static void calibrateSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    static void calibrationWorkHandler(struct k_work *work);
    static void iStopIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    void iStopWorkHandler(struct k_work *work);
    void goToStartPos();
    void stop();

   static void buttonsHandlerWrapper(struct input_event *val, void* userData);
   void buttonsHandler(struct input_event *val);


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
    int startPos = -15000;
    // int startPos = -30000;

};

#endif