#ifndef __PUZZLES__H__
#define __PUZZLES__H__


#ifdef __cplusplus
extern "C" {
#endif

enum messageTypes
{
    RESET = 0,
    STATUS,
    ADD_SUB_TOP,
    ADD_PUB_TOP,
    REMOVE_SUB_TOP,
    REMOVE_PUB_TOP,
    HINT,
    RFID,
    ACTION,
    STATUS_REQUEST,
    BUILT_IN_LED,
};



struct Message
{
    int timestamp;
    int type;
    // struct Data data;
};


#ifdef __cplusplus
}
#endif
#endif