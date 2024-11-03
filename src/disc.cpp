
#include "disc.h"

LOG_MODULE_REGISTER(disc, LOG_LEVEL_INF);

static const struct pwm_dt_spec _ppP = PWM_DT_SPEC_GET(DT_NODELABEL(disc));
static const struct gpio_dt_spec _pdP = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(disc), pd_gpios, 0);
static const struct gpio_dt_spec _pdN = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(disc), pd_gpios, 1);
static const struct gpio_dt_spec _enable = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(disc), enable_gpios, 0);
static const uint16_t _pr = DT_PROP(DT_NODELABEL(disc), pulse_round);
static const uint32_t _maxFreq = DT_PROP(DT_NODELABEL(disc), max_frequency);
static const uint32_t _minFreq = DT_PROP(DT_NODELABEL(disc), min_frequency);
static const uint32_t _pulseWidth = DT_PROP(DT_NODELABEL(disc), pulse_width);


const struct gpio_dt_spec stepPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(stepper_step), gpios, {0});
const struct gpio_dt_spec directionPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(stepper_direction), gpios, {0});
Disc:: Disc()
{
    LOG_INF("Disc Puzzle selected");
    stepper = new AccelStepper(AccelStepper::DRIVER, stepPin, directionPin, directionPin, directionPin);
    stepper->setMaxSpeed(50000);
    stepper->setAcceleration(1000);
}


void Disc:: messageHandler(MqttMsg *msg)
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