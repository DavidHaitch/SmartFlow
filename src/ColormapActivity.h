#ifndef BASEACTIVITY_H
#define BASEACTIVITY_H
#include <LedActivity.h>

class ColormapActivity : public LedActivity {
public:
    ColormapActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        palette = RainbowColors_p;
    }

    CRGBPalette16 palette;
    
    void begin()
    {
    }

    bool update(int param)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int color = inoise8(motionState->pointingX * r, motionState->pointingY * r, motionState->pointingZ * r);
            color = qsub8(color, 16);
            color = qadd8(color, scale8(color, 39));
            ledControl->leds[i] = ColorFromPalette( palette, color + param, ledControl->brightness, LINEARBLEND);
        }
        return true;
    }

    void end()
    {     
    }
private:
    int baseDistance = 2; // governs how drastically color changes with movement
    int stepDistance = 15; //governs how different each pixel is from the one before it.
};
#endif