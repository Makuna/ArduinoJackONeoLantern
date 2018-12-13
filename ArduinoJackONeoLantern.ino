#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <Task.h>
#include <avr/power.h>
#include <limits.h>

// define state for animation handling
struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
    bool IsGettingBrighter;
};

// declare taskManager
TaskManager taskManager;

#ifndef countof
// handy macro that calculates the count of elements in an array
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

// general constants
const RgbColor BlackColor = RgbColor(0,0,0);
const uint16_t PixelCount = 4; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

// global variables
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin); 
NeoPixelAnimator animations(PixelCount);
MyAnimationState animationState[PixelCount];

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
const uint8_t EffectSet = EffectFlag_All; // or effects that you want like AsFlag(Effect_Candle) | AsFlag(Effect_Cycle); 

// foreward declare timer functions so they can be referenced before they are defined
void SwitchToRunning(uint32_t deltaTime);
void SwitchToSleep(uint32_t deltaTime);

// declare switch task and running and stopping timers
SwitchEffectTask switchEffectTask(MsToTaskTime(240000), EffectSet); // 240000 ms = 4 minutes
FunctionTask runningTimer(SwitchToSleep, MsToTaskTime(18000000)); // 18000000 ms = 5 hours
FunctionTask sleepingTimer(SwitchToRunning, MsToTaskTime(68400000)); // 68400000 ms = 19 hours

// Functions

void setup()
{
  Serial.begin(115200);

#ifdef LED_BUILTIN
  // if there is a built in LED, turn it off to save power
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);    
#endif

  strip.Begin();
  strip.Show(); 

  // must be done before the power saving tricks below
  SetRandomSeed();

  // power saving tricks for AVR
#ifdef ADCSRA
  ADCSRA = 0; // disable ADC
  power_all_disable();
  power_timer0_enable(); // used for millis()
  power_usart0_enable(); // used for Serial
#endif

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
 
void SetRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits being set
    seed = analogRead(0);
    delay(1);

    // so read it several times and shift those bits up in the seed
    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    // Serial.println(seed);
    randomSeed(seed);
}

