#include "sensor.h"
#include <EEPROM.h>

/**
 * _setup - sets up after every power Up
 * Description: gets empty tank depth and
 * save in the EEPROM during first power on
 *
 * For subsequent power on, the depth is not changed
 * NOTE: Data stored in the EEPROM persists, just like hard drive
 *
 * For the once we will produce, we will store '0' in the eeprom before
 * sending out for purchase, then we replace 255 in the code below with '0'
 */
void _setup(int address)
{
  if (EEPROM.read(address) == 255 || EEPROM.read(address) == 0)
  {
    Serial.println("First time power up");
    get_depth(NULL);
  }
  else
  {
    Serial.println("Subsequent power up");
    EEPROM.get(DEPTH_ADDRESS, globals->depth); // subsequent power up
  }
}






/**
 * get_depth - Gets Either depth of Empty tank or water level
 * @depth: address to store depth in;
 *
 * Description: if NULL is passed as parameter, it assumes tank
 *  is empty and obtains the depth of empty tank
 * if the parameter is a valid address, it fills it in with the
 * pulse time, for sending to esp32
 *
 * Return: false for NULL as parameter, else true;
 */
bool get_depth(int *depth)
{
  long int sum;
  int avg;

  avg = sum = 0;
  if (depth == NULL)
  {
    static int arr[GLOB_SAMPLE_LEN];
    for (byte c = 0; c < GLOB_SAMPLE_LEN; c++)
    {
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);

      // measure duration of pulse from ECHO pin
      arr[c] = pulseIn(ECHO_PIN, HIGH);
    }

    bubble_sort(arr, GLOB_SAMPLE_LEN);

    for (byte c = 5; c < GLOB_SAMPLE_LEN - 5; c++)
      sum += arr[c];
    avg = (int)(sum / (GLOB_SAMPLE_LEN - 5 - 5));

    // convert the time to length in cm then write to eeprom
    globals->depth = 0.017 * avg;
    EEPROM.put(DEPTH_ADDRESS, globals->depth);

    return (false);
  }
  else
  {
    sum = 0;
    for (byte c = 0; c < SAMPLE_LEN; c++)
    {
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);

      // measure duration of pulse from ECHO pin
      sum += pulseIn(ECHO_PIN, HIGH);
    }

    *depth = 0.017 * (int)(sum / SAMPLE_LEN);

    return (true);
  }
}







/**
 * send - sends the data through the RF sender
 * @message: for now, we will use int
 */
void send(int message)
{
  Serial.print("height of tank is: ");
  Serial.println(globals->depth);

  Serial.print("height of water is: ");
  Serial.println(message);
  delay(1000);
}







/**
 * bubble_sort - some sort of inefficient sorting algo
 * @arr: array to sort in ascending order
 * @n: size of array
 * We used this function to filter inaccurate
 * readings from the ultrasonic sensor
 */
void bubble_sort(int* arr, int n)
{
  for (int i = 0; i < n-1; i++)
  {
    for (int j = 0; j < n-i-1; j++)
    {
      if (arr[j] > arr[j+1])
      {
        int temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
      }
    }
  }
}





/**
 * isr - Interrupt Service Routine
 * Used to reset the constant tank depth stored in eeprom
 */
void reset()
{
  // This function will be called when the interrupt is triggered (RESET BUTTON)
  Serial.println();
  Serial.println("Re-evaluating Tank Capacity...");
  globals->reset = true;
}
