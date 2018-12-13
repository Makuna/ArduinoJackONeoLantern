
inline uint8_t AsFlag(uint8_t enumValue)
{
    return 1 << enumValue;
}

enum Effect
{
    Effect_First,
    Effect_Candle = 0,
    Effect_Radioactive,
    Effect_Cycle,
    Effect_COUNT
};

const uint8_t EffectFlag_All = AsFlag(Effect_Candle) | AsFlag(Effect_Radioactive) | AsFlag(Effect_Cycle);

class SwitchEffectTask : public Task
{
public:
  SwitchEffectTask(uint32_t interval, uint8_t effects) :
      Task(interval),
    _pActiveEffectTask(NULL),
    _activeEffect(Effect_COUNT), // invalid, will cause a new pick
    _effects(effects)
  {
      if (_effects >= AsFlag(Effect_COUNT))
      {
          // invalid flags, default to all
          _effects = EffectFlag_All;
      }
  }
    
private:
  Task* _pActiveEffectTask;
  Effect _activeEffect;
  uint8_t _effects;

  Effect PickNextEffect()
  {
      uint16_t newEffect = random(Effect_First, Effect_COUNT);

      while ((AsFlag(newEffect) & _effects) == 0)
      {
          // increment and wrap until we find a valid one
          newEffect = (newEffect + 1) % Effect_COUNT;
      }

      return static_cast<Effect>(newEffect);
  }

  virtual bool OnStart()
  {
//    Serial.println("new effects on");
//    Serial.flush();
    Effect newEffect = PickNextEffect();
 
    ApplyEffect(newEffect);
    return true;
  }
  
  virtual void OnStop()
  {
//    Serial.println("new effects off");
//    Serial.flush();
    if (_pActiveEffectTask != NULL)
    {
      taskManager.StopTask(_pActiveEffectTask);
    }
    strip.Show(); // force update to the strip
  }
  
  virtual void OnUpdate(uint32_t delta)
  {
    Serial.println("new effect");

    Effect newEffect = PickNextEffect();
 
    ApplyEffect(newEffect);
  }
  
  void ApplyEffect(Effect newEffect)
  {
      if (_activeEffect != newEffect)
      {
          if (_pActiveEffectTask != NULL)
          {
              taskManager.StopTask(_pActiveEffectTask);
          }
          _activeEffect = newEffect;

          switch (_activeEffect)
          {
          case Effect_Candle:
              _pActiveEffectTask = &candleTask;
              break;
          case Effect_Radioactive:
              _pActiveEffectTask = &radioactiveTask;
              break;
          case Effect_Cycle:
              _pActiveEffectTask = &colorCycleTask;
              break;

          default:
              _pActiveEffectTask = NULL;
              break;
          }

          if (_pActiveEffectTask != NULL)
          {
              taskManager.StartTask(_pActiveEffectTask);
          }
      }
  }
};

