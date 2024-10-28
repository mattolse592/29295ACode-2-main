#ifndef SHIFTEDBUTTON_HPP
#define SHIFTEDBUTTON_HPP

#include "Button.hpp"

class ShiftedButton
{

private:
    Button &TriggerButton_;
    Button &ShiftButton_;
    bool Shifted_ = false;

public:
    ShiftedButton(Button &TriggerButton, Button &ShiftButton)
        : TriggerButton_(TriggerButton),
          ShiftButton_(ShiftButton)
    {
    }

    void Tick()
    {
        if (!TriggerButton_.IsPressed())
        {
            return;
        }
        Shifted_ = ShiftButton_.IsPressed();
    }

    bool IsOn()
    {
        return Shifted_;
    }
};

#endif  // SHIFTEDBUTTON_HPP