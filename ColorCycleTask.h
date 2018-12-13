

// cycle effect
#define CycleMinBrightness 5
#define CycleMaxBrightness 192
#define CycleInterval 1500
const uint16_t CyclePixel[] = {0, 1, 2, 3};

// simple blend function
void CycleAnimUpdate(const AnimationParam& param)
{
    // this gets called for each animation on every time step
    // param.progress will start at 0.0 and end at 1.0
    float progress;
    // apply curve
    progress = NeoEase::CubicInOut(param.progress);

    // we use the blend function on the RgbColor to mix
    // color based on the modified progress 
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        progress);
    // apply the color to the strip
    strip.SetPixelColor(param.index, updatedColor);
}

class ColorCycleTask : public Task
{
public:
  ColorCycleTask() :
      Task(MsToTaskTime(33)), // 30hz
    _cycleColor(BlackColor),
    _cycleState(0)
  {
  }
    
private:
  RgbColor _cycleColor;
  uint16_t _cycleState;

  virtual bool OnStart() // optional
  {
    Serial.println("cycle on");
    Serial.flush();
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
      switch (_cycleState)
      {
        case 0:
          _cycleColor = RgbColor( random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness));
                
          BlendCyclePixelToColor(0, _cycleColor);
          BlendCyclePixelToColor(3, BlackColor);
          _cycleState++;
          break;
          
        case 1:
          BlendCyclePixelToColor(1, _cycleColor);
          BlendCyclePixelToColor(0, BlackColor);
          _cycleState++;
          break;
          
        case 2:
          BlendCyclePixelToColor(2, _cycleColor);
          BlendCyclePixelToColor(1, BlackColor);
          _cycleState++;
          break;
          
        case 3:
          BlendCyclePixelToColor(3, _cycleColor);
          BlendCyclePixelToColor(2, BlackColor);
          _cycleState = 0; // repeat
          break;
      }
    }
  }

  void BlendCyclePixelToColor(uint16_t cyclePixel, RgbColor color)
  {
      uint16_t pixel = CyclePixel[cyclePixel];
      animationState[pixel].StartingColor = strip.GetPixelColor(pixel);
      animationState[pixel].EndingColor = color;
      animations.StartAnimation(pixel, CycleInterval, CycleAnimUpdate);
  }
 
};  
