

class SwitchEffectTask : public Task
{
public:
  SwitchEffectTask(uint32_t interval) :
      Task(interval),
    activeEffect(Effect_First), 
    pActiveEffectTask(NULL)
  {
  }
    
private:
  enum Effect
  {
    Effect_First,
    Effect_Candle = 0,
    Effect_Radioactive,
    Effect_Cycle,
    Effect_COUNT
  };
  
  uint16_t activeEffect;
  Task* pActiveEffectTask;

  virtual bool OnStart()
  {
//    Serial.println("new effects on");
//    Serial.flush();
    uint16_t newEffect = random(Effect_First, Effect_COUNT); 
 
    ApplyEffect(newEffect);
    return true;
  }
  
  virtual void OnStop()
  {
//    Serial.println("new effects off");
//    Serial.flush();
    if (pActiveEffectTask != NULL)
    {
      taskManager.StopTask(pActiveEffectTask);
    }
    strip.Show(); // force update to the strip
  }
  
  virtual void OnUpdate(uint32_t delta)
  {
    Serial.println("new effect");

    // never pick the same one, increment at least by one
    uint16_t randomOffset = random(1, Effect_COUNT); 
    // increment and wrap
    uint16_t newEffect = (activeEffect + randomOffset) % Effect_COUNT; 
 
    ApplyEffect(newEffect);
  }
  
  void ApplyEffect(uint16_t newEffect)
  {
    if (pActiveEffectTask != NULL)
    {
      taskManager.StopTask(pActiveEffectTask);
    }
    activeEffect = newEffect;  
      
    switch (activeEffect)
    {
      case Effect_Candle:
        pActiveEffectTask = &candleTask;
        break;
      case Effect_Radioactive:
        pActiveEffectTask = &radioactiveTask;
        break;
      case Effect_Cycle:
        pActiveEffectTask = &colorCycleTask;
        break;
    }
    
    if (pActiveEffectTask != NULL)
    {
      taskManager.StartTask(pActiveEffectTask);
    }
  }
};

