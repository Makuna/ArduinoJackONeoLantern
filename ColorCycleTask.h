

// cycle effect
#define CycleMinBrightness 5
#define CycleMaxBrightness 192
#define CycleInterval 1500
const uint16_t CyclePixel[] = {0, 1, 2, 3};

// simple blend function
void CycleAnimUpdate(const AnimationParam& param)
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

class ColorCycleTask : public Task
{
public:
  ColorCycleTask() :
      Task(MsToTaskTime(33)), // 30hz
    cycleColor(BlackColor),
    cycleState(0)
  {
  }
    
private:
  RgbwColor cycleColor;
  uint16_t cycleState;

  virtual bool OnStart() // optional
  {
//    Serial.println("cycle on");
//    Serial.flush();
      return true;
  }
  
  virtual void OnStop() // optional
  {
//    Serial.println("cycle off");
//    Serial.flush();
    for (uint16_t pixel = 0; pixel < countof(CyclePixel); pixel++)
    {
      strip.SetPixelColor(CyclePixel[pixel], BlackColor);
    }
  }
  
  virtual void OnUpdate(uint32_t deltaTime)
  {
    if (animations.IsAnimating())
    {
      animations.UpdateAnimations();
      strip.Show();
    }
    else
    {
      switch (cycleState)
      {
        case 0:
          cycleColor = RgbColor( random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness));
                
          BlendCyclePixelToColor(0, cycleColor);
          BlendCyclePixelToColor(3, BlackColor);
          cycleState++;
          break;
          
        case 1:
          BlendCyclePixelToColor(1, cycleColor);
          BlendCyclePixelToColor(0, BlackColor);
          cycleState++;
          break;
          
        case 2:
          BlendCyclePixelToColor(2, cycleColor);
          BlendCyclePixelToColor(1, BlackColor);
          cycleState++;
          break;
          
        case 3:
          BlendCyclePixelToColor(3, cycleColor);
          BlendCyclePixelToColor(2, BlackColor);
          cycleState = 0; // repeat
          break;
      }
    }
  }

  void BlendCyclePixelToColor(uint16_t cyclePixel, RgbwColor color)
  {
      uint16_t pixel = CyclePixel[cyclePixel];
      animationState[pixel].StartingColor = strip.GetPixelColor(pixel);
      animationState[pixel].EndingColor = color;
      animations.StartAnimation(pixel, CycleInterval, CycleAnimUpdate);
  }
 
};  
