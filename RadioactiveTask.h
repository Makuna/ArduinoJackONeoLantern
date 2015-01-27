

// radioactive effect 

#define GlowMaxInterval 2000
#define GlowMinInterval 1000
#define StableMaxInterval 2000
#define StableMinInterval 500
const RgbColor RadioActiveLowColor = RgbColor(62,114, 0);
const RgbColor RadioActiveStableColor = RgbColor(74,136, 0);
const RgbColor RadioActiveHighColor = RgbColor(124,228, 0);
const int RadioActivePixel[] = {0, 1, 2, 3}; 

class RadioactiveTask : public Task
{
public:
  RadioactiveTask() : 
      Task(MsToTaskTime(33)), // 30 hz
    radioActiveState(RadioActiveState_Stable)  
  {
  }

private:
  enum RadioActiveState
  {
    RadioActiveState_Stable,
    RadioActiveState_Increasing,
    RadioActiveState_Decreasing
  };

  RadioActiveState radioActiveState;

  virtual bool OnStart() // optional
  {
//    Serial.println("radioactive on");
//    Serial.flush();
    for (int pixel = 0; pixel < countof(RadioActivePixel); pixel++)
    {
      strip.LinearFadePixelColor(10, RadioActivePixel[pixel], RadioActiveLowColor);
    }
    return true;
  }
  
  virtual void OnStop() // optional
  {
//    Serial.println("radioactive off");
//    Serial.flush();
    for (int pixel = 0; pixel < countof(RadioActivePixel); pixel++)
    {
      strip.SetPixelColor(RadioActivePixel[pixel], BlackColor);
    }
  }
  
  virtual void OnUpdate(uint32_t deltaTime)
  {
    if (strip.IsAnimating())
    {
      strip.UpdateAnimations();
      strip.Show();
    }
    else
    {
      switch (radioActiveState)
      {
        case RadioActiveState_Increasing:
          // change to increasing
          {
            uint16_t time = random(GlowMinInterval, GlowMaxInterval);
            
            for (int pixel = 0; pixel < countof(RadioActivePixel); pixel++)
            {
              uint8_t brightness = random(127) + 128; // upper range
              RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveHighColor, brightness);
  
              strip.LinearFadePixelColor(time, RadioActivePixel[pixel], color);
            }
          }
          radioActiveState = RadioActiveState_Decreasing;
          break;
          
        case RadioActiveState_Decreasing:
          {
            uint16_t time = random(GlowMinInterval, GlowMaxInterval);
            
            for (int pixel = 0; pixel < countof(RadioActivePixel); pixel++)
            {
              uint8_t brightness = random(127); // lower range
              RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveStableColor, brightness);
  
              strip.LinearFadePixelColor(time, RadioActivePixel[pixel], color);
            }
          }
          radioActiveState = RadioActiveState_Stable;
          break;
          
        case RadioActiveState_Stable:
          {
            uint16_t time = random(StableMinInterval, StableMaxInterval);
            
            for (int pixel = 0; pixel < countof(RadioActivePixel); pixel++)
            {
              uint8_t brightness = random(255); // full range
              RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveStableColor, brightness);
  
              strip.LinearFadePixelColor(time, RadioActivePixel[pixel], color);
            }
          }
          radioActiveState = RadioActiveState_Increasing;
          break;
      }
    }
  }
  
TASK_DECLARE_END  
