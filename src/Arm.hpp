#ifndef ARM_HPP
#define ARM_HPP

#include "Motor.hpp"
#include "RotationSensor.hpp"
#include "PIDGPT.hpp"

class Arm
{

private:
    Motor Motor_;
    RotationSensor RotationSensor_;
    PIDController pid_ = PIDController(5.0, 0.0, 0.0, 0.0);

public:
    enum State
    {
        DOCK = 0,
        LOAD = 1,
        REACH = 2,
        SCORE = 3
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
        double current_position = RotationSensor_.GetPosition();
        double pid_output = pid_.Calculate(current_position);

        // Use the output from PID to set motor speed
        Motor_.SetSpeed(static_cast<int>(pid_output));

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
        // manual move probably doesn't work
    }

    void ManualMove(int stickInput)
    {
        Motor_.SetSpeed(stickInput / 2);
    }

    void SetTarget(State state)
    {
        target = state;
    }

    void ChangeP(double newP)
    {
        pid_.ChangeP(newP);
    }

    State GetState()
    {
        return target;
    }

private:
    void Dock()
    {
        pid_.setTarget(0.0);
    }

    void Load()
    {
        pid_.setTarget(15.0);
    }

    void Reach()
    {
        pid_.setTarget(115.0);
    }

    void Score()
    {
        pid_.setTarget(165.0);
    }
};
#endif // ARM_HPP