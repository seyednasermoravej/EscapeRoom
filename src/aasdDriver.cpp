#include "aasdDriver.h"
#include "messageQueues.h"
#include "utils.h"



// static const struct pwm_dt_spec servo = 


// static const struct gpio_dt_spec zero = GPIO_DT_SPEC_GET_OR(SERVOS_ZERO_NODE, gpios, {0});


LOG_MODULE_REGISTER(aasd, LOG_LEVEL_ERR);

Aasd:: Aasd(struct k_msgq *_queueIn, struct k_msgq *_queueOut, const struct pwm_dt_spec *_servo, const struct gpio_dt_spec *_pdP, 
    const struct gpio_dt_spec *_pdN, const struct gpio_dt_spec *_ppP, 
    const struct gpio_dt_spec *_ppN):
    queueIn(_queueIn), queueOut(_queueOut),
    servo(_servo), pdP(_pdP), pdN(_pdN), ppP(_ppP), ppN(_ppN)
{
    servoInit();
    gpiosInit();


}

int Aasd:: getPosition()
{

}

void Aasd:: setPosition(int pos)
{

}

void Aasd:: setZeroPosition()
{

}

void Aasd:: setSpeed(uint16_t speed)
{
    
}

void Aasd:: deviceInit()
{
    servoInit();
    gpiosInit();
}

void Aasd:: servoInit()
{
    device_init(servo->dev);

}

void Aasd:: gpiosInit()
{
    gpioInit(ppP, "Error: PP+ is not ready\n");
    gpioInit(ppN, "Error: PP- is not ready\n");
    gpioInit(pdP, "Error: PD+ is not ready\n");
    gpioInit(pdN, "Error: PD- is not ready\n");

    // device_init(ppP->port)

}