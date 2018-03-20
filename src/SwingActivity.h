#ifndef SWINGACTIVITY_H
#define SWINGACTIVITY_H
#include <LedActivity.h>

class SwingActivity : public LedActivity {
public:
    SwingActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool update(int param)
    {
        ledControl->brightness = map(smoothstep(motionState->angularVelocityPercent / 100.0) * 100, 0, 100, ledControl->maxBrightness / 8, ledControl->maxBrightness);
        return true;
    }

private:
    float smoothstep(float x)
    {
        return (4 * pow(x, 3)) - (3 * pow(x, 4));
    }
};
#endif