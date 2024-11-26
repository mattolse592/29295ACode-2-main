#ifndef SHIFTEDBUTTON_HPP
#define SHIFTEDBUTTON_HPP

#include "HoldButton.hpp"

class ShiftedButton
{

private:
    HoldButton &TriggerButton_;
    HoldButton &ShiftButton_;
    bool Value_ = false;

public:
    ShiftedButton(HoldButton &TriggerButton, HoldButton &ShiftButton)
        : TriggerButton_(TriggerButton),
          ShiftButton_(ShiftButton)
    {
    }

    void Tick()
    {
        if (ShiftButton_.IsPressed() == false && TriggerButton_.IsPressed()) {
            Value_ = true;
        } else if (ShiftButton_.IsPressed() == true && TriggerButton_.IsPressed()) {
            Value_ = false;
        }
    }

    bool IsOn()
    {
        return Value_;
    }
};

#endif  // SHIFTEDBUTTON_HPP