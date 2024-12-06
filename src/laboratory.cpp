#include "laboratory.h"
LOG_MODULE_REGISTER(colorPuzzle, LOG_LEVEL_INF);

const struct i2c_dt_spec i2cDev = I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor0));

Laboratory:: Laboratory() 
{

    // ledStrip = new LedStrip(DEVICE_DT_GET(DT_NODELABEL(color_led_strip)), DT_PROP(DT_NODELABEL(color_led_strip), chain_length));
    tcs = new Adafruit_TCS34725(&i2cDev);
    tcs->begin();
    while(1) {
        uint16_t r, g, b, c, colorTemp, lux;

        tcs->getRawData(&r, &g, &b, &c);
        // colorTemp = tcs.calculateColorTemperature(r, g, b);
        colorTemp = tcs->calculateColorTemperature_dn40(r, g, b, c);
        lux = tcs->calculateLux(r, g, b);

        LOG_INF("Color Temp: %d", colorTemp);
        LOG_INF("Lux: %d", lux);
        LOG_INF("R: %d ",r ); 
        LOG_INF("G: %d ",g ); 
        LOG_INF("B: %d ",b ); 
        LOG_INF("C: %d ",c ); 
        k_msleep(2000);
    }
};

void Laboratory:: messageHandler(struct MqttMsg *msg)
{
   uint32_t k = 100; 
}
