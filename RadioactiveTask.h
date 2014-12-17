

// radioactive effect 

#define GlowMaxInterval 2000
#define GlowMinInterval 1000
#define StabilMaxInterval 2000
#define StabilMinInterval 500
const RgbColor RadioActiveLowColor = RgbColor(62,114, 0);
const RgbColor RadioActiveStabilColor = RgbColor(74,136, 0);
const RgbColor RadioActiveHighColor = RgbColor(124,228, 0);
const int RadioActivePixel[] = {0, 1, 2, 3}; 

class RadioactiveTask : public Task
{
public:
  RadioactiveTask() : 
    Task(33), // 30 hz
    radioActiveState(RadioActiveState_Stabil)  
  {
  }

private:
  enum RadioActiveState
  {
    RadioActiveState_Stabil,
    RadioActiveState_Increasing,
    RadioActiveState_Decreasing
  };

  RadioActiveState radioActiveState;

  virtual void OnStart() // optional
  {
    for (int pixel = 0; pixel < CountOf(RadioActivePixel); pixel++)
    {
      strip.LinearFadePixelColor(10, RadioActivePixel[pixel], RadioActiveLowColor);
    }
  }
  
  virtual void OnStop() // optional
  {
    for (int pixel = 0; pixel < CountOf(RadioActivePixel); pixel++)
    {
      strip.SetPixelColor(RadioActivePixel[pixel], BlackColor);
    }
  }
  
  virtual void OnUpdate(uint32_t deltaTimeMs)
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
        case RadioActiveState_Stabil:
          // change to increasing
          {
            uint16_t time = random(GlowMinInterval, GlowMaxInterval);
            
            for (int pixel = 0; pixel <  CountOf(RadioActivePixel); pixel++)
            {
              uint8_t brightness = random(127) + 128;
              RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveHighColor, brightness);
  
              strip.LinearFadePixelColor(time, RadioActivePixel[pixel], color);
            }
          }
          radioActiveState = RadioActiveState_Increasing;
          break;
        case RadioActiveState_Increasing:
          // change to decreasing
          {
            uint16_t time = random(GlowMinInterval, GlowMaxInterval);
            
            for (int pixel = 0; pixel <  CountOf(RadioActivePixel); pixel++)
            {
              uint8_t brightness = random(127);
              RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveStabilColor, brightness);
  
              strip.LinearFadePixelColor(time, RadioActivePixel[pixel], color);
            }
          }
          radioActiveState = RadioActiveState_Decreasing;
          break;
        case RadioActiveState_Decreasing:
          // change to stabil
          {
            uint16_t time = random(StabilMinInterval, StabilMaxInterval);
            
            for (int pixel = 0; pixel <  CountOf(RadioActivePixel); pixel++)
            {
              uint8_t brightness = random(255);
              RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveStabilColor, brightness);
  
              strip.LinearFadePixelColor(time, RadioActivePixel[pixel], color);
            }
          }
          radioActiveState = RadioActiveState_Stabil;
          break;
      }
    }
  }
  
TASK_DECLARE_END  
