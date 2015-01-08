#include <NeoPixelBus.h>
#include <Task.h>
#include <RandomSeed.h>
#include <avr/power.h>

// delcare taskManager
TaskManager taskManager;

// handy macro that calculates the count of elements in an array
#define countof(a) (sizeof(a) / sizeof(a[0]))

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
SwitchEffectTask switchEffectTask(MsToTaskTime(240000)); // 240000 ms = 4 minutes
FunctionTask runningTimer(SwitchToSleep, MsToTaskTime(18000000)); // 18000000 ms = 5 hours
FunctionTask sleepingTimer(SwitchToRunning, MsToTaskTime(68400000)); // 68400000 ms = 19 hours

// Functions

void setup()
{
  Serial.begin(57600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    

  strip.Begin();
  strip.Show(); 

  // must be done before the power saving tricks below
  randomSeed(GenerateRandomSeed());

  // power saving tricks
  ADCSRA = 0; // disable ADC
  power_all_disable();
  power_timer0_enable(); // used for millis()
  power_usart0_enable(); // used for Serial

  SwitchToRunning(0);
}
 
void loop()
{
  taskManager.Loop();
}

void SwitchToSleep(uint32_t deltaTime)
{
  Serial.println("going to sleep");
  Serial.flush();

  taskManager.StopTask(&runningTimer);
  taskManager.StopTask(&switchEffectTask);
  taskManager.StartTask(&sleepingTimer);
}

void SwitchToRunning(uint32_t deltaTime)
{
  Serial.println("waking up");
  Serial.flush();

  taskManager.StopTask(&sleepingTimer);
  taskManager.StartTask(&runningTimer);
  taskManager.StartTask(&switchEffectTask);
}
 


