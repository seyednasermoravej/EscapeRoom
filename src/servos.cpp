#include "servos.h"

LOG_MODULE_REGISTER(servos, LOG_LEVEL_INF);




Servos:: Servos(const struct pwm_dt_spec *_servos, uint8_t _numOfServos, uint32_t minPulse, uint32_t maxPulse, uint16_t maxDegrees):
servos(_servos), numOfservos(_numOfServos), minPulse(minPulse), maxPulse(maxPulse), maxDegrees(maxDegrees)
{
    device_init(servos->dev);
    for (size_t i = 0; i < numOfservos; i++) {
        char message[64];
        sprintf(message, "Error: PWM channel %d is not ready.", i);
        pwmInit(&servos[i], message);
    }
    coeff = (float)(maxPulse - minPulse) / (float)maxDegrees;

}


void Servos:: move(uint8_t index, uint32_t degrees)
{
    uint32_t pulse = (uint32_t)((float)degrees * coeff) + minPulse;
    // uint32_t pulse = degrees + minPulse;
    if(pulse <= minPulse)
    {
        pulse = minPulse + 50;
    }
    if(pulse >= maxPulse)
    {
        pulse = maxPulse - 50;
    }
    LOG_INF("move to pulse: %u", pulse);
    pwm_set_pulse_dt(&servos[index], pulse);
    
}