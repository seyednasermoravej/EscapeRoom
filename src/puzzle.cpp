#include "puzzle.h"

Puzzle:: Puzzle(const char* room, const char* type):roomName(room), puzzleTypeName(type)
{
    // strncpy(roomName, room, sizeof(roomName) - 1);
    // roomName[sizeof(roomName) - 1] = '\0'; // Ensure null-termination
    // strncpy(puzzleTypeName, type, sizeof(puzzleTypeName) - 1);
    // puzzleTypeName[sizeof(puzzleTypeName) - 1] = '\0'; // Ensure null-termination
}


void Puzzle:: alive()
{
    MqttMsg msg = {0}; // Initialize the message structure
    sprintf(msg.topic, "%s/%s/alive", roomName, puzzleTypeName);
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