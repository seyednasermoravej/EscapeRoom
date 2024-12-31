#include "colorTubes.h"

LOG_MODULE_REGISTER(colorTubes, LOG_LEVEL_DBG);

static const struct device *dev0_i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));
static const struct device *dev1_i2c = DEVICE_DT_GET(DT_NODELABEL(i2c1));

static const struct i2c_dt_spec i2c_specs[] = {
    I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor0)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor1)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor2)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor3)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor4)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor5)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor6)),
	// I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor7)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor8)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor9)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor10)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor11)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor12)),
    // I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor13)),
	// I2C_DT_SPEC_GET(DT_NODELABEL(rgb_sensor14))
};


colorTubes:: colorTubes(const char * room, const char *type, uint8_t _numRGBsensors): Puzzle(room, type), numRGBsensors(_numRGBsensors)
{
	device_init(dev0_i2c);
	device_init(dev1_i2c);
	k_msleep(1);

	rgbsensors = new Adafruit_TCS34725 * [ARRAY_SIZE(i2c_specs)];
	for (uint8_t i = 0; i < ARRAY_SIZE(i2c_specs); i++) 
	{
		// device_init(i2c_specs[i].bus);
    	// if (!device_is_ready(i2c_specs[i].bus)) {
        // 	LOG_ERR("Sensor device not ready");
    	// }
		LOG_INF("Initializing RGB_Sensor %d", i + 1);
		rgbsensors[i] = new Adafruit_TCS34725(&i2c_specs[i],TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
		k_msleep(10);
	}
	createMqttTopic(0);
    k_work_init(&rgbSensorWork, rgbSensorWorkHandler);
    k_timer_init(&rgbSensorTimer, rgbSensorTimerHandler, NULL);
    k_timer_start(&rgbSensorTimer, K_SECONDS(4), K_SECONDS(1));
}


void colorTubes:: creatingMqttList(uint16_t _mqttCount)
{
    mqttCount = _mqttCount;
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
	uint8_t r8, g8, b8, c8, colorTemp8, lux8;
	LOG_DBG("Enterd rgb Sensor work");
	colorTubes *instance = CONTAINER_OF(work, colorTubes, rgbSensorWork);
	for(uint8_t i = 0; i < ARRAY_SIZE(i2c_specs); i++)
	{
	// uint8_t i = 0;
        if (instance->rgbsensors[i]->begin()) {
            LOG_INF("Found sensor %d", i+1);
			instance->rgbsensors[i]->getRawData(&r, &g, &b, &c);
        	colorTemp = instance->rgbsensors[i]->calculateColorTemperature(r, g, b);
        	colorTemp = instance->rgbsensors[i]->calculateColorTemperature_dn40(r, g, b, c);
        	lux = instance->rgbsensors[i]->calculateLux(r, g, b);

			r8 = (uint8_t)(r >>8 );//& 0xFF); // استخراج 8 بیت بالا
			g8 = (uint8_t)(g >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			b8 = (uint8_t)(b >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			c8 = (uint8_t)(c >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			lux8 = (uint8_t)(lux >>8 );//& 0xFF);  // استخراج 8 بیت بالا
			colorTemp8 = (uint8_t)(colorTemp >>8 );//& 0xFF);  // استخراج 8 بیت بالا

			sprintf(instance->msgReader.topic, "%s/%s/colorPosition%d", instance->roomName, instance->puzzleTypeName, i + 1);
			sprintf(instance->msgReader.msg, "R%uG%uB%u", r8, g8, b8);
			LOG_INF("The rgb Sensor %d is : r= %u , g= %u, b= %u", i + 1, r8, g8, b8);
			k_msgq_put(&msqSendToMQTT, &instance->msgReader, K_NO_WAIT);
	
			k_msleep(10);
        } else {
            LOG_INF("No TCS34725 number%d found ... check your connections", i+1);
        }
		

	}

}