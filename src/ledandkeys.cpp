#include "ledandkeys.h"


LOG_MODULE_REGISTER(ledandkeys, LOG_LEVEL_INF);

K_THREAD_STACK_DEFINE(ledandkeysStackArea, LED_AND_KEYS_STACK_SIZE);

struct k_thread ledandkeysThread;

uint8_t segment_vals[8]={0};   
uint8_t seg1_val=0;

const struct gpio_dt_spec clkpin = GPIO_DT_SPEC_GET(DT_NODELABEL(tmclk), gpios);
const struct gpio_dt_spec diopin = GPIO_DT_SPEC_GET(DT_NODELABEL(tmdo), gpios);
const struct gpio_dt_spec stbpin = GPIO_DT_SPEC_GET(DT_NODELABEL(tmstb), gpios);
u_int8_t led1_val; 
u_int8_t led2_val; 
u_int8_t led3_val; 
u_int8_t led4_val; 
u_int8_t led5_val; 
u_int8_t led6_val; 
u_int8_t led7_val; 
u_int8_t led8_val; 


ledandkeys::ledandkeys(struct k_msgq *_queue)
{
      queue = _queue;
      int val1;
      int val2;
      _pulse = PULSE1_16;
      _isOn = false;
      _clk_pin = &clkpin;
      _stb_pin = &stbpin;
      _dio_pin = &diopin;

      ledandkeysinit();

      pinMode(_stb_pin, OUTPUT);
      pinMode(_clk_pin, OUTPUT);
      pinMode(_dio_pin, OUTPUT);
      digitalWrite(_stb_pin, HIGH);
      digitalWrite(_clk_pin, HIGH);
      digitalWrite(_dio_pin, HIGH);
      
      reset();    
      //test();
      uint8_t buttons;
      struct MqttMsg *msg = (struct MqttMsg *)k_malloc(sizeof(struct MqttMsg));
      memset(msg, 0, sizeof(struct MqttMsg));
      while(1)
      {

        if(k_msgq_get(queue, msg, K_NO_WAIT) == 0) {
          messageHandler(msg);
        }  
        buttons = getButtons();
        increase_segments(buttons);
        //LOG_INF("button value is %d", buttons);
        k_msleep(800);
      }
}

void ledandkeys:: ledandkeysinit()
{

  device_init(clkpin.port);
	if (!gpio_is_ready_dt(&clkpin)) {
		printk("Error: button device %s is not ready\n",
		       clkpin.port->name);
	}

  device_init(stbpin.port);
	if (!gpio_is_ready_dt(&stbpin)) {
		printk("Error: button device %s is not ready\n",
		       stbpin.port->name);
	}


  device_init(diopin.port);
	if (!gpio_is_ready_dt(&diopin)) {
		printk("Error: button device %s is not ready\n",
		       diopin.port->name);
	}

}

void ledandkeys::increase_segments(uint8_t button_val){
    switch(button_val){
      case 1:
        displayVal(7, segment_vals[0]);
        send_7seg_val(segment_vals[0], SEG1_TOPIC);
        if(segment_vals[0]<15)
          segment_vals[0]++;
        else
          segment_vals[0]=0;
        break;
      case 2:
        displayVal(6, segment_vals[1]);
        send_7seg_val(segment_vals[1], SEG2_TOPIC);
        if(segment_vals[1]<15)
          segment_vals[1]++;
        else
          segment_vals[1]=0;
        break;
      case 4:
        displayVal(5, segment_vals[2]);
        send_7seg_val(segment_vals[2], SEG3_TOPIC);
        if(segment_vals[2]<15)
          segment_vals[2]++;
        else
          segment_vals[2]=0;
        break;
      case 8:
        displayVal(4, segment_vals[3]);
        send_7seg_val(segment_vals[3], SEG4_TOPIC);
        if(segment_vals[3]<15)
          segment_vals[3]++;
        else
          segment_vals[3]=0;
        break;
      case 16:
        displayVal(3, segment_vals[4]);
        send_7seg_val(segment_vals[4], SEG5_TOPIC);        
        if(segment_vals[4]<15)
          segment_vals[4]++;
        else
          segment_vals[4]=0;
        break;
      case 32:
        displayVal(2, segment_vals[5]);
        send_7seg_val(segment_vals[5], SEG6_TOPIC);        
        if(segment_vals[5]<15)
          segment_vals[5]++;
        else
          segment_vals[5]=0;
        break;
      case 64:
        displayVal(1, segment_vals[6]);
        send_7seg_val(segment_vals[6], SEG7_TOPIC);        
        if(segment_vals[6]<15)
          segment_vals[6]++;
        else
          segment_vals[6]=0;
        break;
      case 128:
        displayVal(0, segment_vals[7]);
        send_7seg_val(segment_vals[7], SEG8_TOPIC);        
        if(segment_vals[7]<15)
          segment_vals[7]++;
        else
          segment_vals[7]=0;                                      
      default:
        break;
    }
}

