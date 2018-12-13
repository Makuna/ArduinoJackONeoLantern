

// candle effect 
#define CandleFlickerMaxInterval 800
#define CandleFlickerMinInterval 200

const RgbColor DimCandleColor = RgbColor(32, 8, 8);
const RgbColor BrightCandleColor = RgbColor(124, 124, 100);

const uint16_t CandlePixel[] = {0, 3}; // if more than one, which ones do we use for candle

// simple blend function
void CandleAnimUpdate(const AnimationParam& param)
{
    // this gets called for each animation on every time step
    // param.progress will start at 0.0 and end at 1.0
    float progress;

    // apply curves based on brightness change
    if (animationState[param.index].IsGettingBrighter)
    {
        progress = NeoEase::CubicIn(param.progress);
    }
    else
    {
        progress = NeoEase::CubicOut(param.progress);
    }

    // we use the blend function on the RgbColor to mix
    // color based on the modified progress
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        progress);
    // apply the color to the strip
    strip.SetPixelColor(param.index, updatedColor);
}

TASK_DECLARE_BEGIN(CandleTask)

TASK_DECLARE_START  // optional
  {
    Serial.println("candle on");
    Serial.flush();
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
      RgbColor color = RgbColor::LinearBlend(DimCandleColor, BrightCandleColor, brightness);
  
      for (uint16_t candle = 0; candle < countof(CandlePixel); candle++)
      {
          uint16_t pixel = CandlePixel[candle];
          if (pixel < countof(animationState))
          {
              animationState[pixel].StartingColor = animationState[pixel].EndingColor;
              animationState[pixel].EndingColor = color;
              animationState[pixel].IsGettingBrighter = color.CalculateBrightness() > animationState[pixel].StartingColor.CalculateBrightness();

              animations.StartAnimation(pixel, time, CandleAnimUpdate);
          }
      }
    }
  }
  
TASK_DECLARE_END  
