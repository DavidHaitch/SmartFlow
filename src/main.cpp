#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "MotionState.h"
#include "LedControl.h"
#include "LedActivity.h"
#include "ColormapActivity.h"
#include "IgniteActivity.h"
#include "SparkleActivity.h"

LSM9DS1 imu;
MotionState motionState;
LedControl ledControl;
IgniteActivity ignite(&motionState, &ledControl);
SparkleActivity sparkle(&motionState, &ledControl);
ColormapActivity colormap(&motionState, &ledControl);
LedActivity* base;
LedActivity* effect;

long lastDebugPrint = 0;

void setup()
{
    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;
    imu.settings.gyro.scale = 2000;
    imu.settings.accel.scale = 8;
    imu.begin();
    float angularVelocity = 10;
    Serial.begin(9600);
    while(angularVelocity > 0.01)
    {
        Serial.println(angularVelocity);
        motionState.Update(&imu);
        angularVelocity = motionState.angularVelocity;
        imu.calibrate(true);
    }

    base = &colormap;
    effect = &sparkle;
}

void loop()
{
    if(millis() - lastDebugPrint > 250)
    {
        lastDebugPrint = millis();
        Serial.println(motionState.angularVelocityPercent);
    }

    motionState.Update(&imu);
    base->update(motionState.jerkPercent);
    effect->update(0);
    ledControl.Refresh();
}

void transition(LedActivity* from, LedActivity* to)
{
    from->end();
    from = to;
    from->begin();
}