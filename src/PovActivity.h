#ifndef POVACTIVITY_H
#define POVACTIVITY_H
#include <LedActivity.h>

#define FPS 120
#define FRAMES 16

const byte data[] PROGMEM  = {0, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 0,
                            1, 0, 1, 1, 2, 1, 1, 1, 2, 1, 1, 0, 1,
                            1, 1, 0, 2, 1, 1, 1, 1, 1, 2, 0, 1, 1,
                            1, 1, 2, 0, 1, 1, 1, 1, 1, 0, 2, 1, 1,
                            1, 2, 1, 1, 0, 1, 1, 1, 0, 1, 1, 2, 1, 
                            2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 2, 
                            2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 2, 
                            2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 2, 
                            2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 2, 
                            1, 2, 1, 1, 0, 1, 1, 1, 0, 1, 1, 2, 1, 
                            1, 1, 2, 0, 1, 1, 1, 1, 1, 0, 2, 1, 1, 
                            1, 1, 0, 2, 1, 1, 1, 1, 1, 2, 0, 1, 1, 
                            1, 0, 1, 1, 2, 1, 1, 1, 2, 1, 1, 0, 1, 
                            0, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 0, 
                            0, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 0, 
                            0, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 0};
class PovActivity : public LedActivity {
public:
    PovActivity(MotionState* _motionState, LedControl* _ledControl) : LedActivity(_motionState, _ledControl)
    {
        frameDelay = 1000 / FPS;
        hueShiftDelay = 50;
    }

    bool update(int param)
    {
        if(millis() - lastFrameChange > frameDelay)
        {
            lastFrameChange = millis();
            frame++;
        }

        if(millis() - lastHueShift > hueShiftDelay)
        {
            lastHueShift = millis();
            hueShift++;
        }


        for (int i=0; i<NUM_LEDS; i++)
        {
            int pixelIndex = i % NUM_LEDS;
            int index = frame*NUM_LEDS + pixelIndex;
            int colorId = pgm_read_byte_near(data+index);
            CRGB color = CRGB(0,0,0);
            if(colorId == 0)
            {
                color = CHSV(hueShift, 255, 255);
            }
            if(colorId == 2)
            {
                color = CHSV(hueShift + 128, 255, 255);
            }

            ledControl->leds[i] = color;
        }

        if (frame >= FRAMES) frame = 0;

        return true;
    }

private:
    uint8_t hueShift;
    long lastHueShift;
    int hueShiftDelay;
    long lastFrameChange;
    uint8_t frame;
    uint8_t frameDelay;
};
#endif