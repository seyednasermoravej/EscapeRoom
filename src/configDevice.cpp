#include "configDevice.h"

LOG_MODULE_REGISTER(configDevice, LOG_LEVEL_INF);


#define LCD1_NODE DT_NODELABEL(lcd1)
#define LCD2_NODE DT_NODELABEL(lcd2)


#define DT_SPEC_AND_COMMA_CONFIG_DEVICE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),


static const struct gpio_dt_spec rotaryEncoder[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(rotaryencoder), gpios, DT_SPEC_AND_COMMA_CONFIG_DEVICE)
};

static struct gpio_callback rotaryEncoder_cb_data;

ConfigDevice:: ConfigDevice()
{
    // inputsInit();
	device_init(DEVICE_DT_GET(LCD1_NODE));
	// device_init(DEVICE_DT_GET(LCD2_NODE));
	lcd1 = new Lcd(DEVICE_DT_GET(LCD1_NODE), 0, 2, 3, 4, 5, 6, 7);
	// lcd2 = new Lcd(DEVICE_DT_GET(LCD2_NODE), 0, 2, 3, 4, 5, 6, 7);
}
static int prevclkA;
static int prevPin8;

void rotaryChange(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
    for (int i = 0; i < ARRAY_SIZE(rotaryEncoder); i ++)
    {
    gpio_pin_interrupt_configure_dt(&rotaryEncoder[i],
                                GPIO_INT_DISABLE);
    }
    struct MqttMsg msg;
    strcpy(msg.topic, ROTARY_ENCODER_TOPIC);
        if(pins == BIT(rotaryEncoder[2].pin))
        {
            LOG_INF("language pressed");
            strcpy(msg.msg,"language button pressed");
        }
        else
        {
            int clkA = gpio_pin_get_dt(&rotaryEncoder[0]);
            if((prevclkA == 0) && (clkA == 1))
            {
                int dtB = gpio_pin_get_dt(&rotaryEncoder[1]);
                if(dtB == 1)
                {
                    LOG_INF("language counterclockwise");
                    strcpy(msg.msg, "language counterclockwise");
                }
                else
                {
                    LOG_INF("language clockwise");
                    strcpy(msg.msg,"language clockwise");
                }
            }

            prevclkA = clkA;
        }

            for (int i = 0; i < ARRAY_SIZE(rotaryEncoder); i ++)
            {
            gpio_pin_interrupt_configure_dt(&rotaryEncoder[i],
                                        GPIO_INT_EDGE_BOTH);
            }
    // else if((pins == BIT(rotaryEncoder[3].pin) || (pins == BIT(rotaryEncoder[4].pin)) || (pins == BIT(rotaryEncoder[5].pin))))
    // {
    //     if(pins == BIT(rotaryEncoder[5].pin))
    //     {
    //         LOG_INF("room pressed");
    //         strcpy(msg.msg,"room button pressed");
    //     }
        // else
        // {
        //     int pin8 = gpio_pin_get_dt(&rotaryEncoder[3]);
        //     int pin9 = gpio_pin_get_dt(&rotaryEncoder[4]);
        //     if(prevPin8 != pin8)
        //     {
        //         if(pin9 != prevPin8)
        //         {
        //             LOG_INF("room counterclockwise");
        //             strcpy(msg.msg, "room counterclockwise");
        //         }
        //         else
        //         {
        //             LOG_INF("room clockwise");
        //             strcpy(msg.msg,"room clockwise");
        //         }
        //     }

        //     prevPin8 = pin8;
        // }

    // }
    k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
}
void ConfigDevice:: messageHandler(struct MqttMsg *msg)
{

    if(strcmp(msg->topic, LCD_TOPIC) == 0)
    {
        lcd1->firstLine(msg->msg);
        // lcd2->firstLine(msg->msg);
    }
    else if(strcmp(msg->topic, LCD1_TOPIC) == 0)
    {
        lcd1->firstLine(msg->msg);
    }
    else if(strcmp(msg->topic, LCD2_TOPIC) == 0)
    {
        lcd2->firstLine(msg->msg);
    }
    else
    {
        char log[] = "Command is not recognized.";
        MqttMsg msg = {0};
        strcpy(msg.topic, LOGS_TOPIC);
        strcpy(msg.msg, log);
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        LOG_INF("%s", log);
    }
}

int ConfigDevice:: inputsInit()
{
    int ret;
    uint32_t interruptBits = 0;
    device_init(rotaryEncoder -> port);
    for(unsigned int i = 0; i < ARRAY_SIZE(rotaryEncoder); i++){
        if (!device_is_ready(rotaryEncoder[i].port)) {
		    return -1;
	    }
        ret = gpio_pin_configure_dt(&rotaryEncoder[i], GPIO_INPUT);
	    if (ret < 0) {
		    return -1;
	    }
        ret = gpio_pin_interrupt_configure_dt(&rotaryEncoder[i],
                            GPIO_INT_EDGE_BOTH);
        if (ret != 0) {
            printk("Error %d: failed to configure interrupt on %s pin %d\n",
                ret, rotaryEncoder[0].port->name, rotaryEncoder[i].pin);
            return 0;
        }
        interruptBits |= BIT(rotaryEncoder[i].pin);
    }
    // prevclkA = gpio_pin_get_dt(&rotaryEncoder[0]);
    prevclkA = 0;
    // prevPin8 = gpio_pin_get_dt(&rotaryEncoder[3]);
	gpio_init_callback(&rotaryEncoder_cb_data, rotaryChange, interruptBits);
	gpio_add_callback(rotaryEncoder[0].port, &rotaryEncoder_cb_data); 
	return ret;
}