#include "servos.h"

LOG_MODULE_REGISTER(servos, LOG_LEVEL_INF);




// Servos:: Servos(const struct pwm_dt_spec *_servos, uint8_t _numOfServos, uint16_t minPulse, uint16_t maxPulse, uint16_t step):
// servos(_servos), numOfservos(_numOfServos), minPulse(minPulse), maxPulse(maxPulse), step(step)

Servos:: Servos(const struct pwm_dt_spec *_servos, uint8_t _numOfServos):
servos(_servos), numOfservos(_numOfServos)
{
    device_init(servos->dev);
    for (size_t i = 0; i < numOfservos; i++) {
        pwmInit(&servos[i], "Error: PWM channel %i is not ready.");
    }
}


void Servos:: move(uint8_t index, uint32_t pulse)
{
    pwm_set_pulse_dt(&servos[index], pulse);
}