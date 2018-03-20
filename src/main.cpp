#include <Wire.h>
#include <LSM9DS1.h>
#include "MotionState.h"
#include "LedControl.h"
#include "LedActivity.h"
#include "ColormapActivity.h"
//#include "SparkleActivity.h"
#include "SwingActivity.h"
#include "ColorsweepActivity.h"
#include "ConfigManager.h"
#include "PovActivity.h"
#include "SiezureActivity.h"
LSM9DS1 imu;
MotionState motionState;
LedControl ledControl;
//IgniteActivity ignite(&motionState, &ledControl);
SwingActivity swing(&motionState, &ledControl);
ColorsweepActivity colorsweep(&motionState, &ledControl);
ColormapActivity colormap(&motionState, &ledControl);
PovActivity pov(&motionState, &ledControl);
SiezureActivity zap(&motionState, &ledControl);
LedActivity* base;
LedActivity* effect;
ConfigManager config;

long lastDebugPrint = 0;
bool effectEnable = true;
void calibrate()
{
    motionState.Update(&imu);
    int good = 0;
    while(good < 2)
    {
        ledControl.leds[1] = CRGB::Red;
        ledControl.Refresh();
        if(motionState.angularVelocity < 5)
        {
            good++;
            delay(100);
        }
        else
        {
            for(int i = 0; i < 3; i++)
            {
                imu.aBias[i] = 0;
                imu.aBiasRaw[i] = 0;
                imu.gBias[i] = 0;
                imu.gBiasRaw[i] = 0;
                imu.mBias[i] = 0;
                imu.mBiasRaw[i] = 0;
            }

            imu.calibrate(true);
            imu.calibrateMag(true);
            delay(10);
            good = 0;
        }

        motionState.Update(&imu);
    }
}

void setup()
{
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;
    imu.settings.gyro.scale = 2000;
    imu.settings.gyro.sampleRate = 6;
    imu.settings.gyro.HPFEnable = true;
    imu.settings.accel.scale = 16;
    imu.begin();
    calibrate();
    Serial.begin(9600);
    base = &colormap;
    effect = &swing;
}

bool configured = false;

void loop()
{
    motionState.Update(&imu);

    base->update(motionState.jerkPercent);
    if(!configured)
    {
        int c = config.configure(&motionState, &ledControl);

        if(c == 1)
        {
            if(config.options[0]%3 == 0)
            {
                ledControl.maxBrightness = 255;
            }
            else if(config.options[0]%3 == 1)
            {
                ledControl.maxBrightness = 128;
            }
            else
            {
                ledControl.maxBrightness = 32;
            }
        }

        if(c == 2)
        {
            uint8_t mode = config.options[1]%4;
            effectEnable = false;
            switch(mode)
            {
                case 0:
                    effectEnable = true;
                    base = &colormap;
                    break;
                case 1:
                    effectEnable = true;
                    base = &colorsweep;
                    break;
                case 2:
                    effectEnable = true;
                    base = &pov;
                    break;
                case 3:
                    effectEnable = true;
                    base = &zap;
                    break;
            }
        }

        if(c >= 3)
        {          
            configured = true;
        }
    }

    if(effectEnable)
    {
        effect->update(0);
    }

    ledControl.Refresh();
}