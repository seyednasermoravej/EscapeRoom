#include "puzzle.h"

Puzzle *instance = nullptr;
Puzzle:: Puzzle(const char* room, const char* type) 
{
    instance = this;
    roomName = new char[strlen(room) + 1];
    strcpy(roomName, room);

    puzzleTypeName = new char[strlen(type) + 1];
    strcpy(puzzleTypeName, type);
    k_timer_init(&aliveTimer, Puzzle:: alive, NULL);
    k_timer_start(&aliveTimer, K_SECONDS(4), K_SECONDS(4));
}


void Puzzle:: alive(struct k_timer *timer)
{
    MqttMsg msg = {0}; // Initialize the message structure
    sprintf(msg.topic, "%s/%s/alive", instance->roomName, instance->puzzleTypeName);
    sprintf(msg.msg, "TRUE");
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
