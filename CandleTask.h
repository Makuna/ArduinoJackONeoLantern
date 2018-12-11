

// candle effect 
#define CandleFlickerMaxInterval 400
#define CandleFlickerMinInterval 100

const RgbwColor DimCandleColor = RgbwColor(16, 0, 0, 96);
const RgbwColor BrightCandleColor = RgbwColor(0, 0, 0, 124);

const uint16_t CandlePixel[] = {0, 3}; // if more than one, which ones do we use for candle

// simple blend function
void CandleAnimUpdate(const AnimationParam& param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbwColor updatedColor = RgbwColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);
    // apply the color to the strip
    strip.SetPixelColor(param.index, updatedColor);
}

TASK_DECLARE_BEGIN(CandleTask)

TASK_DECLARE_START  // optional
  {
//    Serial.println("candle on");
//    Serial.flush();
    return true;
  }
  
TASK_DECLARE_STOP  // optional
  {
//    Serial.println("candle off");
//    Serial.flush();
    for (uint16_t pixel = 0; pixel < countof(CandlePixel); pixel++)
    {
      strip.SetPixelColor(CandlePixel[pixel], BlackColor);
    }
  }
  
TASK_DECLARE_UPDATE 
  {
    if (animations.IsAnimating())
    {
      animations.UpdateAnimations();
      strip.Show();
    }
    else
    {
      uint16_t time = random(CandleFlickerMinInterval, CandleFlickerMaxInterval);
      // 0.0-1.0 random number, applied to a curve so middle values are more common
      float brightness = NeoEase::ExponentialCenter((random(0, INT_MAX) / (float)(INT_MAX - 1)));
      RgbwColor color = RgbwColor::LinearBlend(DimCandleColor, BrightCandleColor, brightness);
  
      for (uint16_t candle = 0; candle < countof(CandlePixel); candle++)
      {
          uint16_t pixel = CandlePixel[candle];
          if (pixel < countof(animationState))
          {
              animationState[pixel].StartingColor = animationState[pixel].EndingColor;
              animationState[pixel].EndingColor = color;

              animations.StartAnimation(pixel, time, CandleAnimUpdate);
          }
      }
    }
  }
  
TASK_DECLARE_END  
