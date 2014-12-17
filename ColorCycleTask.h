

// cycle effect
#define CycleMinBrightness 35
#define CycleMaxBrightness 180
#define CycleBlend1 85
#define CycleBlend2 170
#define CycleInterval 1000
const int CyclePixel[] = {0, 1, 2, 3};

class ColorCycleTask : public Task
{
public:
  ColorCycleTask() :
    Task(33), // 30hz
    cycleLeftColor(RgbColor(0,255,0)),
    cycleRightColor(RgbColor(255,0,0)),
    cycleState(0)
  {
  }
    
private:
  RgbColor cycleLeftColor;
  RgbColor cycleRightColor;
  RgbColor cycleNewColor;
  int cycleState;

  virtual void OnStart() // optional
  {
    uint8_t blend = CycleBlend1;
    RgbColor color;
  
    strip.LinearFadePixelColor(10, CyclePixel[0], cycleRightColor);
    color = RgbColor::LinearBlend(cycleRightColor, cycleLeftColor, blend);
    strip.LinearFadePixelColor(10, CyclePixel[1], color);
    color = RgbColor::LinearBlend(cycleRightColor, cycleLeftColor, CycleBlend2);
    strip.LinearFadePixelColor(10, CyclePixel[2], color);
    strip.LinearFadePixelColor(10, CyclePixel[3], cycleLeftColor);
  }
  
  virtual void OnStop() // optional
  {
    for (int pixel = 0; pixel < CountOf(CyclePixel); pixel++)
    {
      strip.SetPixelColor(CyclePixel[pixel], BlackColor);
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
      switch (cycleState)
      {
        case 0:
          {
            cycleNewColor = RgbColor( random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness));
            RgbColor targetColor1 = RgbColor::LinearBlend(cycleNewColor, cycleLeftColor, CycleBlend2);  
            RgbColor targetColor3 = RgbColor::LinearBlend(cycleLeftColor, cycleRightColor, CycleBlend1);  
            RgbColor targetColor4 = RgbColor::LinearBlend(cycleLeftColor, cycleRightColor, CycleBlend2);  
         
            CycleAnimate(targetColor1, cycleLeftColor, targetColor3, targetColor4);
          }
          cycleState++;
          break;
        case 1:
          {
            RgbColor targetColor1 = RgbColor::LinearBlend(cycleNewColor, cycleLeftColor, CycleBlend1);  
            RgbColor targetColor2 = RgbColor::LinearBlend(cycleNewColor, cycleLeftColor, CycleBlend2);  
            RgbColor targetColor4 = RgbColor::LinearBlend(cycleLeftColor, cycleRightColor, CycleBlend1);  
         
            CycleAnimate(targetColor1, targetColor2, cycleLeftColor, targetColor4);
          }
          cycleState++;
          break;
        case 2:
          {
            RgbColor targetColor2 = RgbColor::LinearBlend(cycleNewColor, cycleLeftColor, CycleBlend1);  
            RgbColor targetColor3 = RgbColor::LinearBlend(cycleNewColor, cycleLeftColor, CycleBlend2);  
          
            CycleAnimate(cycleNewColor, targetColor2, targetColor3, cycleLeftColor);
          }
      
          cycleRightColor = cycleLeftColor;
          cycleLeftColor = cycleNewColor;
          cycleState = 0;
          break;
      }
    }
  }
  
  void CycleAnimate(RgbColor target1, RgbColor target2, RgbColor target3, RgbColor target4)
  {
    strip.LinearFadePixelColor(CycleInterval, CyclePixel[0], target4);
    strip.LinearFadePixelColor(CycleInterval, CyclePixel[1], target3);
    strip.LinearFadePixelColor(CycleInterval, CyclePixel[2], target2);
    strip.LinearFadePixelColor(CycleInterval, CyclePixel[3], target1);
  }
  
};  
