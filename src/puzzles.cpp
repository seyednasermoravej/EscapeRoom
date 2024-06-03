#include "puzzles.h"
LOG_MODULE_REGISTER(puzzle, LOG_LEVEL_INF);

K_THREAD_STACK_DEFINE(puzzleStackArea, PUZZLE_STACK_SIZE);
#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	PWM_DT_SPEC_GET_BY_IDX(node_id, idx),
struct k_thread puzzleThread;
static const uint32_t servoMinPulse = DT_PROP(DT_NODELABEL(servos), min_pulse);
static const uint32_t servoMaxPulse = DT_PROP(DT_NODELABEL(servos), max_pulse);
static const struct pwm_dt_spec allServos[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(servos), pwms, DT_SPEC_AND_COMMA)
};
Puzzle::Puzzle() {
    // Implementation
    // ...
}


static const struct json_obj_descr messageFromServerDescr[] = 
{
    JSON_OBJ_DESCR_PRIM(struct MessageFromServer, timestamp, JSON_TOK_NUMBER),
    // JSON_OBJ_DESCR_PRIM(struct Message, type, JSON_TOK_NUMBER),
    // JSON_OBJ_DESCR_PRIM(struct WelcomeMessage, pubTopic, JSON_TOK_STRING),
    // JSON_OBJ_DESCR_PRIM(struct WelcomeMessage, subTopic, JSON_TOK_STRING),
    JSON_OBJ_DESCR_ARRAY(struct MessageFromServer, servos, MAX_NUMBER_OF_SERVO_MOTORS, numOfServos, JSON_TOK_NUMBER),
};



Puzzle *puzzle = nullptr;


void Puzzle:: mqttInMessageHandler(char *data)
{
    struct MessageFromServer *message = (struct MessageFromServer *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
    memset(message, 0, sizeof(struct MessageFromServer));
    int ret = json_obj_parse(data, strlen(data), messageFromServerDescr, ARRAY_SIZE(messageFromServerDescr), message);
    if(message->numOfServos)
    {
        while(1)
        {
            for(uint8_t i = 0; i < message->numOfServos; i++)
            {
                int val = ((((message->servos[i] / 10) + 9) * STEP) + servoMinPulse);
                ret = pwm_set_pulse_dt(&allServos[i], val);
            }
            k_msleep(1000);
        }

        // for(uint8_t i = 0; i < message->numOfServos; i++)
        // {

        // }

    }
}
int deviceInit()
{
	/* Configure channels individually prior to sampling. */
	for (size_t i = 0; i < ARRAY_SIZE(allServos); i++) {
        if (!pwm_is_ready_dt(&allServos[i])) {
            printk("Error: servo device %s is not ready\n",
                allServos[i].dev->name);
            return 1;
        }
    }
    return 0;
}
void puzzleEntryPoint(void *, void *, void *)
{
    // char *buffer = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
    // while(k_msgq_get(&msqReceivedFromMQTT, buffer, K_FOREVER) != 0);
    char *mqttBuffer = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);

    memset(mqttBuffer, 0, MESSAGE_QUEUE_LEN);
    mqttBuffer[0] = '\0';
    PuzzleTypes puzzleType = SERVO_DEVICE;
    puzzle = new Puzzle;
    deviceInit();
    // if(welcomeMessage.puzzleType == SERVO_DEVICE)
    // {
        // puzzle->servo0 = PWM_DT_SPEC_GET(DT_NODELABEL(servo));
    // }

    while(1)
    {
        if(k_msgq_get(&msqReceivedFromMQTT, mqttBuffer, K_NO_WAIT) == 0)
        {
            puzzle -> mqttInMessageHandler(mqttBuffer);
            memset(mqttBuffer, 0, MESSAGE_QUEUE_LEN);
            mqttBuffer[0] = '\0';
        }
        k_msleep(1000);
    }
}

extern "C" void puzzleThreadCreate()
{
    k_tid_t puzzleTid = k_thread_create(&puzzleThread, puzzleStackArea,
									K_THREAD_STACK_SIZEOF(puzzleStackArea),
									puzzleEntryPoint, NULL, NULL, NULL,
									PUZZLE_PRIORITY, 0, K_NO_WAIT);
}


