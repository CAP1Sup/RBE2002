#ifndef __CHASSIS_H
#define __CHASSIS_H

#include <Romi32U4.h>

#define N_WHEEL 1440            // how many counts equate to one wheel rotation?
#define R_WHEEL 35              // what is the radius of a Romi wheel in [mm]
#define C_WHEEL 2 * PI *R_WHEEL // circumference of wheel
#define PID_UPDATE_INTERVAL 50  // how often do we update the motor effort? (ms)

class RomiChassis
{
private:
    // Romi specific constants:
    //  !!! ATTENTION !!!

    // declare variables for PI controller
    float target_left = 0;
    float target_right = 0;
    float Kp = 1;
    float Ki = 0.00;
    float cum_E_left = 0; // accumulated errors
    float cum_E_right = 0;

    // encoder and motor objects
    Romi32U4Encoders encoders;
    Romi32U4Motors motors;

    // declare variables for for keeping track of counts and conversion to velocities
    uint32_t start_time = 0;
    uint32_t end_time = 0;
    uint32_t lastPIDUpdate = 0;
    int prev_count_left = 0;
    int prev_count_right = 0;
    float previous_time = 0;
    uint32_t previousLeftTime = 0;
    uint32_t previousRightTime = 0;

public:
    float getLeftSpeed(void);
    float getRightSpeed(void);
    float getLeftEffort(void);
    float getRightEffort(void);

    void setDriveEffort(int a, int b);
    void UpdateEffortDriveWheelsP(int a, int b);
    void updateMotorEffortPI(int a, int b);

    void updateMotorPID(void);
    void printToSerial(float a, float b, float c, float d);

    void beginDriving(float, float, uint32_t);
    bool isDriveComplete(void);
    void stop(void);
};

extern RomiChassis chassis;

#endif