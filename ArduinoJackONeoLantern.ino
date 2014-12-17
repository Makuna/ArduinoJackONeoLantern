#include <NeoPixelBus.h>
#include <Task.h>

#define CountOf(a) (sizeof(a) / sizeof(a[0]))

// general constants
const RgbColor BlackColor = RgbColor(0,0,0);

// global variables
NeoPixelBus strip = NeoPixelBus(4, 2); // four pixels on pin 2

#include "CandleTask.h"
#include "ColorCycleTask.h"
#include "RadioactiveTask.h"

const uint32_t ChangeEffectTimer = 240000; // 4 minutes in milliseconds

TaskManager taskManager;
CandleTask candleTask(33);
ColorCycleTask colorcycleTask;
RadioactiveTask radioactiveTask;

#include "SwitchEffectTask.h"

SwitchEffectTask switchEffectTask(ChangeEffectTimer);
FunctionTask runningTimer(SwitchToSleep, 18000000); // 5 hours
FunctionTask sleepingTimer(SwitchToRunning, 68400000); // 19 hours

void setup()
{
  strip.Begin();
  strip.Show(); 

  SetRandomSeed();
  
  taskManager.StartTask(&switchEffectTask);
  taskManager.StartTask(&runningTimer);
}
 
void loop()
{
  taskManager.Loop(SLEEP_MODE_PWR_DOWN);
}

void SetRandomSeed()
{
  uint32_t seed;
  
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);
  
  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }
  
  randomSeed(seed);
}

void SwitchToSleep(uint32_t deltaTimeMs)
{
  taskManager.StopTask(&runningTimer);
  taskManager.StopTask(&switchEffectTask);
  taskManager.StartTask(&sleepingTimer);
}

void SwitchToRunning(uint32_t deltaTimeMs)
{
  taskManager.StopTask(&sleepingTimer);
  taskManager.StartTask(&runningTimer);
  taskManager.StartTask(&switchEffectTask);
}
 


