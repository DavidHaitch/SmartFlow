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
    
    virtual bool update(int param);
    ~LedActivity()
    {

    }
protected:
    MotionState* motionState;
    LedControl* ledControl;
};
#endif