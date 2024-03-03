#include "Arduino.h"

// display settings
#define CLK_PIN   13  // or SCK
#define DATA_PIN  14  // or MOSI
#define CS_PIN    12  // or SS

// OBD-II adapter address
uint8_t address[6] = {0x00, 0x10, 0xCC, 0x4F, 0x36, 0x03};

// car Idle rpm
#define RPM_MIN 850

// set gears min and max
#define FIRST_MIN 1
#define FIRST_MAX 100

#define SECOND_MIN 100
#define SECOND_MAX 180

#define THIRD_MIN 180
#define THIRD_MAX 270

#define FOURTH_MIN 270
#define FOURTH_MAX 370

#define FIFTH_MIN 370
#define FIFTH_MAX 1000