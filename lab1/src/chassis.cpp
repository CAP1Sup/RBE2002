#include <Romi32U4.h>
#include "chassis.h"

float RomiChassis::SpeedLeft(void)
{
  // !!! ATTENTION !!!
  // Assignment 1
  float speed = (C_wheel / N_wheel) * ((count_left - prev_count_left) / interval); // Possible mistake on time 
  return speed; //[mm/s]
}

float RomiChassis::SpeedRight(void)
{
  // !!! ATTENTION !!!
  // Assignment 1
  float speed = (C_wheel / N_wheel) * ((count_right - prev_count_right) / interval); // Possible mistake on time 
  return speed; //[mm/s]
}


float RomiChassis::EffortLeft(void){
  return effort_left;
}

float RomiChassis::EffortRight(void){
  return effort_right;
}




void RomiChassis::UpdateEffortDriveWheels(int left, int right)
{ 
  motors.setEfforts(left,right);
}

void RomiChassis::UpdateEffortDriveWheelsPI(int target_speed_left, int target_speed_right)
{
  // !!! ATTENTION !!!
  // Assignment 2
  {
    float u_left = 0;
    float u_right = 0;

    float error_Left = target_speed_left - this->SpeedLeft(); 
    float error_Right = target_speed_right - this->SpeedRight(); 

    E_left += error_Left;
    E_right += error_Right; 

    u_left = Kp * error_Left + Ki * E_left; 
    u_right = Kp * error_Right + Ki * E_right; 

    effort_left = u_left;
    effort_right = u_right;
    motors.setEfforts(u_left,u_right);
  }
}

void RomiChassis::SerialPlotter(float a, float b, float c, float d)
{
    // !!! ATTENTION !!!
    // USE this function for assignment 3!
    Serial.print(a);
    Serial.print('\t');
    Serial.print(b);
    Serial.print('\t');
    Serial.print(c);
    Serial.print('\t');
    Serial.print(d);
    Serial.println();
}

void RomiChassis::MotorControl(void)
{
  uint32_t now = millis();
  if(now - last_update >= interval)
  {    
    prev_count_left = count_left;
    prev_count_right = count_right;
    count_left = encoders.getCountsLeft();
    count_right = encoders.getCountsRight();
    previous_time = millis();
    UpdateEffortDriveWheelsPI(target_left, target_right);
    last_update = now;
  }
}

void RomiChassis::StartDriving(float left, float right, uint32_t duration)
{
  target_left = left; target_right = right;
  start_time = millis();
  last_update = start_time;
  end_time = start_time + duration; //fails at rollover
  E_left = 0;
  E_right = 0;
}

bool RomiChassis::CheckDriveComplete(void)
{
  return millis() >= end_time;
}

void RomiChassis::Stop(void)
{
  target_left = target_right = 0;
  motors.setEfforts(0, 0);
}