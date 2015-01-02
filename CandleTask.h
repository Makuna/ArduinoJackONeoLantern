

// candle effect 
#define CandleFlickerMaxInterval 400
#define CandleFlickerMinInterval 100
const RgbColor DimCandleColor = RgbColor(16, 0, 0);
const RgbColor BrightCandleColor = RgbColor(92, 64, 8);
const int CandlePixel[] = {0, 3};


TASK_DECLARE_BEGIN(CandleTask)

  TASK_DECLARE_FUNCTION OnStart() // optional
  {
//    Serial.println("candle on");
//    Serial.flush();
  }
  
  TASK_DECLARE_FUNCTION OnStop() // optional
  {
//    Serial.println("candle off");
//    Serial.flush();
    for (int pixel = 0; pixel < countof(CandlePixel); pixel++)
    {
      strip.SetPixelColor(CandlePixel[pixel], BlackColor);
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
      uint16_t time = random(CandleFlickerMinInterval, CandleFlickerMaxInterval);
      uint8_t brightness = random(256);
      RgbColor color = RgbColor::LinearBlend(DimCandleColor, BrightCandleColor, brightness);
  
      for (int pixel = 0; pixel <  countof(CandlePixel); pixel++)
      {
        strip.LinearFadePixelColor(time, CandlePixel[pixel], color);
      }
    }
  }
  
TASK_DECLARE_END  
