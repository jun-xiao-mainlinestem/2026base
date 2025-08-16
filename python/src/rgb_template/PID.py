#!/usr/bin/env python3

class PID:
    """PID controller class for autonomous movement control"""
    
    def __init__(self, error, kp, kd=None, ki=None, start_i=None, 
                 settle_error=None, settle_time=None, timeout=None):
        """Initialize PID controller"""
        self.error = error
        self.kp = kp
        self.ki = ki if ki is not None else 0.0
        self.kd = kd if kd is not None else 0.0
        self.start_i = start_i if start_i is not None else 0.0
        self.settle_error = settle_error if settle_error is not None else 0.0
        self.settle_time = settle_time if settle_time is not None else 0.0
        self.timeout = timeout if timeout is not None else 0.0
        
        # Internal state
        self.accumulated_error = 0.0
        self.previous_error = 0.0
        self.output = 0.0
        self.time_spent_settled = 0.0
        self.time_spent_running = 0.0
    
    def compute(self, error):
        """Compute PID output for given error"""
        # StartI is used to prevent integral windup
        if abs(error) < self.start_i:
            self.accumulated_error += error
        
        # Check if error has crossed 0, eliminate integral term if it has
        if (error > 0 and self.previous_error < 0) or (error < 0 and self.previous_error > 0):
            self.accumulated_error = 0
        
        # Calculate PID output
        self.output = (self.kp * error + 
                      self.ki * self.accumulated_error + 
                      self.kd * (error - self.previous_error))
        
        self.previous_error = error
        
        # Update settle time tracking
        if abs(error) < self.settle_error:
            self.time_spent_settled += 10
        else:
            self.time_spent_settled = 0
        
        self.time_spent_running += 10
        
        return self.output
    
    def is_done(self):
        """Check if PID controller has finished"""
        # Check timeout
        if self.timeout != 0 and self.time_spent_running > self.timeout:
            return True
        
        # Check settle time
        if self.time_spent_settled > self.settle_time:
            return True
        
        return False
