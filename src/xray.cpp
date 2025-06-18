#include "xray.h"

LOG_MODULE_REGISTER(xray, LOG_LEVEL_INF);

// static const struct device *dev_i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));
// static const c allRfidIns[] = {
//     PWM_DT_SPEC_GET(DT_NODELABEL(heart_servos))
// };
static const struct i2c_dt_spec i2c_specs[] = {
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid1)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid2)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid3)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid4)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid5)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid6)),
    I2C_DT_SPEC_GET(DT_NODELABEL(rfid7))
};

static const struct gpio_dt_spec gpio_specs[] = {
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid1), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid2), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid3), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid4), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid5), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid6), reset_gpios, {0}),
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rfid7), reset_gpios, {0})
};



Xray:: Xray(const char * room, const char *type, uint8_t _numRfids): Puzzle(room, type), numRfids(_numRfids)
{
    activateI2c0Mux0Channels();

	k_msleep(1);
	rfids = new Adafruit_PN532 * [numRfids];
	tags = new char * [numRfids];
	for (uint8_t i = 0; i < numRfids; i++)
	{
		LOG_INF("Initializing RFID %d", i + 1);
		rfids[i] = new Adafruit_PN532(&i2c_specs[i], &gpio_specs[i]);
		tags[i] = new char[MAX_RFID_TAGS_LEN];
#ifdef WATCH_DOG
        wdt_feed(wdt, wdt_channel_id);
#endif
		k_msleep(10);
	}
	creatingMqttList();
	char *tempBuffer = new char[numRfids * MAX_RFID_TAGS_LEN];

    nvs_read(fileSystem, NVS_RFID_TAGS, tempBuffer, sizeof(char) * numRfids * MAX_RFID_TAGS_LEN);
    for (uint8_t i = 0; i < numRfids; i++)
    {
        memcpy(tags[i], tempBuffer + i * MAX_RFID_TAGS_LEN, MAX_RFID_TAGS_LEN);
    }
    delete[] tempBuffer;
//     exit(0);
    k_work_init(&cardsReaderWork, cardsReaderWorkHandler);
    k_timer_init(&cardsReaderTimer, cardsReaderTimerHandler, NULL);
    k_timer_start(&cardsReaderTimer, K_SECONDS(4), K_SECONDS(1));
}


void Xray:: creatingMqttList()
{
    char topic[128] = {0};
    for(uint8_t i = 0; i < numRfids; i++)
    {
        sprintf(topic, "%stag%d", mqttCommand, i + 1);
        mqttList[i + systemTopicsNo] = *createMqttTopic(topic);
    }
    mqttCount = numRfids + systemTopicsNo;
}


void Xray:: messageHandler(struct MqttMsg *msg)
{
    int rc;
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        if(strstr(command, "tag") != NULL)
        {
            char field[] = "tag";
            int commandIdx = peripheralIdx(field, command);
            uint8_t tagIdx = commandIdx - 1;
            if((commandIdx > 0 ) && (tagIdx < numRfids))
            {
		strcpy(tags[tagIdx], msg->msg);
		    // Copy tags to the temporary buffer for NVS write

		char *tempBuffer = new char[numRfids * MAX_RFID_TAGS_LEN];
		for (uint8_t i = 0; i < numRfids; i++)
		{
			memcpy(tempBuffer + i * MAX_RFID_TAGS_LEN, tags[i], MAX_RFID_TAGS_LEN);
		}
		nvs_write(fileSystem, NVS_RFID_TAGS, tempBuffer, sizeof(char) * numRfids * MAX_RFID_TAGS_LEN);
		delete[] tempBuffer;
            }
            else
            {
                LOG_ERR("Not a valid index");
            }

        }
        else
        {
            LOG_INF("the command is not valid");
        }
    }
    else
        LOG_INF("the command is not valid");
}

void Xray:: cardsReaderTimerHandler(struct k_timer *timer)
{
	LOG_DBG("Enterd card reader timer");
	Xray *instance = CONTAINER_OF(timer, Xray, cardsReaderTimer);
	k_work_submit(&instance->cardsReaderWork);
}
void Xray:: cardsReaderWorkHandler(struct k_work *work)
{
	bool read = false;
	bool correct = true;
	char buff[17];
	LOG_DBG("Enterd card reader work");
	Xray *instance = CONTAINER_OF(work, Xray, cardsReaderWork);
	for(uint8_t i = 0; i < instance->numRfids; i++)
	{
		instance->rfids[i]->begin();
		read = instance->rfids[i]->readCard(buff, 200);
		if(read)
		{
			LOG_INF("The card rfid %d is : %s", i + 1, buff);
			LOG_INF("The card tag %d is : %s", i + 1, instance->tags[i]);
			if(strcmp(instance->tags[i], buff) != 0)
			{
				correct = false;
				LOG_INF("Tag is not matched, tag id: %d", i + 1);
			}
			else
			{
				correct = true;
				LOG_INF("Tag is matched, tag id: %d", i + 1);
			}
			// (strcmp(instance->tags[i], buff) ? (correct &= false): (correct &= true));
			sprintf(instance->msgReader.topic, "%srfid%d", instance->mqttCommand, i + 1);
			sprintf(instance->msgReader.msg, "%s", buff);
			k_msgq_put(&msqSendToMQTT, &instance->msgReader, K_NO_WAIT);
			LOG_DBG("tags number %d is: %s", i + 1);
		}
		k_msleep(10);

	}

}
