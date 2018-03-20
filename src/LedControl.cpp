#include <LedControl.h>
const int refreshRate = 1000 / 60;
LedControl::LedControl()
{
    brightness = 255;
    maxBrightness = 255;
    FastLED.addLeds<APA102, BGR>(leds, NUM_LEDS);
    Clear();
}

void LedControl::Refresh()
{
    int dT = millis() - lastRefresh;
    if(dT > refreshRate)
    {
        lastRefresh += dT;
        uint8_t b = map(brightness, 0, 255, 0, maxBrightness);
        if(b < 16) b = 16;
        FastLED.setBrightness(b);
        FastLED.show();
    }
}

void LedControl::Clear()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }

    FastLED.show();
}