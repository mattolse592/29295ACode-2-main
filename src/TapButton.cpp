#include "../include/pros/misc.hpp"

class TapButton
{

private:
    pros::Controller &Master_;
    int Pressed_ = 0;
    pros::controller_digital_e_t Button_;

public:
    TapButton(pros::Controller &Master, pros::controller_digital_e_t button)
        : Master_(Master)
    {
        Button_ = button;
    }

    void Tick()
    {
        if (Master_.get_digital_new_press(Button_)){
            Pressed_ = Pressed_ + 1;
        }
    }

    int TimesPressed()
    {
        return Pressed_;
    }
}; 