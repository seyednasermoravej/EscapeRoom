#include "puzzle.h"

LOG_MODULE_REGISTER(puzzle, LOG_LEVEL_DBG);

Puzzle *instance = nullptr;
Puzzle:: Puzzle(const char* room, const char* type) 
{
    instance = this;
    roomName = new char[strlen(room) + 1];
    strcpy(roomName, room);

    puzzleTypeName = new char[strlen(type) + 1];
    strcpy(puzzleTypeName, type);
    sprintf(mqttCommand, "%s/%s/", roomName, puzzleTypeName);
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
            gpio_pin_set_dt(relay, 0);
        }
        else
        {
            gpio_pin_set_dt(relay, 1);
        }
    }
    else if(strcmp(command, "off") == 0)
    {
        gpio_pin_set_dt(relay, 0);
    }
    else
    {
        LOG_INF("The command is not valid");
    }
}