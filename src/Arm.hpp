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
    PIDController pid_ = PIDController(2.9 , 0.05, 9.0, 0.0);

    bool manualTakeover_ = false;

public:
    enum State
    {
        DOCK = 0,
        LOAD = 1,
        REACH = 2,
        SCORE = 3,
        MANUAL = 4
    };

private:
    State Target_ = DOCK;

public:
    Arm(Motor Motor, RotationSensor &RotationSensor)
        : Motor_(Motor), RotationSensor_(RotationSensor)
    {
        Motor_.SetBrakeMode(MOTOR_BRAKE_BRAKE);
    }

    void Tick()
    {
        if (manualTakeover_ == false)
        {
            int current_position = RotationSensor_.GetPosition();
            double pid_output = pid_.Calculate(current_position);

            // Use the output from PID to set motor speed
            Motor_.SetSpeed(pid_output);

            switch (Target_)
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
            case MANUAL:
                // at function here
                break;

            default:
                break;
            }
        }
    }

    void ManualMove(int stickInput)
    {
        Motor_.SetSpeed(stickInput / 2);
        SetTarget(MANUAL);
    }

    void SetTarget(State state)
    {
        Target_ = state;
    }

    void ManualMoveSet(bool takeOverSetting)
    {
        manualTakeover_ = takeOverSetting;
    }

    // get functions
    double GetPosition()
    {
        return RotationSensor_.GetPosition();
    }

    double GetPIDValue()
    {
        return pid_.getCurrentValue();
    }

    State GetState()
    {
        return Target_;
    }

    void Manual()
    {
        pid_.setTarget(RotationSensor_.GetPosition());
    }
#pragma region arm states
private:
    void Dock()
    {
        pid_.setTarget(1.0);
    }

    void Load()
    {
        pid_.setTarget(21.0);
    }

    void Reach()
    {
        pid_.setTarget(121.0);
    }

    void Score() {
        pid_.setTarget(135.0);
    }

#pragma endregion
};
#endif // ARM_HPP