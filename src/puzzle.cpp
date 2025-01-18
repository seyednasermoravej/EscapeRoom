#include "puzzle.h"

LOG_MODULE_REGISTER(puzzle, LOG_LEVEL_DBG);

static Puzzle *instance = nullptr;
Puzzle:: Puzzle(const char* room, const char* type) 
{
    instance = this;
    roomName = new char[strlen(room) + 1];
    strcpy(roomName, room);

    puzzleTypeName = new char[strlen(type) + 1];
    strcpy(puzzleTypeName, type);
    sprintf(mqttCommand, "%s/%s/", roomName, puzzleTypeName);
    // mqttList[0] = *createMqttTopic("erase");
    k_timer_init(&aliveTimer, Puzzle:: alive, NULL);
    k_timer_start(&aliveTimer, K_SECONDS(4), K_SECONDS(4));
}


void Puzzle:: alive(struct k_timer *timer)
{
    MqttMsg msg = {0}; // Initialize the message structure
    sprintf(msg.topic, "%s/%s/alive", instance->roomName, instance->puzzleTypeName);
    sprintf(msg.msg, "true");
    k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT); // Assuming k_msgq_put is defined elsewhere
}


uint16_t Puzzle:: getMqttCount()
{
    return mqttCount;
}

struct mqtt_topic *Puzzle:: getMqttList()
{
    return mqttList;
}

Puzzle::~Puzzle() {
    delete[] roomName;
    delete[] puzzleTypeName;
}

void Puzzle:: test()
{}



mqtt_topic* Puzzle:: createMqttTopic(const char *topicName)
{
    char *topic = new char[strlen(topicName) + 1];
    strcpy(topic, topicName);
    return new mqtt_topic {
        .topic =
        {
            .utf8 = (uint8_t*)topic,
            .size = strlen(topic)
        },
        .qos = MQTT_QOS_2_EXACTLY_ONCE
    };
}
int Puzzle::validTopic(char *topic, char *command)
{
    int ret = -1;
    if(strstr(topic, mqttCommand))
    {
        strcpy(command, &topic[strlen(mqttCommand)]);
        ret = 0;
    }
    else
        LOG_INF("the command is not valid");
    return ret;
}

int Puzzle:: peripheralIdx(const char *field, char *command)
{
    int idx = -1;
    if(strstr(command, field))
    {
        char fieldNumStr[4];
        strcpy(fieldNumStr, &command[strlen(field)]);
        idx = atoi(fieldNumStr);
    }
    else
        LOG_ERR("no valid index");
    return idx;

}


int Puzzle:: relayOperation(char *command, const gpio_dt_spec *relay, bool momentry)
{
    if(strcmp(command, "on") == 0)
    {
        if(momentry)
        {
            gpio_pin_set_dt(relay, 1);
            k_msleep(1000);
            return gpio_pin_set_dt(relay, 0);
        }
        else
        {
            return gpio_pin_set_dt(relay, 1);
        }
    }
    else if(strcmp(command, "off") == 0)
    {
        return gpio_pin_set_dt(relay, 0);
    }
    else
    {
        LOG_INF("The command is not valid");
    }
    return -1;
}


struct led_rgb Puzzle:: retrieveColors(char *str)
{
    struct led_rgb rgbw;
    char buf[4]={0};
    uint8_t i = 1;
    uint8_t j = 0;
    while(str[i] != 'G')
    {
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0';
    rgbw.g = atoi(buf); // g is r
    memset(buf, 0, 4);
    j=0;
    i++;
    while(str[i] != 'B')
    {
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0'; 
    rgbw.r = atoi(buf); // r is g
    memset(buf, 0, 4);
    j=0;
    i++;
    while(str[i] != 'W')
    {
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0'; 
    rgbw.b = atoi(buf); // r is g
    memset(buf, 0, 4);
    j=0;
    i++;    
    while(i != strlen(str))
    {
        buf[j] = str[i];
        i++;
        j++;
    }
    buf[j] = '\0';
    rgbw.w = atoi(buf);
    return rgbw;

}

int Puzzle:: activateI2c0Mux0Channels()
{

	int ret;
	ret = device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0)));
	ret = device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0)));
    // Array of devices for all mux0 channels
    const struct device *channels[] = {
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel0)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel1)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel2)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel3)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel4)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel5)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel6)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux0_channel7)),
    };

    for (uint8_t i = 0; i < ARRAY_SIZE(channels); i++) {
        if (!device_is_ready(channels[i])) {
            LOG_ERR("Channel %d is not ready", i);
            continue;
        }

        int ret = device_init(channels[i]);
        if (ret < 0) {
            LOG_ERR("Failed to initialize channel %d: %d", i, ret);
            return ret;
        } else {
            LOG_INF("Channel %d initialized successfully", i);
        }
    }
}

int Puzzle:: activateI2c0Mux1Channels()
{

	int ret;
	ret = device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0)));
	ret = device_init(DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1)));
    // Array of devices for all mux0 channels
    const struct device *channels[] = {
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel0)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel1)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel2)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel3)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel4)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel5)),
        DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel6)),
        // DEVICE_DT_GET(DT_NODELABEL(i2c0_mux1_channel7)),
    };

    for (uint8_t i = 0; i < ARRAY_SIZE(channels); i++) {
        if (!device_is_ready(channels[i])) {
            LOG_ERR("Channel %d is not ready", i);
            continue;
        }

        int ret = device_init(channels[i]);
        if (ret < 0) {
            LOG_ERR("Failed to initialize channel %d: %d", i, ret);
            return ret;
        } else {
            LOG_INF("Channel %d initialized successfully", i);
        }
    }
}