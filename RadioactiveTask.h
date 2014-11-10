

// radioactive effect 
enum RadioActiveState
{
  RadioActiveState_Stabil,
  RadioActiveState_Increasing,
  RadioActiveState_Decreasing
};
#define GlowMaxInterval 2000
#define GlowMinInterval 1000
#define StabilMaxInterval 2000
#define StabilMinInterval 500
const RgbColor RadioActiveLowColor = RgbColor(62,114, 0);
const RgbColor RadioActiveStabilColor = RgbColor(74,136, 0);
const RgbColor RadioActiveHighColor = RgbColor(124,228, 0);
const int RadioActivePixel[] = {0, 1, 2, 3}; 
RadioActiveState radioActiveState = RadioActiveState_Stabil;

TASK_DECLARE_BEGIN(RadioactiveTask)

  TASK_DECLARE_FUNCTION OnStart() // optional
  {
    Serial.println("radioactive on");
    for (int pixel = 0; pixel < CountOf(RadioActivePixel); pixel++)
    {
      strip.LinearFadePixelColor(10, RadioActivePixel[pixel], RadioActiveLowColor);
    }
  }
  
  TASK_DECLARE_FUNCTION OnStop() // optional
  {
    Serial.println("radioactive off");
    for (int pixel = 0; pixel < CountOf(RadioActivePixel); pixel++)
    {
      strip.SetPixelColor(RadioActivePixel[pixel], BlackColor);
    }
  }
  
  TASK_DECLARE_FUNCTION OnUpdate(uint32_t deltaTimeMs)
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