#ifndef LEDCONTROL_H
#define LEDCONTROL_H
#include <FastLED.h>

#define NUM_LEDS    13
#define EFFECTIVE_LEDS 6
class LedControl
{
public:
    LedControl();
    uint8_t brightness;
    CRGB leds[NUM_LEDS];
    void Clear();
    void Refresh();
private:
    long lastRefresh;
};
#endif