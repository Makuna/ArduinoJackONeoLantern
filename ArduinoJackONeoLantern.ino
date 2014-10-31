#include <NeoPixelBus.h>
 
#define CountOf(a) (sizeof(a) / sizeof(a[0]))

// general constants
const RgbColor BlackColor = RgbColor(0,0,0);

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
 
// candle effect 
#define CandleFlickerMaxInterval 400
#define CandleFlickerMinInterval 100
const RgbColor DimCandleColor = RgbColor(16, 0, 0);
const RgbColor BrightCandleColor = RgbColor(92, 64, 8);
const int CandlePixel[] = {1, 2}; 
 
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
 
enum Effect
{
  Effect_Candle,
  Effect_RadioActive,
  Effect_Cycle,
  Effect_COUNT
};

const uint32_t ChangeEffectTimer = 240000; // 4 minutes in milliseconds

// Variables
NeoPixelBus strip = NeoPixelBus(4, 2);
Effect activeEffect = Effect_Candle;
uint32_t effectChangeTick;
  
void setup()
{
  strip.Begin();
  strip.Show(); 
  randomSeed(analogRead(2));
  Serial.begin(9600);
  
  effectChangeTick = millis() - ChangeEffectTimer; // pick new effect emediately
}
 
void loop()
{
  uint32_t currentTick = millis();
  uint32_t delta = currentTick - effectChangeTick;
  
  if (delta > ChangeEffectTimer)
  {
    effectChangeTick = currentTick;
    Serial.println("effect change");
    
    switch (activeEffect)
    {
      case Effect_Candle:
        TurnOffCandleEffect();
        break;
      case Effect_RadioActive:
        TurnOffRadioActiveEffect();
        break;
      case Effect_Cycle:
        TurnOffCycleEffect();
        break;
    }
    
    activeEffect = (Effect)(((int)activeEffect + random(1, Effect_COUNT - 1)) % Effect_COUNT);
    
    switch (activeEffect)
    {
      case Effect_Candle:
        TurnOnCandleEffect();
        break;
      case Effect_RadioActive:
        TurnOnRadioActiveEffect();
        break;
      case Effect_Cycle:
        TurnOnCycleEffect();
        break;
    }
    strip.StartAnimating();
  }
  
  switch (activeEffect)
  {
    case Effect_Candle:
      UpdateCandleEffect();
      break;
    case Effect_RadioActive:
      UpdateRadioActiveEffect();
      break;
    case Effect_Cycle:
      UpdateCycleEffect();
      break;
  }
}
 

void TurnOffCandleEffect()
{
  Serial.println("candle off");
  for (int pixel = 0; pixel < CountOf(CandlePixel); pixel++)
  {
    strip.SetPixelColor(CandlePixel[pixel], BlackColor);
  }
}
 
void TurnOnCandleEffect()
{
  Serial.println("candle on");
}

void UpdateCandleEffect()
{
  if (strip.IsAnimating())
  {
    strip.UpdateAnimations();
    strip.Show();
    delay(31); // ~30hz change cycle
  }
  else
  {
    uint16_t time = random(CandleFlickerMinInterval, CandleFlickerMaxInterval);
    uint8_t brightness = random(256);
    RgbColor color = RgbColor::LinearBlend(DimCandleColor, BrightCandleColor, brightness);

    for (int pixel = 0; pixel <  CountOf(CandlePixel); pixel++)
    {
      strip.LinearFadePixelColor(time, CandlePixel[pixel], color);
    }
  }
}
 
void TurnOffRadioActiveEffect()
{
  Serial.println("radioactive off");
  for (int pixel = 0; pixel < CountOf(RadioActivePixel); pixel++)
  {
    strip.SetPixelColor(RadioActivePixel[pixel], BlackColor);
  }
}
 
void TurnOnRadioActiveEffect()
{
  Serial.println("radioactive on");
  for (int pixel = 0; pixel < CountOf(RadioActivePixel); pixel++)
  {
    strip.LinearFadePixelColor(10, RadioActivePixel[pixel], RadioActiveLowColor);
  }
}
 
void UpdateRadioActiveEffect()
{
  if (strip.IsAnimating())
  {
    strip.UpdateAnimations();
    strip.Show();
    delay(31); // ~30hz change cycle
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

void TurnOffCycleEffect()
{
  Serial.println("cycle off");
  for (int pixel = 0; pixel < CountOf(CyclePixel); pixel++)
  {
    strip.SetPixelColor(CyclePixel[pixel], BlackColor);
  }
}
 
void TurnOnCycleEffect()
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
 
void UpdateCycleEffect()
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
