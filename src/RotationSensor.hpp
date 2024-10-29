#ifndef ROTATIONSENSOR_HPP
#define ROTATIONSENSOR_HPP

#include "../include/pros/rotation.hpp"

class RotationSensor
{

private:
    double Position_;
    pros::Rotation RotationSensor_;

public:
    RotationSensor(char PortNum)
        : RotationSensor_{PortNum}
    {
    }

    void Tick()
    {
        Position_ = RotationSensor_.get_position() / 500; //500 is gear ratio
        //check value here if outside of range
    }

    void Zero() {
        RotationSensor_.set_position(0);
        Tick();
    }

    double GetPosition()
    {
        return RotationSensor_.get_position() / 500;    //500 is gear ratio
    }
};
#endif  // ROTATIONSENSOR_HPP