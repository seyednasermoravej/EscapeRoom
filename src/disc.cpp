#include "disc.h"


static const struct pwm_dt_spec _ppP = PWM_DT_SPEC_GET(DT_NODELABEL(disc));
static const struct gpio_dt_spec _pdP = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(disc), pd_gpios, 0);
static const struct gpio_dt_spec _pdN = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(disc), pd_gpios, 1);
static const struct gpio_dt_spec _enable = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(disc), enable_gpios, 0);
static const uint16_t _pr = DT_PROP(DT_NODELABEL(disc), pulse_round);
static const uint32_t _maxFreq = DT_PROP(DT_NODELABEL(disc), max_frequency);
static const uint32_t _minFreq = DT_PROP(DT_NODELABEL(disc), min_frequency);
static const uint32_t _pulseWidth = DT_PROP(DT_NODELABEL(disc), pulse_width);


Disc:: Disc()
{
    aasd = new Aasd(&_ppP, &_pdP, &_pdN, &_enable, _pr, _maxFreq, _minFreq, _pulseWidth);
    aasd->setSpeed(1000);
}
