#ifndef TOGGLEBUTTON_HPP
#define TOGGLEBUTTON_HPP

#include "../include/pros/misc.hpp"

class ToggleButton
{

private:
    pros::Controller &Master_;
    bool Pressed_ = false;
    pros::controller_digital_e_t Button_;

public:
    ToggleButton(pros::Controller &Master, pros::controller_digital_e_t button)
        : Master_(Master)
    {
        Button_ = button;
    }

    void Tick()
    {
        if (Master_.get_digital_new_press(Button_)){
            Pressed_ = !Pressed_;
        }
    }

    bool IsOn()
    {
        return Pressed_;
    }
}; 
#endif  // TOGGLEBUTTON_HPP