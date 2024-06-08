#include "puzzles.h"
#include "topics.h"
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


static const struct gpio_dt_spec hintButton = GPIO_DT_SPEC_GET_OR(HINT_NODE, gpios,
							      {0});

static const struct json_obj_descr messageFromServerDescr[] = 
{
    JSON_OBJ_DESCR_PRIM(struct MessageFromServer, timestamp, JSON_TOK_NUMBER),
    // JSON_OBJ_DESCR_PRIM(struct Message, type, JSON_TOK_NUMBER),
    // JSON_OBJ_DESCR_PRIM(struct WelcomeMessage, pubTopic, JSON_TOK_STRING),
    // JSON_OBJ_DESCR_PRIM(struct WelcomeMessage, subTopic, JSON_TOK_STRING),
    JSON_OBJ_DESCR_ARRAY(struct MessageFromServer, servos, MAX_NUMBER_OF_SERVO_MOTORS, numOfServos, JSON_TOK_NUMBER),
};

static const struct json_obj_descr hintMessageDescr[] = 
{
    JSON_OBJ_DESCR_PRIM(struct HintMessage, timestamp, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(struct HintMessage, type, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(struct HintMessage, status, JSON_TOK_TRUE),
};

static struct gpio_callback hintButton_cb_data;
void hintPressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
    struct HintMessage *message = (struct HintMessage*)k_malloc(sizeof(struct HintMessage));
    char *buf = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
    message->status = true;
    message->timestamp = k_cycle_get_32();
    message->type = HINT;
    json_obj_encode_buf(hintMessageDescr, ARRAY_SIZE(hintMessageDescr), message, buf, MESSAGE_QUEUE_LEN);
    k_msgq_put(&msqSendToMQTT, buf, K_NO_WAIT);


}
Puzzle::Puzzle() {
    // Implementation
    // ...
}



Puzzle *puzzle = nullptr;


void Puzzle:: mqttInMessageHandler(struct MqttMsg *msg)
{
    int ret;
    if(strcpy(msg->topic, SERVO0_TOPIC))
    {
        int val = (((atoi(msg->msg)/ 10) + 9) * STEP) + servoMinPulse;
        ret = pwm_set_pulse_dt(&allServos[0], val);
    }

        // for(uint8_t i = 0; i < message->numOfServos; i++)
        // {

        // }

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

    	int ret;

	if (!gpio_is_ready_dt(&hintButton)) {
		printk("Error: button device %s is not ready\n",
		       hintButton.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&hintButton, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, hintButton.port->name, hintButton.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&hintButton,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, hintButton.port->name, hintButton.pin);
		return 0;
	}

	gpio_init_callback(&hintButton_cb_data, hintPressed, BIT(hintButton.pin));
	gpio_add_callback(hintButton.port, &hintButton_cb_data);
	printk("Set up button at %s pin %d\n", hintButton.port->name, hintButton.pin);
    return 0;
}
void puzzleEntryPoint(void *, void *, void *)
{
    // char *buffer = (char *)k_malloc(sizeof(char) * MESSAGE_QUEUE_LEN);
    // while(k_msgq_get(&msqReceivedFromMQTT, buffer, K_FOREVER) != 0);
    struct MqttMsg *msg = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));

    memset(msg, 0, sizeof(struct MqttMsg));
    PuzzleTypes puzzleType = SERVO_DEVICE;
    puzzle = new Puzzle;
    deviceInit();
    // if(welcomeMessage.puzzleType == SERVO_DEVICE)
    // {
        // puzzle->servo0 = PWM_DT_SPEC_GET(DT_NODELABEL(servo));
    // }
    
    while(1)
    {
        if(k_msgq_get(&msqReceivedFromMQTT, msg, K_NO_WAIT) == 0)
        {
            puzzle -> mqttInMessageHandler(msg);
            memset(msg, 0, sizeof(struct MqttMsg));
           
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


