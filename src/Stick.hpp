#ifndef STICK_HPP
#define STICK_HPP

#include "../include/pros/misc.hpp"

class Stick
{

private:
    pros::Controller &Master_;
    int Position_;
    int DeadZone_;
    pros::controller_analog_e_t Stick_;

public:
    Stick(pros::Controller &Master, pros::controller_analog_e_t stick, int deadZone = 5)
        : Master_(Master), Stick_{stick}, DeadZone_{deadZone}
    {
    }

    void Tick()
    {
        Position_ = Master_.get_analog(Stick_);

        // apply deadzone
        if (Position_ < DeadZone_ && Position_ > -DeadZone_)
        {
            Position_ = 0;
        }
    }

    int GetPosition()
    {
        return Position_;
    }
};
#endif // STICK_HPP