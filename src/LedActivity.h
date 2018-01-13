#ifndef LEDACTIVITY_H
#define LEDACTIVITY_H
#include <MotionState.h>
#include <LedControl.h>

class LedActivity{
public:
    LedActivity(MotionState* _motionState, LedControl* _ledControl)
    {
        motionState = _motionState;
        ledControl = _ledControl;
    }

    virtual void begin();
    virtual bool update(int param);
    virtual void end();
protected:
    MotionState* motionState;
    LedControl* ledControl;
};
#endif