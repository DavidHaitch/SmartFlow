#ifndef SPARKLEACTIVITY_H
#define SPARKLEACTIVITY_H
#include <LedActivity.h>

#define SPARKLE_LENGTH 50
class SparkleActivity : public LedActivity {
public:
    SparkleActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
    }

    bool update(int param)
    {
        if(millis() - lastSparkleTime > SPARKLE_LENGTH)
        {
            lastSparkleTime = millis();
            for (int i = 0; i < NUM_LEDS; i++)
            {
                float threshold = smoothstep(motionState->angularVelocityPercent / 100.0) * 100;
                if (rand() % 100 < threshold)
                {
                    sparkled[i] = true;
                }
                else
                {
                    sparkled[i] = false;
                }
            }
        }

        for (int i = 0; i < NUM_LEDS; i++)
        {
            if(sparkled[i])
            {
                ledControl->leds[i] = CRGB::White;
            }
        }

        return true;
    }

private:
    long lastSparkleTime;
    bool sparkled[NUM_LEDS];
    float smoothstep(float x)
    {
        return (4 * pow(x, 3)) - (3 * pow(x, 4));
    }
};
#endif