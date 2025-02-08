#include "colorTubes.h"

LOG_MODULE_REGISTER(colorTubes, LOG_LEVEL_DBG);

static const struct i2c_dt_spec i2c_specs[] = {
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor0)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor1)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor2)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor3)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor4)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor5)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor6)),
	I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor7)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor8)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor9)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor10)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor11)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor12)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor13)),
	I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor14)),
	I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor15))
};


colorTubes:: colorTubes(const char * room, const char *type, uint8_t _numRGBsensors): Puzzle(room, type), numRGBsensors(_numRGBsensors)
{

	activateI2c0Mux0Channels();
	activateI2c1Mux0Channels();
	rgbsensors = new Adafruit_TCS34725 * [ARRAY_SIZE(i2c_specs)];
	for (uint8_t i = 0; i < ARRAY_SIZE(i2c_specs); i++) 
	{
		LOG_INF("Initializing RGB_Sensor %d", i + 1);
		rgbsensors[i] = new Adafruit_TCS34725(&i2c_specs[i],TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
		k_msleep(10);
	}
	creatingMqttList();
    k_work_init(&rgbSensorWork, rgbSensorWorkHandler);
    k_timer_init(&rgbSensorTimer, rgbSensorTimerHandler, NULL);
    k_timer_start(&rgbSensorTimer, K_SECONDS(4), K_SECONDS(1));
}


void colorTubes:: creatingMqttList()
{
    mqttCount = systemTopicsNo;
}
void colorTubes:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
	LOG_INF("the command is not valid");
}

void colorTubes:: rgbSensorTimerHandler(struct k_timer *timer)
{
	LOG_DBG("Enterd rgb Sensor timer");
	colorTubes *instance = CONTAINER_OF(timer, colorTubes, rgbSensorTimer);
	k_work_submit(&instance->rgbSensorWork);
}
void colorTubes:: rgbSensorWorkHandler(struct k_work *work)
{
	bool read = false;
	char buff[17];
    uint16_t r, g, b, c, colorTemp, lux;
	LOG_DBG("Enterd rgb Sensor work");
	colorTubes *instance = CONTAINER_OF(work, colorTubes, rgbSensorWork);
	for(uint8_t i = 0; i < ARRAY_SIZE(i2c_specs) - 1; i++)
	{
	// uint8_t i = 0;
        if (instance->rgbsensors[i]->begin()) {
            LOG_INF("Found sensor %d", i+1);
			instance->rgbsensors[i]->getRawData(&r, &g, &b, &c);
        	colorTemp = instance->rgbsensors[i]->calculateColorTemperature(r, g, b);
        	colorTemp = instance->rgbsensors[i]->calculateColorTemperature_dn40(r, g, b, c);
        	lux = instance->rgbsensors[i]->calculateLux(r, g, b);

			// r8 = (uint8_t)(r >>8 );//& 0xFF); // استخراج 8 بیت بالا
			// g8 = (uint8_t)(g >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			// b8 = (uint8_t)(b >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			// c8 = (uint8_t)(c >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			// lux8 = (uint8_t)(lux >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			// colorTemp8 = (uint8_t)(colorTemp >>8 );//& 0xFF);  // استخراج 8 بیت بالا

			sprintf(instance->msgReader.topic, "%scolorPosition%d", instance->mqttCommand, i + 1);
			sprintf(instance->msgReader.msg, "R%uG%uB%uC%uL%uT%u", r, g, b, c, lux, colorTemp);
			LOG_INF("The rgb Sensor %d is : r= %u , g= %u, b= %u, c= %u", i + 1, r, g, b, c, lux, colorTemp);
			k_msgq_put(&msqSendToMQTT, &instance->msgReader, K_NO_WAIT);
	
			k_msleep(10);
        } else {
            LOG_INF("No TCS34725 number%d found ... check your connections", i+1);
        }
		

	}

}