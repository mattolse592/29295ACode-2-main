#ifndef OPTICALSENSOR_HPP
#define OPTICALSENSOR_HPP

#include "../include/pros/optical.hpp"

class OpticalSensor
{

private:
    pros::Optical OpticalSensor_;
    

public:
    int Hue_;
    OpticalSensor(char PortNum)
        : OpticalSensor_{PortNum}
    {
    }

    void Tick()
    {
        
        Hue_ = OpticalSensor_.get_hue();
    }

 
   
};
#endif // OPTICALSENSOR_HPP