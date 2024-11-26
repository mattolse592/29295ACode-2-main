#ifndef TAPBUTTON_HPP
#define TAPBUTTON_HPP

#include "../include/pros/misc.hpp"

class TapButton
{

private:
    pros::Controller &Master_;
    int Pressed_ = 0;
    bool dePressed_ = false;
    pros::controller_digital_e_t Button_;

public:
    TapButton(pros::Controller &Master, pros::controller_digital_e_t button)
        : Master_(Master)
    {
        Button_ = button;
    }

    void Tick()
    {
        if (Master_.get_digital_new_press(Button_))
        {
            Pressed_ = Pressed_ + 1;
            dePressed_ = true;
        }
        else
        {
            dePressed_ = false;
        }
    }

    int TimesPressed()
    {
        return Pressed_;
    }
    bool IsPressed()
    {
        return dePressed_;
    }
};
#endif // TAPBUTTON_HPP