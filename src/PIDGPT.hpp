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

    double currentValue;

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
        if (abs(error) < 1)
        {
            integral_ = 0.0;
        }
        if (integral_ > 127)
        {
            integral_ = 127;
        }
        else if (integral_ < -127)
        {
            integral_ = -127;
        }
        double integral = ki_ * integral_;

        // Derivative term
        double derivative = kd_ * (error - previous_error_);
        previous_error_ = error;

        // PID output
        double output = proportional + integral + derivative;

        // Limit output to motor's accepted range (-127 to 127)
        if (output > 127)
        {
            output = 127;
        }

        if (output < -127)
        {
            output = -127;
        }
        currentValue = output;
        return currentValue;
    }

    void ChangeP(double deltaP)
    {
        kp_ += deltaP;
    }

    void setTarget(double target)
    {
        target_ = target;
    }

    double getTarget()
    {
        return target;
    }

    double getCurrentValue()
    {
        return currentValue;
    }

private:
    double kp_, ki_, kd_;   // PID gains
    double target_;         // Target position in degrees
    double previous_error_; // Previous loop error for derivative
    double integral_;       // Cumulative sum for integral
};
#endif // PIDGPT_HPP