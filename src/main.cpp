#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "MotionState.h"
#include "LedControl.h"
#include "LedActivity.h"
#include "ColormapActivity.h"
#include "SparkleActivity.h"
#include "ColorsweepActivity.h"
#include "ConfigManager.h"
#include "PovActivity.h"
#include "SiezureActivity.h"
LSM9DS1 imu;
MotionState motionState;
LedControl ledControl;
//IgniteActivity ignite(&motionState, &ledControl);
//SparkleActivity sparkle(&motionState, &ledControl);
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
        Serial.println(motionState.angularVelocity);
        
    }
}

void setup()
{
    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;
    imu.settings.gyro.scale = 2000;
    imu.settings.gyro.sampleRate = 6;
    imu.settings.gyro.HPFEnable = true;
    imu.settings.accel.scale = 16;
    imu.begin();
    Serial.begin(9600);
    calibrate();
    base = &colormap;
    //effect = &sparkle;
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
                ledControl.brightness = 255;
            }
            else if(config.options[0]%3 == 1)
            {
                ledControl.brightness = 128;
            }
            else
            {
                ledControl.brightness = 64;
            }
        }

        if(c == 2)
        {
            if(config.options[1]%4 == 0)
            {
                effectEnable = true;
                base = &colormap;
            }
            else if(config.options[1]%4 == 1)
            {
                effectEnable = false;
                base = &colorsweep;
            }
            else if(config.options[1]%4 == 2)
            {
                effectEnable = false;
                base = &pov;
            }
            else
            {
               effectEnable = false;
               base = &zap; 
            }
        }

        if(c >= 3)
        {
            configured = true;
        }
    }

    if(effectEnable)
    {
        //effect->update(0);
    }

    ledControl.Refresh();
}