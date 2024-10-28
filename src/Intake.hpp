#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "Motor.hpp"

class Intake
{

private:
    Motor Motor_;

public:
    Intake(Motor Motor)
        : Motor_(Motor)
    {
    }

    void Tick()
    {
    }

    void Forward()
    {
        Motor_.SetSpeed(127);
    }

    void Reverse()
    {
        Motor_.SetSpeed(-127);
    }

    void Stop()
    {
        Motor_.SetSpeed(0);
    }
};
#endif  // INTAKE_HPP