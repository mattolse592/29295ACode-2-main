#ifndef OPTICALSENSOR_HPP
#define OPTICALSENSOR_HPP

#include "../include/pros/optical.hpp"

class OpticalSensor
{

private:
    pros::Optical OpticalSensor_;
    int Hue_;
    int Proximity_;
    
public:
    
    OpticalSensor(char PortNum)
        : OpticalSensor_{PortNum}
    {
    }

    void Tick()
    {
        Proximity_ = OpticalSensor_.get_proximity();
        Hue_ = OpticalSensor_.get_hue();
    }

    int GetHue() {
        return Hue_;
    }

    int GetProx() {
        return Proximity_;
    }
    
    void LEDon() {
        OpticalSensor_.set_led_pwm(100);
    }
   
};
#endif // OPTICALSENSOR_HPP