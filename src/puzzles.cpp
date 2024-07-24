#include "puzzles.h"
#include "topics.h"
LOG_MODULE_REGISTER(puzzle, LOG_LEVEL_INF);

K_THREAD_STACK_DEFINE(puzzleStackArea, PUZZLE_STACK_SIZE);

struct k_thread puzzleThread;

static const struct gpio_dt_spec builtInLed = GPIO_DT_SPEC_GET_OR(BUILT_IN_NODE, gpios,
							      {0});



Puzzle::Puzzle() {

    builtIntLedInit();
}



Puzzle *puzzle = nullptr;

void Puzzle:: puzzleTypeSelection(char *type)
{
    deviceSpecified = true;

    
    if(strcmp(type, "servos") == 0)
    {
        puzzleType = SERVOS_PUZZLE;
        servos = new Servos;
        LOG_INF("Puzzle type is Servos");
    }
    else if(strcmp(type, "gate") == 0)
    {
        puzzleType = GATE_PUZZLE;
        gate = new Gate;
        LOG_INF("Puzzle type is Gate");
    }
    else if(strcmp(type, "config") == 0)
    {
        puzzleType = CONFIG_DEVICE_PUZZLE;
        configDevice = new ConfigDevice;
        LOG_INF("Device is configuring by user.");
    }
    else
    {
        deviceSpecified = false;
    }

}


void Puzzle:: mqttInMessageHandler(struct MqttMsg *msg)
{
    int ret;
    if(!deviceSpecified)
    {
        if(strcmp(msg->topic, PUZZLE_TYPE_TOPIC) == 0)
        {
            puzzleTypeSelection(msg->msg);
        }
    }
    else
    {
        
        if(strcmp(msg->topic, BUILT_IN_LED_TOPIC) == 0)
        {   
            int ret;
            if(strncmp("0", msg->msg, 1) == 0)
            {
                ret = gpio_pin_set_dt(&builtInLed, 0);
                LOG_INF("Built in led deactivate");
            }
            else if(strncmp("1", msg->msg, 1) == 0)
            {
                ret = gpio_pin_set_dt(&builtInLed, 1);
                LOG_INF("Built in led activate");
            }
            else
            {
                LOG_INF("Command is not recognized");
            }
        }
        else
        {
            
            switch (puzzle->puzzleType)
            {
            case GATE_PUZZLE:
                gate -> messageHandler(msg);
                break;
            
            case SERVOS_PUZZLE:
                servos -> messageHandler(msg);
                break;

            case CONFIG_DEVICE_PUZZLE:
                configDevice -> messageHandler(msg);
                break;

            default:
                break;
            }
        }


    }

}


int Puzzle:: builtIntLedInit()
{
    int ret;
	if (!gpio_is_ready_dt(&builtInLed)) {
		printk("Error: button device %s is not ready\n",
		       builtInLed.port->name);
		return 0;
	}
    ret = gpio_pin_configure_dt(&builtInLed, GPIO_OUTPUT_ACTIVE);

	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, builtInLed.port->name, builtInLed.pin);
		return 0;
	}

}

void puzzleEntryPoint(void *, void *, void *)
{
    struct MqttMsg *msg = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));

    memset(msg, 0, sizeof(struct MqttMsg));
    puzzle = new Puzzle;
    while(!puzzle->deviceSpecified)
    {
        if(k_msgq_get(&msqReceivedFromMQTT, msg, K_NO_WAIT) == 0)
        {
            puzzle -> mqttInMessageHandler(msg);
            memset(msg, 0, sizeof(struct MqttMsg));
           
        }
        k_msleep(1000);

    }
    
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
    k_thread_name_set(puzzleTid, "puzzle");
}


