#ifndef COLORMAPACTIVITY_H
#define COLORMAPACTIVITY_H
#include <LedActivity.h>

class ColormapActivity : public LedActivity {
public:
    ColormapActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        palette = RainbowColors_p;
    }

    CRGBPalette16 palette;

    bool update(int param)
    {
        if(millis() - lastShiftDecay > 20)
        {
            //shift += param / 5;
            if(shift > 0) shift--;
        }

        shift = map(motionState->orientation.getYawRadians() * 100,-314, 314, 0, 255 );

        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int color = inoise8(motionState->pointingX * r, motionState->pointingY * r, motionState->pointingZ * r);
            ledControl->leds[i] = ColorFromPalette( palette, color + shift, 255, LINEARBLEND);
        }
        
        return true;
    }

private:
    long lastShiftDecay = 0;
    uint8_t shift = 0;
    int baseDistance = 200; // governs how drastically color changes with movement
    int stepDistance = 5; //governs how different each pixel is from the one before it.
};
#endif