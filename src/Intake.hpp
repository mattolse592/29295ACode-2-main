#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "Motor.hpp"

class Intake
{

private:
    Motor Motor_;
    int Speed_;

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

    void ChangeSpeed(int speed) {
        Speed_ = speed;
    }

    void Discard() {
        Forward(); 
        pros::delay(500);
        Reverse();
        pros::delay(300);
    }

};
#endif  // INTAKE_HPP