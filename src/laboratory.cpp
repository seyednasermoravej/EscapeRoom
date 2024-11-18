#include "laboratory.h"

const struct device *const _strip = DEVICE_DT_GET(DT_NODELABEL(laboratory_led_strip));
Laboratory:: Laboratory() 
{
    // strip = _strip;
        // strip = DEVICE_DT_GET(DT_NODELABEL(laboratory_led_strip));
};

void Laboratory:: messageHandler(struct MqttMsg *msg)
{
    
}
