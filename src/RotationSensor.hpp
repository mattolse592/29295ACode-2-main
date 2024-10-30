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
        : RotationSensor_{PortNum,true}
    {
    }

    void Tick()
    {
        Position_ = RotationSensor_.get_position() / 100;
        // check value here if outside of range
    }

    void Zero()
    {
        Position_ = 0;
        RotationSensor_.set_position(Position_);
    }

    double GetPosition()
    {
        return RotationSensor_.get_position() / 100;
    }
};
#endif // ROTATIONSENSOR_HPP