#ifndef SHIFTEDBUTTON_HPP
#define SHIFTEDBUTTON_HPP

#include "HoldButton.hpp"

class ShiftedButton
{

private:
    HoldButton &TriggerButton_;
    HoldButton &ShiftButton_;
    bool Shifted_ = false;

public:
    ShiftedButton(HoldButton &TriggerButton, HoldButton &ShiftButton)
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