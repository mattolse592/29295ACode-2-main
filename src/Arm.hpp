#ifndef ARM_HPP
#define ARM_HPP

#include "Motor.hpp"
#include "RotationSensor.hpp"

class Arm
{

private:
    Motor Motor_;
    RotationSensor RotationSensor_;
public:
    enum State
    {
        DOCK=0,
        LOAD=1,
        REACH=2,
        SCORE=3
    };

private:
    State target = DOCK;

public:
    Arm(Motor Motor, RotationSensor RotationSensor)
        : Motor_(Motor), RotationSensor_(RotationSensor)
    {
        Motor_.SetBrakeMode(MOTOR_BRAKE_BRAKE);
    }

    void Tick()
    {
        switch (target)
        {
        case DOCK:
            Dock();
            break;
        case LOAD:
            Load();
            break;
        case REACH:
            Reach();
            break;
        case SCORE:
            Score();
            break;

        default:
            break;
        }
    }

    void ManualMove(int stickInput)
    {
        Motor_.SetSpeed(stickInput);        
    }

    void SetTarget(State state)
    {
        target = state;
    }

private:
    void Dock()
    {
    }

    void Load()
    {
    }

    void Reach()
    {
    }

    void Score()
    {
    }
};
#endif  // ARM_HPP