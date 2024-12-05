
#include "platform.h"

LOG_MODULE_REGISTER(platform, LOG_LEVEL_INF);

static Platform *instance2 = nullptr;

const struct gpio_dt_spec stepPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(platform_stepper_step), gpios, {0});
const struct gpio_dt_spec directionPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(platform_stepper_direction), gpios, {0});
const struct gpio_dt_spec enablePin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(platform_stepper_enable), gpios, {0});
const struct gpio_dt_spec calibrateSwitch = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(platform_calibrate_switch), gpios, {0});
const struct gpio_dt_spec homeSwitch = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(platform_home_switch), gpios, {0});

const struct gpio_dt_spec iStop = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(platform_interrupt_stop_button), gpios, {0});

static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(platform_buttons));
// static const struct gpio_dt_spec relays[] = {
//     DT_FOREACH_PROP_ELEM(DT_NODELABEL(rotating_platform_relays), gpios, DT_SPEC_AND_COMMA_CONFIG_DEVICE)
// };

void Platform:: buttonsHandlerWrapper(struct input_event *val, void* userData)
{
   instance2->buttonsHandler(val);
}


void Platform:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        if(val->value)
        {
            struct MqttMsg msg = {0};
            sprintf(msg.topic, "%s/%s/button%d", roomName, puzzleTypeName, val->code - INPUT_BTN_0);
            sprintf(msg.msg, "true");
            LOG_INF("button %d is pressed", val->code - INPUT_BTN_0);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
        /////////////////////////////////end time buttons
    }
}

Platform:: Platform(const char * room, const char *type): Puzzle(room, type)
{
    LOG_INF("Platform Puzzle is selected");

    stepperInit();
    // relaysInit();
    k_work_init(&calibrationWork, calibrationWorkHandler);
    // device_init(relays->port);
    creatingMqttList(1);
    device_init(buttons);
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void *)this);
}

void Platform:: creatingMqttList(uint16_t _mqttCount)
{
    mqttList[0] = introRoom_platform_position_topic;
    mqttCount = _mqttCount;
}

void Platform:: homeSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    LOG_INF("Entered Home.");
    Platform *instance = CONTAINER_OF(cb, Platform, homeSwitch_cb_data);
    instance ->isHome = true;
}

int Platform:: homeSwitchInit()
{
    // device_init(homeSwitch.port);

    int ret;
    if (!device_is_ready(homeSwitch.port)) {
        return -1;
    }
    ret = gpio_pin_configure_dt(&homeSwitch, GPIO_INPUT);
    if (ret < 0) {
        return -1;
    }
    ret = gpio_pin_interrupt_configure_dt(&homeSwitch,
                        GPIO_INT_EDGE_RISING);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
            ret, homeSwitch.port->name, homeSwitch.pin);
        return 0;
    }
    
	gpio_init_callback(&homeSwitch_cb_data, homeSwitchIrqWrapper, BIT(homeSwitch.pin));
	gpio_add_callback(homeSwitch.port, &homeSwitch_cb_data); 
	return ret;

}

void Platform:: calibrateSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    Platform *instance = CONTAINER_OF(cb, Platform, calibrateSwitch_cb_data);
    static uint32_t prevTime = 0;
    uint32_t currentTime = k_cyc_to_ms_ceil32(arch_k_cycle_get_32());
    if((currentTime - prevTime > 1000) || (prevTime - currentTime > 1000))
    {
        prevTime = currentTime;
        k_work_submit(&instance->calibrationWork);
    }
    prevTime = currentTime;

}

void Platform:: calibrationWorkHandler(struct k_work *work) 
{
    // Cast work to the correct type
    Platform *instance = CONTAINER_OF(work, Platform, calibrationWork);
    // Call the actual calibration function
    instance->calibration(); // Pass appropriate parameters


}

void Platform:: calibration()
{
    static uint8_t numIn = 0;
    numIn++;
    if(numIn % 2)
    {
        LOG_INF("Entered calibration");

        goToHome();
        isHome = false;
        calibrated = true;

        stepper->setAcceleration(500);
        goToStartPos();

        stepper->setCurrentPosition(0);

        numIn = 1;
        calibrated = true;
        LOG_INF("calibrated. The steps per degree is: %lf", stepsPerDegree);
        struct MqttMsg msg;
        // strcpy(msg.topic, "%s/%s/calibrated", roomName, puzzleTypeName);
        strcpy(msg.msg, "true");
        k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        // stepsPerDegree = 350000/360;
        // stepsPerDegree = (double)stepsPerRev / 360; 
    }

    //////////////////////////////////

    /////////////////////////////
    // while(!isHome)
    // {
    //     stepper->run();
    // }
    // LOG_INF("current pos before stop in calibration is: %ld", stepper->currentPosition());
    // stepper->stop();
    // LOG_INF("current pos after stop in calibration is: %ld", stepper->currentPosition());
    // long stepsPerRev = stepper->currentPosition();
    
}