void ledandkeys::send_7seg_val(uint8_t segment_val, char pub_topic[30]){
    char segment_val_char[2];
    sprintf(segment_val_char,"%d",segment_val);
    //LOG_INF("The 7 segement value is %d\n\r",segment_val);
    struct MqttMsg send = {0};
    strcpy(send.topic, pub_topic);
    strcpy(send.msg, segment_val_char);
    k_msgq_put(&msqSendToMQTT, &send, K_NO_WAIT);
}

uint8_t ledandkeys::check_zero_one(char val_char){
    if(val_char=='0')
      return 0;
    else if(val_char=='1')
      return 1;
    else
      return 0;
}

void ledandkeys:: messageHandler(struct MqttMsg *msg)
{   
    uint8_t led_val;
    if(strcmp(msg->topic, LED1_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val);
    }
    else if(strcmp(msg->topic, LED2_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val<<1);
    }
    else if(strcmp(msg->topic, LED3_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val<<2);
    }
    else if(strcmp(msg->topic, LED4_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val<<3);
    }
    else if(strcmp(msg->topic, LED5_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val<<4);
    }
    else if(strcmp(msg->topic, LED6_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val<<5);
    }                    
    else if(strcmp(msg->topic, LED7_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val<<6);
    }
    else if(strcmp(msg->topic, LED8_TOPIC) == 0)
    {
      led_val = check_zero_one(msg->msg[0]);
      writeLeds(led_val<<7);
    }    
}

bool ledandkeys::getButton(button_t s){
  _buttons = getButtons();
  return bit_read(_buttons, s);
}

// buttons K3/KS1-8
uint8_t ledandkeys::getButtons(){
  digitalWrite(_stb_pin, LOW);
  writeData(INSTRUCTION_READ_KEY);
  //Twait 1µs
  pinMode(_dio_pin, INPUT);
  digitalWrite(_clk_pin, LOW);
  uint8_t data[4];
  for(uint8_t i=0; i<sizeof(data);i++){
    data[i] = shiftIn(LSBFIRST);
    delayMicroseconds(1);
  }
  pinMode(_dio_pin, OUTPUT);
  digitalWrite(_stb_pin, HIGH);
  _buttons=0;
  for (uint8_t i=0; i<4;i++){
    _buttons |= data[i]<<i;
  }
  return _buttons;
}

void ledandkeys::reset(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_AUTO);
  digitalWrite(_stb_pin, LOW);
  writeData(FIRST_DISPLAY_ADDRESS);
  for(uint8_t i=0;i<16;i++)
    writeData(0);
  digitalWrite(_stb_pin, HIGH);
}

void ledandkeys::displayVal(uint8_t digitId, uint8_t val){
  if ((digitId>7) | (val>15) | (val<0)) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS+14-(digitId*2), _digits[val]);
}

void ledandkeys::displayDig(uint8_t digitId, uint8_t pgfedcba){
  if (digitId>7) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS+14-(digitId*2), pgfedcba);
}

