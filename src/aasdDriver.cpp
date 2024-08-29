#include "aasdDriver.h"
#include "messageQueues.h"
#include "utils.h"
#include <math.h>


// static const struct gpio_dt_spec zero = GPIO_DT_SPEC_GET_OR(SERVOS_ZERO_NODE, gpios, {0});


LOG_MODULE_REGISTER(aasd, LOG_LEVEL_ERR);

Aasd* Aasd::instance = nullptr;
// Aasd:: Aasd(struct k_msgq *_queueIn, struct k_msgq *_queueOut, 
Aasd:: Aasd(const struct pwm_dt_spec *_ppP,
    const struct gpio_dt_spec *_pdP, 
    const struct gpio_dt_spec *_pdN, 
    const struct gpio_dt_spec *_setZeroSpeedPin,
    const struct gpio_dt_spec *_readZeroSpeedPin,
    const struct gpio_dt_spec *_enable,
    const struct gpio_dt_spec *_servoReady,
    const uint16_t _pr, const uint32_t _maxPulseFreq, const uint32_t _minPulseFreq, const uint32_t _minPulseWidth):
    // queueIn(_queueIn), queueOut(_queueOut),
    ppP(_ppP), pdP(_pdP), pdN(_pdN), setZeroSpeedPin(_setZeroSpeedPin),
    readZeroSpeedPin(_readZeroSpeedPin), enable(_enable), servoReady(_servoReady),
    pr(_pr), maxPulseFreq(_maxPulseFreq), minPulseFreq(_minPulseFreq), minPulseWidth(_minPulseWidth)
{
    commonInits();
    aasdGpioInit();
    setZeroPosition();
}

// Aasd:: Aasd(struct k_msgq *_queueIn, struct k_msgq *_queueOut,
Aasd:: Aasd(const struct pwm_dt_spec *_ppP,
    const struct gpio_dt_spec *_pdP, 
    const struct gpio_dt_spec *_pdN, 
    const struct gpio_dt_spec *_enable,
    const uint16_t _pr, const uint32_t _maxPulseFreq, const uint32_t _minPulseFreq, const uint32_t _minPulseWidth):
    // queueIn(_queueIn), queueOut(_queueOut),
    ppP(_ppP), pdP(_pdP), pdN(_pdN),
    enable(_enable), pr(_pr), maxPulseFreq(_maxPulseFreq), minPulseFreq(_minPulseFreq),
    minPulseWidth(_minPulseWidth)
{
    commonInits();
}

// void Aasd:: rampHandler(struct k_timer *timer)
// {
//     // Increase frequency
//     Aasd:: instance -> currentPulseFreq += (Aasd:: instance -> maxPulseFreq - Aasd:: instance ->minPulseFreq) / (K_MSEC(10) / k_timer_remaining_get(timer));
    
//     if (Aasd:: instance ->currentPulseFreq > Aasd:: instance ->maxPulseFreq) {
//         Aasd:: instance ->currentPulseFreq = Aasd:: instance ->maxPulseFreq;
//         k_timer_stop(timer);
//     }

//     // Update PWM
//     pwm_set_dt(&pwm_servo, (1000000 / current_freq), MIN_PULSE_WIDTH_US);
// }
int Aasd:: getPosition()
{
    return position;
}

void Aasd:: setPosition(int pos)
{

}

void Aasd:: setZeroPosition()
{
    GPIO_PIN_WR(setZeroSpeedPin->port, setZeroSpeedPin->pin, HIGH);
    position = 0;
}

void Aasd:: setSpeed(float speed)
{
    // if(speed >= 0)
    // {
    //     setDirection(true);
    // }
    // else
    // {
    //     setDirection(false);
    // }

    speed *= pr;
    while (!stop)
    {
        if(speed > currentPulseFreq)
        {
            currentPulseFreq += minStepFreq;
            if(currentPulseFreq > maxPulseFreq)
            {
                currentPulseFreq = maxPulseFreq;
            }
        }
        else if(speed < currentPulseFreq)
        {
            currentPulseFreq -= minStepFreq;
            if(abs(currentPulseFreq) > maxPulseFreq)
            {
                currentPulseFreq = -maxPulseFreq;
            }
        }
        else
        {
            //do nothing
        }
        k_msleep(10);
        position += (currentPulseFreq * 0.01);
        if(currentPulseFreq > 0)
        {
            setDirection(true);
        }
        else
        {
            setDirection(false);
        }
        pwm_set_dt(ppP, PWM_HZ(abs(currentPulseFreq)), minPulseWidth);
    }
    pwm_set_dt(ppP, 0, 0);
}

void Aasd:: servoInit()
{
    pwmInit(ppP, "pwm channel for aasd driver is not ready.");
}

void Aasd:: commonGpiosInit()
{
    gpioInit(pdP, "Error: PD+ is not ready.");
    GPIO_PIN_CFG(pdP->port, pdP->pin, GPIO_OUTPUT);
    gpioInit(pdN, "Error: PD- is not ready.");
    GPIO_PIN_CFG(pdN->port, pdN->pin, GPIO_OUTPUT);
    gpioInit(enable,"Error: servo enable is not ready.");
    GPIO_PIN_CFG(enable->port, enable->pin, GPIO_OUTPUT);

}

void Aasd:: aasdGpioInit()
{
    gpioInit(setZeroSpeedPin, "Error: set zero speed pin is not ready.");
    GPIO_PIN_CFG(setZeroSpeedPin->port, setZeroSpeedPin->pin, GPIO_OUTPUT);
    gpioInit(readZeroSpeedPin, "Error: read zero speed pin is not ready");
    GPIO_PIN_CFG(pdN->port, pdN->pin, GPIO_INPUT);
    gpioInit(servoReady,"Error: servo ready is not ready.");
    GPIO_PIN_CFG(pdN->port, pdN->pin, GPIO_INPUT);

}

void Aasd:: setDirection(bool dir)
{
    if(dir == direction)
    {
        return;
    }
    if(dir)
    {
        GPIO_PIN_WR(pdN->port, pdN->pin, LOW);
        GPIO_PIN_WR(pdP->port, pdP->pin, HIGH);
        direction = true;
    }
    else
    {
        GPIO_PIN_WR(pdP->port, pdP->pin, LOW);
        GPIO_PIN_WR(pdN->port, pdN->pin, HIGH);
        direction = false;
    }
}


// void Aasd:: smoothStart(bool dir)
// {
//     // Reset frequency

//     // Start with minimum frequency
//     pwm_set_dt(ppP, PWM_HZ(currentPulseFreq), minPulseWidth);
//     if(dir)
//         position += 0.01 * currentPulseFreq; // number of steps in 0.01 second
//     else
//         position -= 0.01 * currentPulseFreq;// number of steps in 0.01 second

//     // Start ramping timer
//     k_timer_start(&rampTimer, K_MSEC(10), K_MSEC(10));  // Update every 10ms
// }

void Aasd:: commonInits()
{
    servoInit();
    commonGpiosInit();
    currentPulseFreq = minPulseFreq;
    position = 0;
    instance = this;
    currentPulseFreq = 0;
    minStepFreq = (maxPulseFreq + minPulseFreq) / 2;
    // k_timer_init(&rampTimer, Aasd:: rampHandler, NULL);
}