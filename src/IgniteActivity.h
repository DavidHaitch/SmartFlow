#ifndef IGNITEACTIVITY_H
#define IGNITEACTIVITY_H
#include <LedActivity.h>

class IgniteActivity : public LedActivity {
public:
    IgniteActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    void begin()
    {
    }

    bool update(int param)
    {
        if (ignitedLeds <= NUM_LEDS / 2)
        {
            if(millis() - lastChangeTime > 50)
            {
                ignitedLeds++;
                lastChangeTime = millis();
            }

            for (int i = 0; i < NUM_LEDS; i++)
            {
                if (i < ignitedLeds || NUM_LEDS - i < ignitedLeds)
                {
                    //ledControl->leds[i] = CRGB::White;
                    continue;
                }
                else
                {
                    ledControl->leds[i] = CRGB::Black;
                }
            }

            return true;
        }       
        else
        {
            return false;
        }
    }

    void end()
    {       
    }
private:
    long lastChangeTime;
    uint8_t ignitedLeds;
};
#endif