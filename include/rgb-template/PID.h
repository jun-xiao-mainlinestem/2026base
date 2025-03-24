#pragma once
#include "vex.h"

class PID
{

private: 
  float error = 0;
  float kp = 0;
  float ki = 0;
  float kd = 0;
  float starti = 0;
  float settle_error = 0;
  float settle_time = 0;
  float timeout = 0;
  float accumulated_error = 0;
  float previous_error = 0;
  float output = 0;
  float time_spent_settled = 0;
  float time_spent_running = 0;
  
public:
  PID(float error, float kp, float kd);
  PID(float error, float kp, float ki, float kd, float starti, float settle_error, float settle_time, float timeout);
  float compute(float error);
  bool is_done();
};