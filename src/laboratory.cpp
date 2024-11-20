#include "laboratory.h"



Laboratory:: Laboratory() 
{

    // ledStrip = new LedStrip(DEVICE_DT_GET(DT_NODELABEL(laboratory_led_strip)), DT_PROP(DT_NODELABEL(laboratory_led_strip), chain_length));
};

void Laboratory:: messageHandler(struct MqttMsg *msg)
{
   uint32_t k = 100; 
}
