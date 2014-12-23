#include <NeoPixelBus.h>
#include <Task.h>

// delcare taskManager
TaskManager taskManager;

#define CountOf(a) (sizeof(a) / sizeof(a[0]))
// Pin 13 has an LED connected on most Arduino boards.
#define ledPin 13

// general constants
const RgbColor BlackColor = RgbColor(0,0,0);

// global variables
NeoPixelBus strip = NeoPixelBus(4, 2 /*, NEO_RGB */); // four pixels on pin 2

// include effect task definitions
#include "CandleTask.h"
#include "ColorCycleTask.h"
#include "RadioactiveTask.h"

// declare effect tasks
CandleTask candleTask(33);
ColorCycleTask colorCycleTask;
RadioactiveTask radioactiveTask;

// include effect switching task definitions
#include "SwitchEffectTask.h"

// declare switch task and running and stopping timers
SwitchEffectTask switchEffectTask(240000); // 240000 ms = 4 minutes
FunctionTask runningTimer(SwitchToSleep, 18000000); // 18000000 ms = 5 hours
FunctionTask sleepingTimer(SwitchToRunning, 68400000); // 68400000 ms = 19 hours

uint8_t sleepMode; // what sleep mode is used on an individual loop

// Funtions

uint32_t CalculateRandomSeed(uint8_t analogPin = 0)
{
  uint32_t seed = 0;
  
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  // of usefull information, so we call it multiple times and 
  // shift the bits in
  for (int shifts = 0; shifts < 31; shifts += 3)
  {
    seed |= (analogRead(analogPin) & 0x7) << shifts;
    delay(1); // purely to allow the noise to change more
  }
  
  return seed;
}

void setup()
{
  Serial.begin(38400);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    

  strip.Begin();
  strip.Show(); 

  randomSeed(CalculateRandomSeed());

  SwitchToRunning(0);
}
 
void loop()
{
  if (sleepMode == SLEEP_MODE_PWR_DOWN)
  {
    digitalWrite(ledPin, LOW); 
//    Serial.println(" sleep");
//    Serial.flush();
  }
  
  if (taskManager.Loop(sleepMode))
  {
    digitalWrite(ledPin, HIGH); 
//    Serial.println("back to ...");
//    Serial.flush();
  }
}

void SwitchToSleep(uint32_t deltaTimeMs)
{
  Serial.println("going to sleep");
  Serial.flush();

  taskManager.StopTask(&runningTimer);
  taskManager.StopTask(&switchEffectTask);
  taskManager.StartTask(&sleepingTimer);
  sleepMode = SLEEP_MODE_PWR_DOWN;
}

void SwitchToRunning(uint32_t deltaTimeMs)
{
  Serial.println("waking up");
  Serial.flush();

  taskManager.StopTask(&sleepingTimer);
  taskManager.StartTask(&runningTimer);
  taskManager.StartTask(&switchEffectTask);
  sleepMode = SLEEP_MODE_IDLE;
}
 


