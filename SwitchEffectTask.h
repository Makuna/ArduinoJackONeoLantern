
#define     Effect_First        0
#define     Effect_Candle       0
#define     Effect_RadioActive  1
#define     Effect_Cycle        2
#define     Effect_COUNT        3

class SwitchEffectTask : public Task
{
public:
  SwitchEffectTask(uint32_t intervalMs) :
    Task(intervalMs),
    activeEffect(Effect_First), 
    pActiveEffectTask(NULL)
  {
  }
    
private:
  int activeEffect;
  Task* pActiveEffectTask;

  virtual void OnStart()
  {
    int newEffect = random(Effect_First, Effect_COUNT); 
 
    ApplyEffect(newEffect);
  }
  
  virtual void OnStop()
  {
    if (pActiveEffectTask != NULL)
    {
      taskManager.StopTask(pActiveEffectTask);
    }
  }
  
  virtual void OnUpdate(uint32_t deltaMs)
  {
    // never pick the same one, increment at least by one
    int randomOffset = random(1, Effect_COUNT); 
    // increment and wrap
    int newEffect = (activeEffect + randomOffset) % Effect_COUNT; 
 
    ApplyEffect(newEffect);
  }
  
  void ApplyEffect(int newEffect)
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
};

