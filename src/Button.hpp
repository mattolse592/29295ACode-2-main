#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../include/pros/misc.hpp"

class Button
{

private:
    pros::Controller &Master_;
    bool Pressed_ = false;
    pros::controller_digital_e_t Button_;

public:
    Button(pros::Controller &Master, pros::controller_digital_e_t button)
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
#endif  // BUTTON_HPP