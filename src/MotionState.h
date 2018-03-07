#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H
#include <MadgwickAHRS.h>
#include <SparkFunLSM9DS1.h>
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

class MotionState
{
public:
    MotionState();
    Madgwick orientation;
    int angularVelocityPercent;
    float angularVelocity;
    int jerkPercent;
    float jerk;
    float pointingX;
    float pointingY;
    float pointingZ;
    int Update(LSM9DS1* imu); // returns milliseconds if processing falls behind filter rate.
private:
    
    float maxAngularVelocity;
    float maxJerk;
    float lastAccel;
    long lastUpdateTime;
};
#endif