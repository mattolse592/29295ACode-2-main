#ifndef LIMITSWITCH_HPP
#define LIMITSWITCH_HPP

#include "../include/pros/adi.hpp"

class LimitSwitch
{

private:
    pros::ADIDigitalIn LimitSwitch_;
    bool Pressed_ = false;

public:
    LimitSwitch(char PortName)
        : LimitSwitch_(PortName)
    {
    }

    void Tick()
    {
        Pressed_ = LimitSwitch_.get_value();
    }

    bool GetValue() {
        return Pressed_;
    }

    
};
#endif // LIMITSWITCH_HPP