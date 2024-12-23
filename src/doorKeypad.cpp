#include "doorKeypad.h"
#include <cstring>
LOG_MODULE_REGISTER(doorKeypad, LOG_LEVEL_INF);
static const struct device *const buttons = DEVICE_DT_GET(DT_NODELABEL(door_keypad_buttons));

#define DT_SPEC_AND_COMMA_GATE(node_id, prop, idx) \
 	GPIO_DT_SPEC_GET_BY_IDX(node_id, prop, idx),

const struct gpio_dt_spec display[] = {
    DT_FOREACH_PROP_ELEM(DT_NODELABEL(door_keypad_display), gpios, DT_SPEC_AND_COMMA_GATE)
};
static DoorKeypad *instance = nullptr;

void DoorKeypad:: buttonsHandlerWrapper(struct input_event *val, void *userData)
{
    instance->buttonsHandler(val);
}


void DoorKeypad:: buttonsHandler(struct input_event *val)
{
    if (val->type == INPUT_EV_KEY)
    {
        struct MqttMsg msg = {0};

        if(val->value)
        {
            sprintf(msg.topic, "%s/%s/button%d", roomName, puzzleTypeName, (val->code - INPUT_BTN_0) + 1);
            sprintf(msg.msg, "true");
            LOG_INF("%s, %s", msg.topic, msg.msg);
            k_msgq_put(&msqSendToMQTT, &msg, K_NO_WAIT);
        }
      
    }

}

static const uint8_t digit_segments[128] = {
    0xC0, // 0b11000000 ['0'] 
    0xF9, // 0b11111001 ['1'] 
    0xA4, // 0b10100100 ['2'] 
    0xB0, // 0b10110000 ['3'] 
    0x99, // 0b10011001 ['4'] 
    0x92, // 0b10010010 ['5'] 
    0x82, // 0b10000010 ['6'] 
    0xF8, // 0b11111000 ['7'] 
    0x80, // 0b10000000 ['8'] 
    0x90, // 0b10010000 ['9'] 
    0x88, // 0b10001000 ['A'] 
    0x83, // 0b10000011 ['b'] 
    0xC6, // 0b11000110 ['C'] 
    0xA1, // 0b10100001 ['d'] 
    0x86, // 0b10000110 ['E'] 
    0x8E, // 0b10001110 ['F'] 
    0xFF  // Blank (all off) [' '] 
};
static const uint8_t segment_map[128] = {
    // Control characters (0–31)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x00–0x07
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x08–0x0F
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x10–0x17
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x18–0x1F

    // Printable characters (32–127)
    0xFF,                   // 0x20 (Space)
    0xFF, 0xFF, 0xFF, 0xFF, // 0x21–0x24
    0xFF, 0xFF, 0xFF, 0xFF, // 0x25–0x28
    0xFF, 0xFF, 0xFF, 0xFF, // 0x29–0x2C
    0xFF, 0xFF, 0xFF, 0xFF, // 0x2D–0x30

    // Numeric digits (0–9, ASCII 48–57)
    0xC0, // 0x30 (0) => A, B, C, D, E, F
    0xF9, // 0x31 (1) => B, C
    0xA4, // 0x32 (2) => A, B, D, E, G
    0xB0, // 0x33 (3) => A, B, C, D, G
    0x99, // 0x34 (4) => B, C, F, G
    0x92, // 0x35 (5) => A, C, D, F, G
    0x82, // 0x36 (6) => A, C, D, E, F, G
    0xF8, // 0x37 (7) => A, B, C
    0x80, // 0x38 (8) => A, B, C, D, E, F, G
    0x90, // 0x39 (9) => A, B, C, D, F, G

    // Characters 'A'–'Z', ASCII 65–90
    0x88, // 0x41 (A) => A, B, C, E, F, G
    0x83, // 0x42 (b) => C, D, E, F, G
    0xC6, // 0x43 (C) => A, D, E, F
    0xA1, // 0x44 (d) => B, C, D, E, G
    0x86, // 0x45 (E) => A, D, E, F, G
    0x8E, // 0x46 (F) => A, E, F, G
    0xFF, // 0x47 (G) => Unsupported
    0xFF, // Remaining unsupported letters
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

    // Lowercase letters 'a'–'z', ASCII 97–122
    0x88, // 0x61 (a) => Same as A
    0x83, // 0x62 (b) => Same as b
    0xC6, // 0x63 (c) => Same as C
    0xA1, // 0x64 (d) => Same as d
    0x86, // 0x65 (e) => Same as E
    0x8E, // 0x66 (f) => Same as F
    0xFF, // Remaining unsupported letters
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

    // Remaining characters (symbols, etc.)
    0xFF, 0xFF, 0xFF, 0xFF, // Unsupported
    0xFF, 0xFF, 0xFF, 0xFF, // Unsupported
    0xFF, 0xFF, 0xFF, 0xFF, // Unsupported
    0xFF, 0xFF, 0xFF, 0xFF  // Unsupported
};
// Function to set GPIO pins based on value
void set_pins(const struct gpio_dt_spec *display, uint8_t value, uint8_t start_index, uint8_t pin_count) {
    for (uint8_t i = 0; i < pin_count; i++) {
        bool pin_state = (value & (1 << i)) != 0;
        gpio_pin_set(display[i + start_index].port, display[i + start_index].pin, pin_state);
    }
}

