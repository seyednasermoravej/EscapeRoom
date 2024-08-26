#include "aasdDriver.h"
#include "messageQueues.h"
#include "utils.h"


// static const struct gpio_dt_spec zero = GPIO_DT_SPEC_GET_OR(SERVOS_ZERO_NODE, gpios, {0});


LOG_MODULE_REGISTER(aasd, LOG_LEVEL_ERR);

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
    servoInit();
    commonGpiosInit();
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
    servoInit();
    commonGpiosInit();
    position = 0;
}

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
    if(speed >= 0)
    {
        setDirection(true);
        smoothStart(true);
    }
    else
    {
        setDirection(false);
        smoothStart(false);
    }

    speed *= pr;
    if(speed > maxPulseFreq)
    {
        speed = maxPulseFreq;
    }

    pwm_set_dt(ppP, PWM_HZ(speed), minPulseWidth);
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


void Aasd:: smoothStart(bool dir)
{
    // Reset frequency
    uint32_t currentPulseFreq = minPulseFreq;

    // Start with minimum frequency
    pwm_set_dt(ppP, PWM_HZ(currentPulseFreq), minPulseWidth);
    if(dir)
        position += 0.01 * currentPulseFreq; // number of steps in 0.01 second
    else
        position -= 0.01 * currentPulseFreq;// number of steps in 0.01 second

    // Start ramping timer
    k_timer_start(&rampTimer, K_MSEC(10), K_MSEC(10));  // Update every 10ms
}