void Platform:: goToStartPos()
{
    stepper->move(startPos);

    while (stepper->currentPosition() != startPos) // Full speed up to 300
        stepper->run();
    stepper->stop();

}

void Platform:: goToHome()
{
    stepper->move(1000000);
    while(!isHome)
    {
        stepper->run();
        if(isHome)
        {
            stepper->setAcceleration(6000);
            stepper->stop();
            while(stepper->currentPosition() != stepper->targetPosition())
                stepper->run();
            break;
        }
    }
    LOG_INF("current pos before stop is: %ld", stepper->currentPosition());
    stepper->setCurrentPosition(0);
    LOG_INF("Current Position is: %ld", stepper->currentPosition());
}
int Platform:: calibrateSwitchInit()
{
    int ret;
    // ret = device_init(calibrateSwitch.port);

    if (!device_is_ready(calibrateSwitch.port)) {
        return -1;
    }
    ret = gpio_pin_configure_dt(&calibrateSwitch, GPIO_INPUT);
    if (ret < 0) {
        return -1;
    }
    ret = gpio_pin_interrupt_configure_dt(&calibrateSwitch,
                        GPIO_INT_EDGE_FALLING);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
            ret, calibrateSwitch.port->name, calibrateSwitch.pin);
        return 0;
    }
    
	gpio_init_callback(&calibrateSwitch_cb_data, calibrateSwitchIrqWrapper, BIT(calibrateSwitch.pin));
	gpio_add_callback(calibrateSwitch.port, &calibrateSwitch_cb_data); 
	return ret;

}


void Platform:: goToPosition(int position)
{
    struct MqttMsg msg = {0};
    // long position = pos * stepsPerDegree;



    stepper->setAcceleration(500);





    stepper->moveTo(position);
    LOG_DBG("current position is: %ld, target is: %ld", stepper->currentPosition(), stepper->targetPosition());
    while ((stepper->currentPosition() != position)) // Full speed up to 300
    {
        // if(k_msgq_get(&msqReceivedFromMQTT, &msg, K_NO_WAIT))
        // {
        //     if(strcmp(msg.topic, SET_STEPPER_STOP) == 0)
        //     {
        //         stepper->stop();
        //         break;
        //     }
        //     else
        //     {
        //         k_msgq_put(&msqReceivedFromMQTT, &msg, K_NO_WAIT);
        //     }
        // }
        stepper->run();
    }
    LOG_INF("moving finished");
}
int Platform:: stepperInit()
{
    stepper = new AccelStepper(AccelStepper::DRIVER, &stepPin, &directionPin, &directionPin, &directionPin, enablePin); 
    stepper->setMaxSpeed(5000);
    stepper->setAcceleration(500);
    homeSwitchInit();
    calibrateSwitchInit();
    // calibration();
} 

void Platform:: iStopIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    Platform *instance = CONTAINER_OF(cb, Platform, iStop_cb_data);
    k_work_submit(&instance->iStopWork);
}

void Platform:: iStopWorkHandler(struct k_work *work)
{
    Platform *instance = CONTAINER_OF(work, Platform, iStopWork);
    instance->stepper->stop();
}


int Platform:: iStopInit()
{
    // device_init(iStop.port);
    int ret;
    if (!device_is_ready(iStop.port)) {
        return -1;
    }
    ret = gpio_pin_configure_dt(&iStop, GPIO_INPUT);
    if (ret < 0) {
        return -1;
    }
    ret = gpio_pin_interrupt_configure_dt(&iStop, GPIO_INT_EDGE_FALLING);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
            ret, iStop.port->name, iStop.pin);
        return -1;
    }
	gpio_init_callback(&iStop_cb_data, iStopIrqWrapper, BIT(iStop.pin));
	gpio_add_callback(iStop.port, &iStop_cb_data); 
    return ret;
}



// int Platform:: relaysInit()
// {
//     int ret;
//     // device_init(relays->port);
//     for(unsigned int i = 0; i < ARRAY_SIZE(relays); i++){
//         if (!device_is_ready(relays[i].port)) {
// 		    return -1;
// 	    }

//         ret = gpio_pin_configure_dt(&relays[i], GPIO_OUTPUT);
// 	    if (ret < 0) {
// 		    return -1;
// 	    }
//     }
//     return 0;
// } 

void Platform:: messageHandler(MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    if(calibrated)
    {
        if(strcmp(msg->topic, INTRO_ROOM_PLATFORM_POSITION_TOPIC) == 0)
        {
            if(strcmp(msg->msg, "back") == 0)
            {
                goToPosition(-152500);
                // goToPosition(-305000);
            }
            else if(strcmp(msg->msg, "front") == 0)
            {
                goToPosition(0);
            }
            else
            {
                LOG_INF("The position is not valid");
            }
        }
        else
        {
            LOG_INF("The command is not valid.");
        }

    }
    else
    {
        LOG_INF("The device is not calibrated.");
    }

}


void Platform:: stop()
{
    stepper->stop();
    while(stepper->currentPosition() != stepper->targetPosition())
        stepper->run();
}