
#include "rotatingPlatform.h"

LOG_MODULE_REGISTER(rotatingPlatform, LOG_LEVEL_INF);


const struct gpio_dt_spec stepPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rotating_platform_stepper_step), gpios, {0});
const struct gpio_dt_spec directionPin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rotating_platform_stepper_direction), gpios, {0});
const struct gpio_dt_spec enablePin = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rotating_platform_stepper_enable), gpios, {0});
const struct gpio_dt_spec calibrateSwitch = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rotating_platform_calibrate_switch), gpios, {0});
const struct gpio_dt_spec homeSwitch = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rotating_platform_home_switch), gpios, {0});

const struct gpio_dt_spec iStop = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(rotating_platform_interrupt_stop_button), gpios, {0});

static const struct gpio_dt_spec buttons[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(rotating_platform_buttons), gpios, DT_SPEC_AND_COMMA_CONFIG_DEVICE)
};
static const struct gpio_dt_spec relays[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(rotating_platform_relays), gpios, DT_SPEC_AND_COMMA_CONFIG_DEVICE)
};

RotatingPlatform:: RotatingPlatform()
{
    LOG_INF("Rotating Platform Puzzle is selected");

    k_work_init(&calibrationWork, calibrationWorkHandler);
    stepperInit();
    buttonsInit();
    relaysInit();
}

void RotatingPlatform:: homeSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    LOG_INF("Entered Home.");
    RotatingPlatform *instance = CONTAINER_OF(cb, RotatingPlatform, homeSwitch_cb_data);
    instance ->isHome = true;
}

int RotatingPlatform:: homeSwitchInit()
{
    device_init(homeSwitch.port);

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

void RotatingPlatform:: calibrateSwitchIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if(pins == BIT(calibrateSwitch.pin))
    {
        RotatingPlatform *instance = CONTAINER_OF(cb, RotatingPlatform, calibrateSwitch_cb_data);
        static uint32_t prevTime = 0;
        uint32_t currentTime = k_cyc_to_ms_ceil32(arch_k_cycle_get_32());
        if((currentTime - prevTime > 4000) || (prevTime - currentTime > 4000))
        {
            prevTime = currentTime;
            k_work_submit(&instance->calibrationWork);
        }
        prevTime = currentTime;

    }
}

void RotatingPlatform:: calibrationWorkHandler(struct k_work *work) 
{
    // Cast work to the correct type
    RotatingPlatform *instance = CONTAINER_OF(work, RotatingPlatform, calibrationWork);
    // Call the actual calibration function
    instance->calibration(); // Pass appropriate parameters


}


void RotatingPlatform:: calibration()
{
    LOG_INF("Entered calibration");
    goToHome();
    isHome = false;
    //////////////////////////////////
    stepper->move(-25000);

    while (stepper->currentPosition() != -25000) // Full speed up to 300
        stepper->run();
    stepper->stop();

    /////////////////////////////
    // while(!isHome)
    // {
    //     stepper->run();
    // }
    // LOG_INF("current pos before stop in calibration is: %ld", stepper->currentPosition());
    // stepper->stop();
    // LOG_INF("current pos after stop in calibration is: %ld", stepper->currentPosition());
    // long stepsPerRev = stepper->currentPosition();
    // stepsPerDegree = (double)stepsPerRev / 360; 
    // // sem_post(&semCalibrate);
    // calibrated = true;
    // LOG_INF("calibrated. The steps per degree is: %lf", stepsPerDegree);
    // struct MqttMsg msg;
    // strcpy(msg.topic, STATUS_TOPIC);
    // strcpy(msg.msg, "calibrated");
    // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
    
}

void RotatingPlatform:: goToHome()
{
    stepper->move(1000000);
    // // while(!isHome)
    // // {
    // //     stepper->run();
    // // }
    // while(!isHome)
    // {
    //     stepper->run();
    //     if(isHome)
    //         stepper->stop();
    // }
        while (stepper->currentPosition() != 1000000) // Full speed up to 300
        {

            stepper->run();
            if(isHome)
            {
                stepper->stop();
                while(stepper->currentPosition() != stepper->targetPosition())
                    stepper->run();
                break;
            }
        }

    // stepper->stop();
    LOG_INF("current pos before stop is: %ld", stepper->currentPosition());
    stepper->setCurrentPosition(0);
    LOG_INF("Current Position is: %ld", stepper->currentPosition());
}
int RotatingPlatform:: calibrateSwitchInit()
{
    int ret;
    ret = device_init(calibrateSwitch.port);

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


void RotatingPlatform:: goToPosition(int pos)
{
    long position = pos * stepsPerDegree;
    stepper->moveTo(position);
    while (stepper->currentPosition() != position) // Full speed up to 300
        stepper->run();
}
int RotatingPlatform:: stepperInit()
{
    stepper = new AccelStepper(AccelStepper::DRIVER, &stepPin, &directionPin, &directionPin, &directionPin, enablePin); 
    stepper->setMaxSpeed(500000);
    stepper->setAcceleration(1000);
    homeSwitchInit();
    calibrateSwitchInit();
    // calibration();
} 

void RotatingPlatform:: iStopIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    RotatingPlatform *instance = CONTAINER_OF(cb, RotatingPlatform, iStop_cb_data);
    k_work_submit(&instance->iStopWork);
}

