#include "./enums.h"
#include "LedControl.h"
#include "./config.h"

#include <CAN.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

void displayImage(uint64_t image);
void getObdData();
void getCurrentGear();
void processPid(int pid);
bool isBetween(double currentValue, double min, double max);

uint8_t pids[] = {ENGINE_RPM, VEHICLE_SPEED};

const uint64_t IMAGES[] = {
  0xe7e7e77fffe7ff7f,  // R
  0xe7f7f7ffffefefe7,  // N
  0x38383838383e3c38,  // 1
  0xffff071c70e7ff7e,  // 2
  0x7fffe0ffffe0ff7f,  // 3
  0x707070ffff777777,  // 4
  0x7effe0ff7f07ffff   // 5
};
const int IMAGES_LEN = sizeof(IMAGES)/8;


LedControl display = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1);

Gear current_gear = NEUTRAL;

float rpm = 0;
float kph = 0;
float gear_ratio = 0;


void setup()
{
#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

  display.clearDisplay(0);
  display.shutdown(0, false);
  display.setIntensity(0, 15);

  Serial.begin(115200);
  while (!Serial);

  DEBUG_PRINTLN(F("OBD2 data printer"));

  while (true) {
    DEBUG_PRINT(F("Attempting to connect to OBD2 CAN bus ... "));

    if (!OBD2.begin()) {
      DEBUG_PRINTLN(F("failed!"));

      delay(1000);
    } else {
      DEBUG_PRINTLN(F("success"));
      break;
    }
  }

  DEBUG_PRINTLN();
  DEBUG_PRINT("VIN = ");
  DEBUG_PRINTLN(OBD2.vinRead());
  DEBUG_PRINT("ECU Name = ");
  DEBUG_PRINTLN(OBD2.ecuNameRead());
  DEBUG_PRINTLN();
}

void loop()
{
  for (int pid = 0; pid < sizeof(pids); pid++) {
    processPid(pids[pid]);
  }

  // wait 5 seconds before next run
  delay(100);
  // getObdData();

  if (rpm > 0 && kph > 0){
    gear_ratio = (kph / rpm) * 10000;
    DEBUG_PRINTLN(gear_ratio);
  }
  else if (rpm <= RPM_MIN && kph == 0){
    gear_ratio = 0;
  }

  getCurrentGear();

  displayImage(IMAGES[(int)current_gear]);
}

bool isBetween(float currentValue, float min, float max){
  return (currentValue >=min) && (currentValue <= max);
}

void processPid(int pid) {
  if (!OBD2.pidSupported(pid)) {
    // PID not supported, continue to next one ...
    return;
  }

  // print PID name
  DEBUG_PRINT(OBD2.pidName(pid));
  DEBUG_PRINT(F(" = "));

  if (OBD2.pidValueRaw(pid)) {
    // read the raw PID value
    unsigned long pidRawValue = OBD2.pidReadRaw(pid);

    DEBUG_PRINT(F("0x"));
    DEBUG_PRINT(pidRawValue);
  } else {
    // read the PID value
    float pidValue = OBD2.pidRead(pid);

    if (isnan(pidValue)) {
      DEBUG_PRINT("error");
    } else {
      // print value with units
      if (pid == ENGINE_RPM){
        rpm = pidValue;
      }
      else {
        kph = pidValue;
      }

      DEBUG_PRINT(pidValue);
      DEBUG_PRINT(F(" "));
      DEBUG_PRINT(OBD2.pidUnits(pid));
    }
  }
}


void getObdData(){
  // switch (obd_state)
  // {
  //   case ENG_RPM:
  //   {
  //     rpm = myELM327.rpm();
      
  //     if (myELM327.nb_rx_state == ELM_SUCCESS)
  //     {
  //       obd_state = SPEED;
  //     }
  //     else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
  //     {
  //       //myELM327.printError();
  //       obd_state = SPEED;
  //     }
      
  //     break;
  //   }
    
  //   case SPEED:
  //   {
  //     kph = myELM327.kph();
      
  //     if (myELM327.nb_rx_state == ELM_SUCCESS)
  //     {
  //       obd_state = ENG_RPM;
  //     }
  //     else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
  //     {
  //       //myELM327.printError();
  //       obd_state = ENG_RPM;
  //     }
      
  //     break;
  //   }
  // }
}

void getCurrentGear(){
  if (gear_ratio == 0){
    current_gear = NEUTRAL;
  }
  if (isBetween(gear_ratio, FIRST_MIN, FIRST_MAX)){
    current_gear = FIRST;
  }
  if (isBetween(gear_ratio, SECOND_MIN, SECOND_MAX)){
    current_gear = SECOND;
  }
  if (isBetween(gear_ratio, THIRD_MIN, THIRD_MAX)){
    current_gear = THIRD;
  }
  if (isBetween(gear_ratio, FOURTH_MIN, FOURTH_MAX)){
    current_gear = FOURTH;
  }
  if (isBetween(gear_ratio, FIFTH_MIN, FIFTH_MAX)){
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
