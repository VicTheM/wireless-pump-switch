#include "sensor.h"
#include <EEPROM.h>
/* Main Entrance into the Sensor unit */

/* where ever you see 'Serial.println()
 * is simply for debugging purposes
 */

globals_t *globals;


void setup()
{
  globals_t my_glob = GLOBALS_T_INIT;
  globals = &my_glob;

  Serial.begin(9600);
  while (!Serial){;} // Wait for serial to begin indeed

  delay(3000);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(DATA_OUT_PIN, OUTPUT);
  pinMode(INDICATOR_PIN, OUTPUT);
  pinMode(BLINK_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(INTERRUPT_PIN, INPUT);
  pinMode(POSITION_PIN, INPUT);
  pinMode(BATTERY_PIN, INPUT);

/* Retrieve height of empty tank or create one and store in EEPROM*/
  _setup(DEPTH_ADDRESS);
}



int water_height = 0;

void loop()
{
  if (globals->reset == true)
    get_depth(NULL);
  get_depth(&water_height);
  send(water_height);
}
