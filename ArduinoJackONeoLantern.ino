#include <NeoPixelBus.h>
#include <Task.h>

#define CountOf(a) (sizeof(a) / sizeof(a[0]))

// general constants
const RgbColor BlackColor = RgbColor(0,0,0);

enum Effect
{
  Effect_Candle,
  Effect_RadioActive,
  Effect_Cycle,
  Effect_COUNT
};

const uint32_t ChangeEffectTimer = 240000; // 4 minutes in milliseconds

// Variables
NeoPixelBus strip = NeoPixelBus(4, 2);
Effect activeEffect = Effect_Candle;
FunctionTask switchEffectTask( switchEffect, ChangeEffectTimer);

TaskManager taskManager();
Task* pActiveEffectTask = null;
CandleTask candleTask(33);
ColorCycleTask colorcycleTask(33);

void setup()
{
  strip.Begin();
  strip.Show(); 
  randomSeed(analogRead(2));
  Serial.begin(9600);
  
  taskManager.StartTask(&switchEffectTask);
}
 
void loop()
{
  taskManager.Loop();
}
 
void switchEffect(uint32_t deltaMs)
{
  Serial.println("effect change");
    
  if (pActiveEffectTask != NULL)
  {
    taskManager.StopTask(pActiveEffectTask);
  }
    
  activeEffect = (Effect)(((int)activeEffect + random(1, Effect_COUNT - 1)) % Effect_COUNT);
    
  switch (activeEffect)
  {
    case Effect_Candle:
      pActiveEffectTask = &candleTask;
      break;
    case Effect_RadioActive:
      pActiveEffectTask = &radioactiveTask;
      break;
    case Effect_Cycle:
      pActiveEffectTask = &colorcycleTask;
      break;
  }
  
  if (pActiveEffectTask != NULL)
  {
    taskManager.StartTask(pActiveEffectTask);
  }
  
  strip.StartAnimating();
}

