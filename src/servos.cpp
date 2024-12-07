#include "servos.h"

LOG_MODULE_REGISTER(servos, LOG_LEVEL_INF);




Servos:: Servos(const struct pwm_dt_spec *_servos, uint8_t _numOfServos, uint32_t minPulse, uint32_t maxPulse, uint16_t maxDegrees):
servos(_servos), numOfservos(_numOfServos), minPulse(minPulse), maxPulse(maxPulse), maxDegrees(maxDegrees)

// Servos:: Servos(const struct pwm_dt_spec *_servos, uint8_t _numOfServos):
// servos(_servos), numOfservos(_numOfServos)
{
    device_init(servos->dev);
    for (size_t i = 0; i < numOfservos; i++) {
        pwmInit(&servos[i], "Error: PWM channel %i is not ready.");
    }
    coeff = (float)(maxPulse - minPulse) / (float)maxDegrees;
}


void Servos:: move(uint8_t index, uint32_t degrees)
{
    uint32_t pulse = (uint32_t)((float)degrees * coeff) + minPulse;
    // uint32_t pulse = degrees + minPulse;
    if(pulse < minPulse)
    {
        pulse = minPulse;
    }
    if(pulse > maxPulse)
    {
        pulse = maxPulse;
    }
    pwm_set_pulse_dt(&servos[index], pulse);
    
}