void RotatingPlatform:: iStopWorkHandler(struct k_work *work)
{
    RotatingPlatform *instance = CONTAINER_OF(work, RotatingPlatform, iStopWork);
    instance->stepper->stop();
}


int RotatingPlatform:: iStopInit()
{
    device_init(iStop.port);
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


void RotatingPlatform:: buttonsIrqWrapper(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    RotatingPlatform *instance = CONTAINER_OF(cb, RotatingPlatform, buttons_cb_data);
    instance->buttonsIrq(dev, pins);
}

void RotatingPlatform:: buttonsIrq(const struct device *dev, uint32_t pins)
{

}


int RotatingPlatform:: buttonsInit()
{
    int ret = 0;
    uint32_t interruptBits = 0;
    device_init(buttons -> port);

    for(unsigned int i = 0; i < ARRAY_SIZE(buttons); i++){
        if (!device_is_ready(buttons[i].port)) {
		    return -1;
	    }
        ret = gpio_pin_configure_dt(&buttons[i], GPIO_INPUT);
	    if (ret < 0) {
		    return -1;
	    }
        ret = gpio_pin_interrupt_configure_dt(&buttons[i],
                            GPIO_INT_EDGE_FALLING);
        if (ret != 0) {
            printk("Error %d: failed to configure interrupt on %s pin %d\n",
                ret, buttons[0].port->name, buttons[i].pin);
            return -1;
        }
        interruptBits |= BIT(buttons[i].pin);
    }
	gpio_init_callback(&buttons_cb_data, buttonsIrqWrapper, interruptBits);
	gpio_add_callback(buttons[0].port, &buttons_cb_data); 
	return ret;
} 
int RotatingPlatform:: relaysInit()
{
    int ret;
    device_init(relays->port);
    for(unsigned int i = 0; i < ARRAY_SIZE(relays); i++){
        if (!device_is_ready(relays[i].port)) {
		    return -1;
	    }

        ret = gpio_pin_configure_dt(&relays[i], GPIO_OUTPUT);
	    if (ret < 0) {
		    return -1;
	    }
    }
    return 0;
} 

void RotatingPlatform:: messageHandler(MqttMsg *msg)
{
    if(calibrated)
    {
        if(strcmp(msg->topic, SET_STEPPER_POSITION) == 0)
        {
            goToPosition(atoi(msg->msg));
        }
        else if(strcmp(msg->topic, SET_STEPPER_TIME_POSITION) == 0)
        {
            char time[8];
            char pos[8];
            uint8_t commaPos = 0;
            while(*(msg->msg + commaPos) != ',')
            {
                time[commaPos] = *(msg->msg + commaPos);
                commaPos++;
            }
            strcpy(pos, (msg->msg + commaPos) + 1);
            int timeInt = atoi(time);
            int posInt = atoi(pos);
            int accel = (2 * posInt)/(timeInt * timeInt);
            stepper->setAcceleration(accel);
            stepper->moveTo(posInt);
            LOG_INF("The new position is:%ld", stepper->currentPosition());
            // aasd->setPosition(atoi(msg->msg));
        }
        // else if(strcmp(msg->topic, GET_STEPPER_SPEED) == 0)
        // {
            // MqttMsg msg;
            // strcpy(msg.topic, GET_AASD_SPEED);
            // sprintf(msg.msg, "%d", aasd->getSpeed());
            // // itoa(aasd->getSpeed(), msg.msg, 10);
            // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        // }
        // else if(strcmp(msg->topic, GET_STEPPER_POSITION) == 0)
        // {
            // MqttMsg msg;
            // strcpy(msg.topic, GET_AASD_POSITION);
            // sprintf(msg.msg, "%d", aasd->getPosition());
            // // itoa(aasd->getPosition(), msg.msg, 10);
            // k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        // }
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