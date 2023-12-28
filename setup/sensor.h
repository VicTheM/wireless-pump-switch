/* HEADER FOR THE SENSOR UNIT 9ARDUINO */
#ifndef _SENSOR_H_
#define _SENSOR_H_

// STRUCTURES
/**
 * globals_t - a global structure to hold all global variables
 * @depth: holds depth of empty tank
 * @reset: if true, get depth of empty tank again
 */
struct globals_t{
  int depth;
  bool reset;
};


// MACROS, CONSTANTS AND GLOBALS
#define DEPTH_ADDRESS 0                   // Used to store tank capacity in non-volatile memory
#define GLOBALS_T_INIT {0, false}
#define SAMPLE_LEN 5
#define GLOB_SAMPLE_LEN 10

#define TRIG_PIN 3
#define ECHO_PIN 4
#define INTERRUPT_PIN 2
#define POSITION_PIN 8
#define INDICATOR_PIN 13
#define DATA_OUT_PIN 11
#define BATTERY_PIN A0
#define BLINK_PIN 6

extern globals_t *globals;


//FUNCTIONS
void _setup(int address);
void send(int message);
bool get_depth(int *depth);



#endif /* _SENSOR_H_ */