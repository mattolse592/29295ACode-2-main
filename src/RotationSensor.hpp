#ifndef ROTATIONSENSOR_HPP
#define ROTATIONSENSOR_HPP

#include "../include/pros/rotation.hpp"

class RotationSensor
{

private:
    int Position_;
    pros::Rotation RotationSensor_;

public:
    RotationSensor(char PortNum)
        : RotationSensor_{PortNum}
    {
    }

    void Tick()
    {
        Position_ = RotationSensor_.get_position() / 100;
    }

    void Zero() {
        RotationSensor_.set_position(0);
        Tick();
    }

    int GetPosition()
    {
        return Position_;
    }
};
#endif  // ROTATIONSENSOR_HPP