// Function to display a string
void display_string(const struct gpio_dt_spec *display, const char *str) {
    uint8_t buffer[8] = {0}; // Store segment data for each digit
    size_t len = strlen(str);

    // Map characters to segment data
    for (uint8_t i = 0; i < 8; i++) {
        char c = (i < len) ? str[i] : ' '; // Pad with blanks if string is shorter
        buffer[i] = segment_map[c];
    }

    // Cycle through each digit and display it
    for (uint8_t digit = 0; digit < 8; digit++) {
        // Disable all digits (active high)
        set_pins(display, 0x00, 0, 8);

        // Set segment data for the current digit (active low)
        set_pins(display, buffer[digit], 8, 8);

        // Enable the current digit (active high)
        set_pins(display, 1 << digit, 0, 8);

        // Delay to stabilize display
        k_msleep(1);
    }

    // Turn off all digits at the end of the cycle
    set_pins(display, 0x00, 0, 8);
}
DoorKeypad:: DoorKeypad(const char * room, const char *type): Puzzle(room, type)
{
    LOG_INF("%s/%s", room, type);
    LOG_INF("%s/%s", roomName, puzzleTypeName);
    creatingMqttList(1);
    device_init(buttons);
    instance = this;
    INPUT_CALLBACK_DEFINE(buttons, buttonsHandlerWrapper, (void*)this);
    device_init(DEVICE_DT_GET(DT_NODELABEL(spi1)));
    device_init(DEVICE_DT_GET(DT_NODELABEL(sevensegment)));

    int ret;
    for(unsigned int i = 0; i < ARRAY_SIZE(display); i++){
        if (!device_is_ready(display[i].port)) {
		    // return -1;
	    }
        ret = gpio_pin_configure_dt(&display[i], GPIO_OUTPUT_INACTIVE);
	    if (ret < 0) {
		    // return -1;
	    }
    }

	    const char *message = "ALLAHO12";
    while (1) {
        display_string(display, message);
        k_msleep(500); // Update every 500 ms
    }


}

void DoorKeypad:: creatingMqttList(uint16_t _mqttCount)
{
    char topic[128] = {0};
    sprintf(topic, "%s/%s/display", roomName, puzzleTypeName);
    mqttList[0] = *createMqttTopic(topic);
    mqttCount = _mqttCount;
}
void DoorKeypad:: messageHandler(struct MqttMsg *msg)
{
    LOG_INF("Command received: topic: %s, msg: %s",msg->topic, msg->msg);
    char command[16] = {0};
    int ret = validTopic(msg->topic, command);
    if(!ret)
    {
        char field[] = "display";
        if((strcmp(field, command) == 0))
        {
            	while(1)
	    {
        for(uint8_t i = 0; i < 8; i++)
        {
        gpio_pin_set_dt(&display[8 + i], (digit_segments[1]>> i) &0x01);
        }
        LOG_INF("Entered while loop");
        gpio_pin_set_dt(&display[0], 0);
        gpio_pin_set_dt(&display[1], 0);
        gpio_pin_set_dt(&display[2], 0);
        gpio_pin_set_dt(&display[3], 0);
        gpio_pin_set_dt(&display[4], 1);
        gpio_pin_set_dt(&display[5], 1);
        gpio_pin_set_dt(&display[6], 1);
        gpio_pin_set_dt(&display[7], 1);
        // gpio_pin_set_dt(&display[9], 1);
        // gpio_pin_set_dt(&display[10], 1);
        // gpio_pin_set_dt(&display[11], 1);
        // gpio_pin_set_dt(&display[12], 1);
        // gpio_pin_set_dt(&display[13], 1);
        // gpio_pin_set_dt(&display[14], 1);
        // gpio_pin_set_dt(&display[15], 1);
        k_msleep(100);
        // gpio_pin_set_dt(&display[0], 0);
        // gpio_pin_set_dt(&display[1], 0);
        // gpio_pin_set_dt(&display[2], 0);
        // gpio_pin_set_dt(&display[3], 0);
        // gpio_pin_set_dt(&display[4], 0);
        // gpio_pin_set_dt(&display[5], 0);
        // gpio_pin_set_dt(&display[6], 0);
        // gpio_pin_set_dt(&display[7], 0);
        // gpio_pin_set_dt(&display[8], 0);
        // gpio_pin_set_dt(&display[9], 0);
        // gpio_pin_set_dt(&display[10], 0);
        // gpio_pin_set_dt(&display[11], 0);
        // gpio_pin_set_dt(&display[12], 0);
        // gpio_pin_set_dt(&display[13], 0);
        // gpio_pin_set_dt(&display[14], 0);
        // gpio_pin_set_dt(&display[15], 0);
        // k_usleep(1);
    }
        }
        else
        {
            LOG_ERR("Not a valid index");
        }
    }
}