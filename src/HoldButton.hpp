#ifndef HOLDBUTTON_HPP
#define HOLDBUTTON_HPP

#include "../include/pros/misc.hpp"

class HoldButton
{

private:
    pros::Controller &Master_;
    bool Pressed_ = false;
    pros::controller_digital_e_t Button_;

public:
    HoldButton(pros::Controller &Master, pros::controller_digital_e_t button)
        : Master_(Master), Button_{button}
    {
    }

    void Tick()
    {
        Pressed_ = Master_.get_digital(Button_);
    }

    bool IsPressed()
    {
        return Pressed_;
    }
};
#endif  // HOLDBUTTON_HPP