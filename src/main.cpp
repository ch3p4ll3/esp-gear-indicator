#include "BluetoothSerial.h"
#include "ELMduino.h"
#include "./enums.h"
#include "LedControl.h"
#include "./config.h"


BluetoothSerial SerialBT;
#define ELM_PORT   SerialBT
#define DEBUG_PORT Serial


const bool DEBUG = false;
const int  TIMEOUT = 2000;

void displayImage(uint64_t image);
void getObdData();
void getCurrentGear();
bool isBetween(double currentValue, double min, double max);

ELM327 myELM327;

const uint64_t IMAGES[] = {
  0xe7e7e77fffe7ff7f,
  0xe7f7f7ffffefefe7,
  0x38383838383e3c38,
  0xffff071c70e7ff7e,
  0x7fffe0ffffe0ff7f,
  0x707070ffff777777,
  0x7effe0ff7f07ffff
};
const int IMAGES_LEN = sizeof(IMAGES)/8;


LedControl display = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1);

Gear current_gear = NEUTRAL;
PID_STATES obd_state = ENG_RPM;

float rpm = 0;
float kph = 0;
float gear_rateo = 0;


void setup()
{
#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

  display.clearDisplay(0);
  display.shutdown(0, false);
  display.setIntensity(0, 10);

  DEBUG_PORT.begin(115200);
  //SerialBT.setPin("1234");
  ELM_PORT.begin("ArduHUD", true);
  
  if (!ELM_PORT.connect(address))
  {
    DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
    while(1);
  }

  if (!myELM327.begin(ELM_PORT, DEBUG, TIMEOUT))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  Serial.println("Connected to ELM327");
}

void loop()
{
  getObdData();

  if (rpm > 0 && kph > 0){
    gear_rateo = (kph / rpm) * 10000;
    Serial.println(gear_rateo);
  }
  else if (rpm <= RPM_MIN && kph == 0){
    gear_rateo = 0;
  }

  getCurrentGear();

  displayImage(IMAGES[(int)current_gear]);
}

bool isBetween(float currentValue, float min, float max){
  return (currentValue >=min) && (currentValue <= max);
}

void getObdData(){
  switch (obd_state)
  {
    case ENG_RPM:
    {
      rpm = myELM327.rpm();
      
      if (myELM327.nb_rx_state == ELM_SUCCESS)
      {
        obd_state = SPEED;
      }
      else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
      {
        //myELM327.printError();
        obd_state = SPEED;
      }
      
      break;
    }
    
    case SPEED:
    {
      kph = myELM327.kph();
      
      if (myELM327.nb_rx_state == ELM_SUCCESS)
      {
        obd_state = ENG_RPM;
      }
      else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
      {
        //myELM327.printError();
        obd_state = ENG_RPM;
      }
      
      break;
    }
  }
}

void getCurrentGear(){
  if (gear_rateo == 0){
    current_gear = NEUTRAL;
  }
  if (isBetween(gear_rateo, FIRST_MIN, FIRST_MAX)){
    current_gear = FIRST;
  }
  if (isBetween(gear_rateo, SECOND_MIN, SECOND_MAX)){
    current_gear = SECOND;
  }
  if (isBetween(gear_rateo, THIRD_MIN, THIRD_MAX)){
    current_gear = THIRD;
  }
  if (isBetween(gear_rateo, FOURTH_MIN, FOURTH_MAX)){
    current_gear = FOURTH;
  }
  if (isBetween(gear_rateo, FIFTH_MIN, FIFTH_MAX)){
    current_gear = FIFTH;
  }
}

void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(row, j));
    }
  }
}
