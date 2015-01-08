

// cycle effect
#define CycleMinBrightness 5
#define CycleMaxBrightness 192
#define CycleInterval 1500
const int CyclePixel[] = {0, 1, 2, 3};

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
  RgbColor cycleColor;
  int cycleState;

  virtual void OnStart() // optional
  {
//    Serial.println("cycle on");
//    Serial.flush();
  }
  
  virtual void OnStop() // optional
  {
//    Serial.println("cycle off");
//    Serial.flush();
    for (int pixel = 0; pixel < countof(CyclePixel); pixel++)
    {
      strip.SetPixelColor(CyclePixel[pixel], BlackColor);
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
      switch (cycleState)
      {
        case 0:
          cycleColor = RgbColor( random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness),
                random(CycleMinBrightness, CycleMaxBrightness));
                
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[0], cycleColor);    
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[3], BlackColor);    

          cycleState++;
          break;
          
        case 1:
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[1], cycleColor);    
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[0], BlackColor);    

          cycleState++;
          break;
          
        case 2:
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[2], cycleColor);    
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[1], BlackColor); 
          cycleState++;
          break;
          
        case 3:
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[3], cycleColor);    
          strip.LinearFadePixelColor(CycleInterval, CyclePixel[2], BlackColor); 
          cycleState = 0; // repeat
          break;
      }
    }
  }
 
};  
