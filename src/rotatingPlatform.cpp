
#include "rotatingPlatform.h"

LOG_MODULE_REGISTER(rotatingPlatform, LOG_LEVEL_INF);

const struct gpio_dt_spec stepPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(stepper_step), gpios, {0});
const struct gpio_dt_spec directionPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(stepper_direction), gpios, {0});
const struct gpio_dt_spec enablePin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(stepper_enable), gpios, {0});

RotatingPlatform:: RotatingPlatform()
{
    LOG_INF("RotatingPlatform Puzzle selected");
    stepper = new AccelStepper(AccelStepper::DRIVER, stepPin, directionPin, directionPin, directionPin, enablePin);
    stepper->setMaxSpeed(50000);
    stepper->setAcceleration(1000);
}


void RotatingPlatform:: messageHandler(MqttMsg *msg)
{
    if(strcmp(msg->topic, SET_STEPPER_POSITION) == 0)
    {
        stepper->moveTo(atoi(msg->msg));
    }
    if(strcmp(msg->topic, SET_AASD_SPEED) == 0)
    {
        // aasd->setPosition(atoi(msg->msg));
    }
    if(strcmp(msg->topic, GET_AASD_SPEED) == 0)
    {
        // MqttMsg msg;
        // strcpy(msg.topic, GET_AASD_SPEED);
        // sprintf(msg.msg, "%d", aasd->getSpeed());
        // // itoa(aasd->getSpeed(), msg.msg, 10);
        // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    }
    // if(strcmp(msg->topic, GET_AASD_POSITION) == 0)
    // {
    //     // MqttMsg msg;
    //     // strcpy(msg.topic, GET_AASD_POSITION);
    //     // sprintf(msg.msg, "%d", aasd->getPosition());
    //     // // itoa(aasd->getPosition(), msg.msg, 10);
    //     // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    // }
}