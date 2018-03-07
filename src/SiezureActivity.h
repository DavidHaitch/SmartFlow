#ifndef SIEZUREACTIVITY_H
#define SIEZUREACTIVITY_H
#include <LedActivity.h>

class SiezureActivity : public LedActivity {
public:
    SiezureActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool update(int param)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            offset+=16;
            ledControl->leds[i] = ColorFromPalette(RainbowColors_p, offset, 255, LINEARBLEND);
        }
        
        return true;
    }
private:
    int offset;
    long lastShiftTime;
};
#endif