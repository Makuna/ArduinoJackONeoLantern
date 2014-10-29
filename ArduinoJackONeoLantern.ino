#include <NeoPixelBus.h>
 
#define CountOf(a) (sizeof(a) / sizeof(a[0]))

// general constants
const RgbColor BlackColor = RgbColor(0,0,0);

// proximity sensor constants
#define ProximitySensorAnalogIndex 0 // note:  This is specifically the analog index, not physical pin
#define ProximityThreshold 85 // larger is closer, 65-512 usable ranges
#define ProximityConsecutiveReadings 8 // number of readings at thresholds before changing proximity state
 
// proximity effect constants
const RgbColor ProximityColor = RgbColor(128,0, 0);
const int EffectPixel[] = {0, 1, 2, 3}; 

 
// candle effect constants
#define CandleFlickerMaxInterval 400
#define CandleFlickerMinInterval 100
const RgbColor DimCandleColor = RgbColor(16, 0, 0);
const RgbColor BrightCandleColor = RgbColor(92, 64, 8);
const int CandlePixel[] = {1, 2}; 
 
// proximity sensor variables
int proximityInRangeReadings = 0;
bool isProximityDetected = false;
 
// candle effect variables 
NeoPixelBus strip = NeoPixelBus(4, 2);

void setup()
{
  strip.Begin();
  strip.Show(); 
  randomSeed(analogRead(2));
  Serial.begin(9600);
}
 
void loop()
{
//  if (UpdateProximityDetection())
//  {
//    if (isProximityDetected)
//    {
//     TurnOffCandleEffect();
//      TurnOnProximityEffect();
//    }
//    else
//    {
//      TurnOffProximityEffect();
//      TurnOnCandleEffect();
//    }
//  }

  if (isProximityDetected)
  {
    UpdateProximityEffect();
  }
  else
  {
    UpdateCandleEffect();
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
 
void TurnOffProximityEffect()
{
  Serial.println("proximity off");
  for (int pixel = 0; pixel < CountOf(EffectPixel); pixel++)
  {
    strip.SetPixelColor(EffectPixel[pixel], BlackColor);
  }
}
 
void TurnOnProximityEffect()
{
  Serial.println("proximity on");
  for (int pixel = 0; pixel < CountOf(EffectPixel); pixel++)
  {
    strip.LinearFadePixelColor(10, EffectPixel[pixel], ProximityColor);
  }
}
 
void UpdateProximityEffect()
{
  if (strip.IsAnimating())
  {
    strip.UpdateAnimations();
    strip.Show();
    delay(31); // ~30hz change cycle
  }
}

bool UpdateProximityDetection()
{
  int proximity = analogRead(ProximitySensorAnalogIndex);
  bool previousState = isProximityDetected;
  
  // leakey bucket implementation for state change
  // this provides a debounce filter
  //
  
  // update count of in range readings
  if (proximity > ProximityThreshold)
  {
    // fill to limit
    proximityInRangeReadings = min(ProximityConsecutiveReadings, proximityInRangeReadings + 1);
  }
  else if (proximityInRangeReadings > 0)
  {
    // leak
    proximityInRangeReadings--;
  }
  
  // test thresholds and set state only if thresholds are reached
  // otherwise leave the state alone
  if (proximityInRangeReadings == ProximityConsecutiveReadings)
  {
    isProximityDetected = true;
  }
  else if (proximityInRangeReadings == 0)
  {
    isProximityDetected = false;
  }
  
  return previousState != isProximityDetected;
}
