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
    device_init(ppP->port);
	if (!gpio_is_ready_dt(ppP)) {
		LOG_ERR("Error: PP+ %s is not ready\n",
		       ppP->port->name);
	}

    device_init(ppN->port);
	if (!gpio_is_ready_dt(ppN)) {
		LOG_ERR("Error: PP- %s is not ready\n",
		       ppN->port->name);
	}

    device_init(pdP->port);
	if (!gpio_is_ready_dt(pdP)) {
		LOG_ERR("Error: PD+ %s is not ready\n",
		       pdP->port->name);
	}

    device_init(pdN->port);
	if (!gpio_is_ready_dt(pdN)) {
		LOG_ERR("Error: PD- %s is not ready\n",
		       pdN->port->name);
	}
    // device_init(ppP->port)

}