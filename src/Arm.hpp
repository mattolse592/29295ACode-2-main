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
    PIDController pid_ = PIDController(7.0, 0.0, 0.0, 0.0);

    int current_position = 0;
    double pid_output;

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
        current_position = RotationSensor_.GetPosition();
        pid_output = pid_.Calculate(current_position);

        // Use the output from PID to set motor speed
        Motor_.SetSpeed(pid_output);

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
        //Motor_.SetSpeed(stickInput / 2);
    }

    void SetTarget(State state)
    {
        target = state;
    }

    void ChangeP(double deltaP)
    {
        pid_.ChangeP(deltaP);
    }

    double GetPosition()
    {
        return current_position;
    }

    double GetPIDValue() {
        return pid_output;
    }

    State GetState()
    {
        return target;
    }

#pragma region arm states
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
#pragma endregion
};
#endif // ARM_HPP