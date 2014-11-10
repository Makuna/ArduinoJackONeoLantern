

// cycle effect
#define CycleMinBrightness 35
#define CycleMaxBrightness 180
#define CycleBlend1 85
#define CycleBlend2 170
#define CycleInterval 1000
const int CyclePixel[] = {0, 1, 2, 3};  
RgbColor CycleLeftColor = RgbColor(0,255,0);
RgbColor CycleRightColor = RgbColor(255,0,0);
RgbColor CycleNewColor;
int CycleState = 0;

TASK_DECLARE_BEGIN(ColorCycleTask)

  TASK_DECLARE_FUNCTION OnStart() // optional
  {
    Serial.println("cycle on");
    uint8_t blend = CycleBlend1;
    RgbColor color;
  
    strip.LinearFadePixelColor(10, CyclePixel[0], CycleRightColor);
    color = RgbColor::LinearBlend(CycleRightColor, CycleLeftColor, blend);
    strip.LinearFadePixelColor(10, CyclePixel[1], color);
    color = RgbColor::LinearBlend(CycleRightColor, CycleLeftColor, CycleBlend2);
    strip.LinearFadePixelColor(10, CyclePixel[2], color);
    strip.LinearFadePixelColor(10, CyclePixel[3], CycleLeftColor);
  }
  
  TASK_DECLARE_FUNCTION OnStop() // optional
  {
    Serial.println("cycle off");
    for (int pixel = 0; pixel < CountOf(CyclePixel); pixel++)
    {
      strip.SetPixelColor(CyclePixel[pixel], BlackColor);
    }
  }
  
  TASK_DECLARE_FUNCTION OnUpdate(uint32_t deltaTimeMs)
  {
    if (strip.IsAnimating())
    {
      strip.UpdateAnimations();
      strip.Show();
      delay(31); // ~30hz change cycle
    }
    else
    {
      switch (CycleState)
      {
        case 0:
          {
            CycleNewColor = RgbColor( random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness));
            RgbColor targetColor1 = RgbColor::LinearBlend(CycleNewColor, CycleLeftColor, CycleBlend2);  
            RgbColor targetColor3 = RgbColor::LinearBlend(CycleLeftColor, CycleRightColor, CycleBlend1);  
            RgbColor targetColor4 = RgbColor::LinearBlend(CycleLeftColor, CycleRightColor, CycleBlend2);  
         
            CycleAnimate(targetColor1, CycleLeftColor, targetColor3, targetColor4);
          }
          CycleState++;
          break;
        case 1:
          {
            RgbColor targetColor1 = RgbColor::LinearBlend(CycleNewColor, CycleLeftColor, CycleBlend1);  
            RgbColor targetColor2 = RgbColor::LinearBlend(CycleNewColor, CycleLeftColor, CycleBlend2);  
            RgbColor targetColor4 = RgbColor::LinearBlend(CycleLeftColor, CycleRightColor, CycleBlend1);  
         
            CycleAnimate(targetColor1, targetColor2, CycleLeftColor, targetColor4);
          }
          CycleState++;
          break;
        case 2:
          {
            RgbColor targetColor2 = RgbColor::LinearBlend(CycleNewColor, CycleLeftColor, CycleBlend1);  
            RgbColor targetColor3 = RgbColor::LinearBlend(CycleNewColor, CycleLeftColor, CycleBlend2);  
          
            CycleAnimate(CycleNewColor, targetColor2, targetColor3, CycleLeftColor);
          }
      
          CycleRightColor = CycleLeftColor;
          CycleLeftColor = CycleNewColor;
          CycleState = 0;
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
  
TASK_DECLARE_END  
