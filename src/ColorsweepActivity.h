#ifndef COLORSWEEPACTIVITY_H
#define COLORSWEEPACTIVITY_H
#include <LedActivity.h>

#define SWEEP_DELAY 1
class ColorsweepActivity : public LedActivity {
public:
    ColorsweepActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool update(int param)
    {
        if(micros() - lastShiftTime >= SWEEP_DELAY)
        {
            lastShiftTime = micros();
            coord += 5;
        }

        for (int i = 0; i < NUM_LEDS; i++)
        {
            float r = baseDistance + (stepDistance * (i + 1));
            int color = inoise8(coord,  r);
            color = qsub8(color, 16);
            color = qadd8(color, scale8(color, 39));
            ledControl->leds[i] = ColorFromPalette( RainbowColors_p, color + offset, 255, LINEARBLEND);
        }
         
        offset++;
        return true;
    }
private:
    long coord;
    int offset;
    long lastShiftTime;
    int baseDistance = 20; // governs how drastically color changes with movement
    int stepDistance = 30; //governs how different each pixel is from the one before it.
};
#endif