#include "../include/pros/adi.hpp"

class MogoMech
{

private:
    pros::ADIDigitalOut Port_;

public:
    MogoMech(char PortName)
        : Port_(PortName)
    {
    }

    void Activate(){
        Port_.set_value(true);
    }
    void Deactivate() {
        Port_.set_value(false);
    }
};