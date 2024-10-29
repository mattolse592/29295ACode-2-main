#ifndef PIDGPT_HPP
#define PIDGPT_HPP

#include "RotationSensor.hpp"
#include "Motor.hpp"

class PIDController
{
private:
    double kp;
    double ki;
    double kd;
    double target;

public:
    PIDController(double kp, double ki, double kd, double target)
        : kp_(kp), ki_(ki), kd_(kd), target_(target), previous_error_(0), integral_(0) {}

    double Calculate(double measured_value)
    {
        // Calculate the error
        double error = target_ - measured_value;

        // Proportional term
        double proportional = kp_ * error;

        // Integral term
        integral_ += error;
        double integral = ki_ * integral_;

        // Derivative term
        double derivative = kd_ * (error - previous_error_);
        previous_error_ = error;

        // PID output
        double output = proportional + integral + derivative;

        // Limit output to motor's accepted range (-127 to 127)
        if (output > 127) {
            output = 127;
        }
            
        if (output < -127) {
            output = -127;
        }

        return output;
    }

    void ChangeP(double newP) {
        kp_ += newP;
    }

    void setTarget(double target)
    {
        target_ = target;
    }

    double getTarget() {
        return target;
    }

private:
    double kp_, ki_, kd_;   // PID gains
    double target_;         // Target position in degrees
    double previous_error_; // Previous loop error for derivative
    double integral_;       // Cumulative sum for integral
};
#endif // PIDGPT_HPP