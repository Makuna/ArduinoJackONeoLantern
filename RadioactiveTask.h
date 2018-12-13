

// radioactive effect 

#define GlowMaxInterval 2000
#define GlowMinInterval 1000
#define StableMaxInterval 2000
#define StableMinInterval 500
const RgbColor RadioActiveLowColor = RgbColor(62,114, 0);
const RgbColor RadioActiveStableColor = RgbColor(74,136, 0);
const RgbColor RadioActiveHighColor = RgbColor(124,228, 0);
const uint16_t RadioActivePixel[] = {0, 1, 2, 3}; 

// simple blend function
void RadioactiveAnimUpdate(const AnimationParam& param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);
    // apply the color to the strip
    strip.SetPixelColor(param.index, updatedColor);
}

class RadioactiveTask : public Task
{
public:
    RadioactiveTask() :
        Task(MsToTaskTime(33)), // 30 hz
        _radioActiveState(RadioActiveState_Stable)
    {
    }

private:
    enum RadioActiveState
    {
        RadioActiveState_Stable,
        RadioActiveState_Increasing,
        RadioActiveState_Decreasing
    };

    RadioActiveState _radioActiveState;

    virtual bool OnStart() // optional
    {
        Serial.println("radioactive on");
        Serial.flush();

        for (uint16_t radioPixel = 0; radioPixel < countof(RadioActivePixel); radioPixel++)
        {
            uint16_t pixel = RadioActivePixel[radioPixel];
            animationState[pixel].StartingColor = BlackColor;
            animationState[pixel].EndingColor = RadioActiveLowColor;
            animations.StartAnimation(pixel, 10, RadioactiveAnimUpdate);
        }
        return true;
    }

    virtual void OnStop() // optional
    {
        //    Serial.println("radioactive off");
        //    Serial.flush();
        for (uint16_t pixel = 0; pixel < countof(RadioActivePixel); pixel++)
        {
            strip.SetPixelColor(RadioActivePixel[pixel], BlackColor);
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
            switch (_radioActiveState)
            {
            case RadioActiveState_Increasing:
                // change to increasing
            {
                uint16_t time = random(GlowMinInterval, GlowMaxInterval);

                for (uint16_t radioPixel = 0; radioPixel < countof(RadioActivePixel); radioPixel++)
                {
                    uint8_t brightness = random(127) + 128; // upper range
                    RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveHighColor, brightness/255.0f);

                    uint16_t pixel = RadioActivePixel[radioPixel];
                    animationState[pixel].StartingColor = strip.GetPixelColor(pixel);
                    animationState[pixel].EndingColor = color;
                    animations.StartAnimation(pixel, time, RadioactiveAnimUpdate);
                }
            }
            _radioActiveState = RadioActiveState_Decreasing;
            break;

            case RadioActiveState_Decreasing:
            {
                uint16_t time = random(GlowMinInterval, GlowMaxInterval);

                for (uint16_t radioPixel = 0; radioPixel < countof(RadioActivePixel); radioPixel++)
                {
                    uint8_t brightness = random(127); // lower range
                    RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveStableColor, brightness/255.0f);

                    uint16_t pixel = RadioActivePixel[radioPixel];
                    animationState[pixel].StartingColor = strip.GetPixelColor(pixel);
                    animationState[pixel].EndingColor = color;
                    animations.StartAnimation(pixel, time, RadioactiveAnimUpdate);
                }
            }
            _radioActiveState = RadioActiveState_Stable;
            break;

            case RadioActiveState_Stable:
            {
                uint16_t time = random(StableMinInterval, StableMaxInterval);

                for (uint16_t radioPixel = 0; radioPixel < countof(RadioActivePixel); radioPixel++)
                {
                    uint8_t brightness = random(256); // full range
                    RgbColor color = RgbColor::LinearBlend(RadioActiveLowColor, RadioActiveStableColor, brightness/255.0f);

                    uint16_t pixel = RadioActivePixel[radioPixel];
                    animationState[pixel].StartingColor = strip.GetPixelColor(pixel);
                    animationState[pixel].EndingColor = color;
                    animations.StartAnimation(pixel, time, RadioactiveAnimUpdate);
                }
            }
            _radioActiveState = RadioActiveState_Increasing;
            break;
            }
        }
    }
};
