#include "MotionState.h"
MotionState::MotionState()
{
    orientation.begin(100);
}

int MotionState::Update(LSM9DS1* imu)
{
    int dT = millis() - lastUpdateTime;
    int lag = 0;
    if(dT < 10)
    {
        return 0;
    }

    lastUpdateTime += dT;
    lag = dT - 10;

    if (imu->gyroAvailable()) imu->readGyro();
    if (imu->accelAvailable()) imu->readAccel();
    if (imu->magAvailable()) imu->readMag();

    float cgX = imu->calcGyro(imu->gx);
    float cgY = imu->calcGyro(imu->gy);
    float cgZ = imu->calcGyro(imu->gz);

    float caX = imu->calcAccel(imu->ax);
    float caY = imu->calcAccel(imu->ay);
    float caZ = imu->calcAccel(imu->az);

    float cmX = imu->calcMag(imu->mx);
    float cmY = imu->calcMag(imu->my);
    float cmZ = imu->calcMag(imu->mz);
    
    orientation.update(cgX, cgY, cgZ,
                caX, caY, caZ,
                cmX, cmY, cmZ);

    float accel = abs(caX) + abs(caY) + abs(caZ);
    float jerk = abs(accel - lastAccel);
    if(jerk > maxJerk)
    {
        maxJerk = jerk;
        jerkPercent = 100;
    }

    if(maxJerk < 10 * jerk)
    {
        //We don't have enough samples to really make sense.
        jerkPercent = 0;
    }
    else
    {
        jerkPercent = (jerk / maxJerk) * 100;
    }   

    lastAccel = accel;

    // X axis is not factored into angular velocity, because that represents the roll axis.
    // For applications where the SmartFlow board does not point along the axis of the prop, this must be changed.
    angularVelocity = abs(cgY) + abs(cgZ);
    if(angularVelocity > maxAngularVelocity)
    {
        maxAngularVelocity = angularVelocity;
        angularVelocityPercent = 100;
    } 

    if(maxAngularVelocity < 10 * angularVelocity)
    {
        //We don't have enough samples to really make sense.
        angularVelocityPercent = 0;
    }
    else
    {
        angularVelocityPercent = (angularVelocity / maxAngularVelocity) * 100;
    }

    float yawRad = orientation.getYawRadians();
    float pitchRad = orientation.getPitchRadians();

    pointingX = cos(yawRad) * cos(pitchRad);
    pointingY = sin(yawRad) * cos(pitchRad);
    pointingZ = sin(pitchRad);
    return lag;
}