#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "../include/pros/motors.hpp"
#include "../include/pros/motors.h"
class Motor
{

private:
    int MotorSpeed_;
    pros::Motor Motor_;

public:
    Motor(char PortNum,  pros::motor_gearset_e GearBox)
        : Motor_(PortNum, GearBox)
    {
    }

    void Tick() {
        
    }

    void SetSpeed(int motorSpeed) {
        // values from -127 to 127
        MotorSpeed_ = motorSpeed;
        Motor_.move(MotorSpeed_);
    }

    void Stop() {
        SetSpeed(0);
    }

    void SetBrakeMode(pros::motor_brake_mode_e brakeMode) {
        Motor_.set_brake_mode(brakeMode);
    }
};
#endif  // MOTOR_HPP