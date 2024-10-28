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
        Position_ = RotationSensor_.get_angle();
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