void ledandkeys::displayClear(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  for (uint8_t i=0;i<15;i+=2){
    writeDataAt(FIRST_DISPLAY_ADDRESS+i,0x00);
  }
}

void ledandkeys::writeLed(uint8_t num,bool state){
  if ((num<1) | (num>8)) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS + (num*2-1), state);
}

void ledandkeys::writeLeds(uint8_t val){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  for(uint8_t i=1;i<9;i++){
    writeDataAt(FIRST_DISPLAY_ADDRESS + (i*2-1), val & 0x01);
    val >>= 1; 
  }
}

void ledandkeys::displayTurnOn(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  _isOn = true;
}

void ledandkeys::displayTurnOff(){
  setDisplayMode(DISPLAY_TURN_OFF | _pulse);
  _isOn = false;
}

void ledandkeys::displaySetBrightness(pulse_t newpulse){
  if ((newpulse<PULSE1_16) | (newpulse>PULSE14_16)) return;
  _pulse = newpulse;
  uint8_t data = (_isOn) ? DISPLAY_TURN_ON : DISPLAY_TURN_OFF;
  data |= _pulse;
  setDisplayMode(data);
}

void ledandkeys::writeData(uint8_t data){
  shiftOut(LSBFIRST,data);
} 

void ledandkeys::writeDataAt(uint8_t displayAddress, uint8_t data){
    digitalWrite(_stb_pin, LOW);
    writeData(displayAddress);
    writeData(data);
    digitalWrite(_stb_pin, HIGH);
    delayMicroseconds(1);
}

void ledandkeys::setDisplayMode(uint8_t displayMode){
  digitalWrite(_stb_pin, LOW);
  writeData(displayMode);
  digitalWrite(_stb_pin, HIGH);
  delayMicroseconds(1);
}
void ledandkeys::setDataInstruction(uint8_t dataInstruction){
  digitalWrite(_stb_pin, LOW);
  writeData(dataInstruction);
  digitalWrite(_stb_pin, HIGH);
  delayMicroseconds(1);  
}

void ledandkeys::test(){
  uint8_t val=0;
  for(uint8_t i=0;i<5;i++){
    setDisplayMode(DISPLAY_TURN_ON | _pulse);
    setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_AUTO);
    digitalWrite(_stb_pin, LOW);
    writeData(FIRST_DISPLAY_ADDRESS);
    for(uint8_t i=0;i<16;i++)
      writeData(val);
    digitalWrite(_stb_pin, HIGH);
    delay(1000);
    val = ~val;
  }

}

uint8_t ledandkeys::bit_read(uint32_t value, uint8_t bit) {
    return (value >> bit) & 0x01;
}

uint8_t ledandkeys::shiftIn(uint8_t bitOrder)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        digitalWrite(_clk_pin, HIGH);
        if (bitOrder == LSBFIRST)
            value |= digitalRead(_dio_pin) << i;
        else
            value |= digitalRead(_dio_pin) << (7 - i);
        digitalWrite(_clk_pin, LOW);
    }
    return value;
}

void ledandkeys::shiftOut(uint8_t bitOrder, uint8_t val) {
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST)
            digitalWrite(_dio_pin, !!(val & (1 << i)));
        else    
            digitalWrite(_dio_pin, !!(val & (1 << (7 - i))));
        
        digitalWrite(_clk_pin, HIGH);
        digitalWrite(_clk_pin, LOW);        
    }
}


void ledandkeysEntryPoint(void *, void *, void *)
{

    ledandkeys *ledandkey = new ledandkeys(&msqLedandKey);

}


void ledandkeysThreadCreate()
{
    k_tid_t ledandkeysTid = k_thread_create(&ledandkeysThread, ledandkeysStackArea,
									K_THREAD_STACK_SIZEOF(ledandkeysStackArea),
									ledandkeysEntryPoint, NULL, NULL, NULL,
									LED_AND_KEYS_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(ledandkeysTid, "ledandkeys");
}
