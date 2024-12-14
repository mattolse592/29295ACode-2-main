#include "main.h"
#include "Arm.hpp"
#include "RotationSensor.hpp"
#include "Motor.hpp"
#include "LimitSwitch.hpp"
#include "Intake.hpp"
#include "OpticalSensor.hpp"

/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////

int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                       // If this is 127 and the robot tries to heading correct, it's only correcting by
                       // making one side slower.  When this is 87%, it's correcting by making one side
                       // faster and one side slower, giving better heading correction.
int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///
RotationSensor rotSen(15);
pros::ADIDigitalOut m('A');
pros::ADIDigitalOut doinker('D');
LimitSwitch limSwitch('B');
OpticalSensor o(1);
Intake intake(Motor(3, pros::E_MOTOR_GEARSET_06));
Intake hooks(Motor(6, pros::E_MOTOR_GEARSET_18));
Arm arm(Motor(-21, pros::E_MOTOR_GEARSET_36), rotSen);

int reverseTimer = 0;
bool ringDetected = false;
bool badColour = false;
bool intakeOn = false;

Arm::State armPos = Arm::DOCK;
// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

#pragma region constants
void default_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 15, 0, 46, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void one_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void two_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void exit_condition_defaults()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}
#pragma endregion
void colourSort(void *param)
{
  hooks.Forward();
  intake.Forward();
  o.LEDon();
  while (pros::competition::is_autonomous())
  {

    limSwitch.Tick();

    if (limSwitch.GetValue() == true)
    {
      ringDetected = true;
      badColour = false;

      if (o.GetHue() >= 180 && o.GetHue() <= 230 && o.GetProx() > 150)
      {
        badColour = true;
      }
    }

    if (limSwitch.GetValue() == false && ringDetected == true)
    {
      reverseTimer = 20;
      ringDetected = false;
    }

    if (reverseTimer > 0 && badColour)
    {
      hooks.Reverse();
    }
    reverseTimer -= 1;
    pros::delay(ez::util::DELAY_TIME);
  }
}

void initializet()
{
  pros::Task sortTask(colourSort, (void *)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "sorts colours");
}

void ColorSortFunction()
{
  LimitSwitch limSwitch('B');
  OpticalSensor o(1);

  intake.Forward();
  o.LEDon();

  int reverseTimer = 0;
  bool ringDetected = false;
  bool badColour = false;

  while (pros::competition::is_autonomous())
  {
    rotSen.Tick();
    arm.Tick();
    o.Tick();
    limSwitch.Tick();

    arm.SetTarget(armPos);

    master.set_text(0, 0, "limswitch value " + std::to_string(limSwitch.GetValue()));

    if (limSwitch.GetValue() == 1)
    {
      ringDetected = true;
      badColour = false;
    }

    //blue
      if (o.GetHue() >= 180 && o.GetHue() <= 230 && o.GetProx() > 150)
      {
        badColour = true;
      }

      //  //red
      // if (o.GetHue() >= 320 && o.GetHue() <= 360 && o.GetProx() > 150)
      // {
      //   badColour = true;
      // }
      // else if (o.GetHue() >= 0 && o.GetHue() <= 50 && o.GetProx() > 150)
      // {
      //   badColour = true;
      // }


    if (limSwitch.GetValue() == 0 && ringDetected == true)
    {
      reverseTimer = 20;
      ringDetected = false;
    }

    if (reverseTimer > 0 && badColour)
    {
      hooks.Reverse();
    }
    else if (intakeOn == true)
    {
      hooks.Forward();
    }
    else {
      hooks.Stop();
    }
    reverseTimer--;
    pros::delay(ez::util::DELAY_TIME);
  }
}
void ColorSortTest()
{
  pros::Task sortTask(ColorSortFunction);
}

void SafeAutonRed()
{
   pros::Task sortTask(ColorSortFunction);
  intakeOn = false;
  DRIVE_SPEED -= 30;

  // drive to and grab mogo
  chassis.set_drive_pid(-80, DRIVE_SPEED - 20);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(500);
  intakeOn = true;

  // score pre load and turn to ring stacks
  hooks.Forward();
  intake.Forward();
  chassis.set_turn_pid(120, TURN_SPEED);
  chassis.wait_drive();

  // drive to ring stack
  chassis.set_drive_pid(53, DRIVE_SPEED);
  chassis.wait_drive();
  //pros::delay(00);

  // turns and get rings
  chassis.set_turn_pid(63, TURN_SPEED);
  chassis.wait_drive();

  // goes forward into rings
  chassis.set_drive_pid(30, DRIVE_SPEED - 20);
  chassis.wait_drive();
  chassis.set_drive_pid(36, DRIVE_SPEED);
  chassis.wait_drive();

  // turns to rings
  chassis.set_drive_pid(-16, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-47, TURN_SPEED);
  chassis.wait_drive();

  // drives and intakes ring
  chassis.set_drive_pid(70, DRIVE_SPEED-40);
  chassis.wait_drive();

  //go get middle ring
  chassis.set_turn_pid(-100, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED + 30);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED - 20);
  chassis.wait_drive();
  chassis.set_drive_pid(40, DRIVE_SPEED - 10);
  chassis.wait_drive();
  chassis.set_turn_pid(-230, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED);
  chassis.wait_drive();
}

void SafeElimBlue() {
  pros::Task sortTask(ColorSortFunction);
  intakeOn = false;
  DRIVE_SPEED -= 30;

  // drive to and grab mogo
  chassis.set_drive_pid(-80, DRIVE_SPEED - 20);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(500);
  intakeOn = true;

  // score pre load and turn to ring stacks
  hooks.Forward();
  intake.Forward();
  chassis.set_turn_pid(-120, TURN_SPEED);
  chassis.wait_drive();

  // drive to ring stack
  chassis.set_drive_pid(53, DRIVE_SPEED);
  chassis.wait_drive();
  //pros::delay(00);

  // turns and get rings
  chassis.set_turn_pid(-63, TURN_SPEED);
  chassis.wait_drive();

  // goes forward into rings
  chassis.set_drive_pid(30, DRIVE_SPEED / 2);
  chassis.wait_drive();
  chassis.set_drive_pid(38, DRIVE_SPEED);
  chassis.wait_drive();

  // turns to rings
  chassis.set_drive_pid(-16, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(47, TURN_SPEED);
  chassis.wait_drive();

  // drives and intakes ring
  chassis.set_drive_pid(70, DRIVE_SPEED-40);
  chassis.wait_drive();

  //go get middle ring
  chassis.set_turn_pid(100, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED + 30);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED - 20);
  chassis.wait_drive();
  chassis.set_drive_pid(40, DRIVE_SPEED - 10);
  chassis.wait_drive();
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  doinker.set_value(true);
  chassis.set_drive_pid(100, DRIVE_SPEED + 30);
  chassis.wait_drive();
  
}

void soloAWPred()
{
  pros::Task sortTask(ColorSortFunction);
  // drive forwards
  chassis.set_drive_pid(-115, DRIVE_SPEED);
  chassis.wait_drive();

  // turns toward mogo
  chassis.set_turn_pid(35, TURN_SPEED);
  chassis.wait_drive();

  // drives up, graps mogo and score preload
  chassis.set_drive_pid(-22, DRIVE_SPEED - 70);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(300);
  intakeOn = true;
  m.set_value(true);

  // turn and back up into rings
  chassis.set_turn_pid(-10, TURN_SPEED);
  chassis.wait_drive();

  // drops mogo
  chassis.set_drive_pid(50, DRIVE_SPEED - 40);
  chassis.wait_drive();
  pros::delay(1200);

  // hopefully has ring inside bot
  //  chassis.set_drive_pid(45, DRIVE_SPEED);
  //  chassis.wait_drive();

  DRIVE_SPEED -= 15;
  m.set_value(false);
  chassis.set_turn_pid(15, TURN_SPEED);
  chassis.wait_drive();

  // turns towards the other mogo
  TURN_SPEED -= 30;
  chassis.set_turn_pid(-75, TURN_SPEED);
  chassis.wait_drive();

  // drives and grabs mogo
  chassis.set_drive_pid(-60, DRIVE_SPEED - 30);
  chassis.wait_drive();
  pros::delay(100);
  m.set_value(true);
  pros::delay(300);

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  // drives to ring
  chassis.set_drive_pid(100, DRIVE_SPEED);
  chassis.wait_drive();
  pros::delay(800);
  chassis.set_drive_pid(20, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-10, DRIVE_SPEED);
  chassis.wait_drive();


  // turn and drive to ladder
  chassis.set_turn_pid(-150, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(80, DRIVE_SPEED);
  chassis.wait_drive();
  intakeOn = false;

#pragma region turns towards middle ring stack not used
// chassis.set_turn_pid(45, TURN_SPEED);
////intake
// chassis.wait_drive();

// // drives into rings
// chassis.set_drive_pid(80, DRIVE_SPEED - 20);
// chassis.wait_drive();
// chassis.set_drive_pid(10, 50);
// chassis.wait_drive();

// chassis.set_turn_pid(180, TURN_SPEED);
// chassis.wait_drive();

// chassis.set_drive_pid(50, DRIVE_SPEED);
// chassis.wait_drive();
// chassis.set_drive_pid(30, DRIVE_SPEED - 90);
// chassis.wait_drive();
#pragma endregion

#pragma region drive to corner code not used
// //turns to and drive near corner
// chassis.set_turn_pid(-10, TURN_SPEED);
// chassis.wait_drive();
// chassis.set_drive_pid(70, DRIVE_SPEED);
// chassis.wait_drive();

// //turns and drives into corner
// chassis.set_turn_pid(-75, TURN_SPEED);
// chassis.wait_drive();
// chassis.set_drive_pid(100, DRIVE_SPEED);
// chassis.wait_drive();

// // turns drops mogo and goes to ladder
// chassis.set_drive_pid(-35, DRIVE_SPEED);
// chassis.wait_drive();
// chassis.set_turn_pid(120, TURN_SPEED);
// chassis.wait_drive();
// m.set_value(false);
////intakeStop();
// chassis.set_drive_pid(115, DRIVE_SPEED);
// chassis.wait_drive();
// chassis.set_drive_pid(30, DRIVE_SPEED - 90);
#pragma endregion
}

void SafeElimRed() {
  pros::Task sortTask(ColorSortFunction);
  intakeOn = false;
  DRIVE_SPEED -= 30;

  // drive to and grab mogo
  chassis.set_drive_pid(-80, DRIVE_SPEED - 20);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(500);
  intakeOn = true;

  // score pre load and turn to ring stacks
  hooks.Forward();
  intake.Forward();
  chassis.set_turn_pid(120, TURN_SPEED);
  chassis.wait_drive();

  // drive to ring stack
  chassis.set_drive_pid(53, DRIVE_SPEED);
  chassis.wait_drive();
  //pros::delay(00);

  // turns and get rings
  chassis.set_turn_pid(63, TURN_SPEED);
  chassis.wait_drive();

  // goes forward into rings
  chassis.set_drive_pid(30, DRIVE_SPEED / 2);
  chassis.wait_drive();
  chassis.set_drive_pid(38, DRIVE_SPEED);
  chassis.wait_drive();

  // turns to rings
  chassis.set_drive_pid(-16, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-47, TURN_SPEED);
  chassis.wait_drive();

  // drives and intakes ring
  chassis.set_drive_pid(70, DRIVE_SPEED-40);
  chassis.wait_drive();

  //go get middle ring
  chassis.set_turn_pid(-100, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED + 30);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED - 20);
  chassis.wait_drive();
  chassis.set_drive_pid(40, DRIVE_SPEED - 10);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED);
  chassis.wait_drive();
  doinker.set_value(true);
  chassis.set_drive_pid(100, DRIVE_SPEED + 30);
  chassis.wait_drive();
  
}
void soloElimRed() {
  pros::Task sortTask(ColorSortFunction);
  intakeOn = false;
  chassis.set_drive_pid(-115, DRIVE_SPEED);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(50);
  chassis.set_drive_pid(50, DRIVE_SPEED);
  chassis.wait_drive();
  intakeOn = true;
   
}


void soloAWPblue()
{
  pros::Task sortTask(ColorSortFunction);
  // drive forwards
  chassis.set_drive_pid(-115, DRIVE_SPEED);
  chassis.wait_drive();

  // turns toward mogo
  chassis.set_turn_pid(-35, TURN_SPEED);
  chassis.wait_drive();

  // drives up, graps mogo and score preload
  chassis.set_drive_pid(-22, DRIVE_SPEED - 70);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(300);
  intakeOn = true;
  m.set_value(true);

  // turn and back up into rings
  chassis.set_turn_pid(10, TURN_SPEED);
  chassis.wait_drive();

  // drops mogo
  chassis.set_drive_pid(50, DRIVE_SPEED - 40);
  chassis.wait_drive();
  pros::delay(1200);

  // hopefully has ring inside bot
  //  chassis.set_drive_pid(45, DRIVE_SPEED);
  //  chassis.wait_drive();

  DRIVE_SPEED -= 15;
  m.set_value(false);
  chassis.set_turn_pid(-15, TURN_SPEED);
  chassis.wait_drive();

  // turns towards the other mogo
  TURN_SPEED -= 30;
  chassis.set_turn_pid(75, TURN_SPEED);
  chassis.wait_drive();

  // drives and grabs mogo
  chassis.set_drive_pid(-60, DRIVE_SPEED - 30);
  chassis.wait_drive();
  pros::delay(100);
  m.set_value(true);
  pros::delay(300);

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  // drives to ring
  chassis.set_drive_pid(100, DRIVE_SPEED);
  chassis.wait_drive();
  pros::delay(800);
  chassis.set_drive_pid(20, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-10, DRIVE_SPEED);
  chassis.wait_drive();


  // turn and drive to ladder
  chassis.set_turn_pid(150, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(80, DRIVE_SPEED);
  chassis.wait_drive();
  intakeOn = false;
}

void SafeAutonBlue()
{
   pros::Task sortTask(ColorSortFunction);
  intakeOn = false;
  DRIVE_SPEED -= 30;

  // drive to and grab mogo
  chassis.set_drive_pid(-80, DRIVE_SPEED - 20);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(500);
  intakeOn = true;

  // score pre load and turn to ring stacks
  hooks.Forward();
  intake.Forward();
  chassis.set_turn_pid(-120, TURN_SPEED);
  chassis.wait_drive();

  // drive to ring stack
  chassis.set_drive_pid(53, DRIVE_SPEED);
  chassis.wait_drive();
  //pros::delay(00);

  // turns and get rings
  chassis.set_turn_pid(-63, TURN_SPEED);
  chassis.wait_drive();

  // goes forward into rings
  chassis.set_drive_pid(30, DRIVE_SPEED / 2);
  chassis.wait_drive();
  chassis.set_drive_pid(38, DRIVE_SPEED);
  chassis.wait_drive();

  // turns to rings
  chassis.set_drive_pid(-16, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(47, TURN_SPEED);
  chassis.wait_drive();

  // drives and intakes ring
  chassis.set_drive_pid(70, DRIVE_SPEED-40);
  chassis.wait_drive();

  //go get middle ring
  chassis.set_turn_pid(100, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED + 30);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED - 20);
  chassis.wait_drive();
  chassis.set_drive_pid(40, DRIVE_SPEED - 10);
  chassis.wait_drive();
  chassis.set_turn_pid(230, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED);
  chassis.wait_drive();
  
}

void skills()
{
  pros::Task sortTask(ColorSortFunction);
  intakeOn = false;

  // score on stake
  rotSen.Zero();
  armPos = arm.REACH;
  chassis.set_drive_pid(-5, DRIVE_SPEED / 2);
  chassis.wait_drive();
  pros::delay(500);
  armPos = arm.DOCK;

  //drive into 1st mogo 
  chassis.set_drive_pid(-40, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-60, DRIVE_SPEED - 30);
  chassis.wait_drive();
  m.set_value(true);
  intakeOn = true;
  pros::delay(450);

  // drive to 1st ring
  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(58, DRIVE_SPEED);
  chassis.wait_drive();

  // drive to second ring
  chassis.set_turn_pid(270, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(74, DRIVE_SPEED- 30);
  chassis.wait_drive();

  // drive to wall stake
  chassis.set_drive_pid(-10, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(49, DRIVE_SPEED);
  chassis.wait_drive();
  armPos = arm.LOAD;

  //score on wall stake 
  chassis.set_turn_pid(270,TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(20, DRIVE_SPEED);
  chassis.wait_drive();
  intakeOn = false;
  armPos = arm.SCORE;
  pros::delay(600);
  chassis.set_drive_pid(-39, DRIVE_SPEED);
  chassis.wait_drive();

  // drives to corner rings
  chassis.set_turn_pid(360,TURN_SPEED);
  chassis.wait_drive();
  armPos = arm.DOCK;
  intakeOn = true;
  chassis.set_drive_pid(90, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(55, DRIVE_SPEED - 50);
  chassis.wait_drive();
  chassis.set_drive_pid(-35, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(270,TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(22, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(150,TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-34, DRIVE_SPEED);
  chassis.wait_drive();
  pros::delay(200);
  m.set_value(false);

  //drive to 2nd mogo
  chassis.set_drive_pid(10, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(270, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-150, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-50, DRIVE_SPEED - 40);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(400);

  // drive to 1st ring
  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(58, DRIVE_SPEED);
  chassis.wait_drive();

  // drive to 2nd 
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(49, DRIVE_SPEED);
  chassis.wait_drive();

  // drive to wall stake
  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(49, DRIVE_SPEED);
  chassis.wait_drive();
  armPos = arm.LOAD;

  //score on wall stake
   chassis.set_turn_pid(90,TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(47, DRIVE_SPEED);
  chassis.wait_drive();
  pros::delay(300);
  intakeOn = false;
  armPos = arm.SCORE;
  pros::delay(600);
  chassis.set_drive_pid(-39, DRIVE_SPEED);
  chassis.wait_drive();

  //drive to corner rings
  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
  armPos = arm.DOCK;
  intakeOn = true;
  chassis.set_drive_pid(90, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(50, DRIVE_SPEED - 59);
  chassis.wait_drive();
  chassis.set_drive_pid(-32, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(90,TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(31, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(210,TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-37, DRIVE_SPEED);
  chassis.wait_drive();
  pros::delay(200);
  m.set_value(false);
  chassis.set_drive_pid(15, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(0, TURN_SPEED);
}
 



#pragma region trash autons
///
// Turn Example
///
void turn_example()
{
  // The first parameter is target degrees
  // The second parameter is max speed the robot will drive at

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();

  // chassis.set_turn_pid(45, TURN_SPEED);
  // chassis.wait_drive();

  // chassis.set_turn_pid(0, TURN_SPEED);
  // chassis.wait_drive();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed()
{
  // wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  // When the robot gets to -6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_until(-6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();
}

///
// Swing Example
///
void swing_example()
{
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is target degrees
  // The third parameter is speed of the moving side of the drive

  chassis.set_swing_pid(ez::LEFT_SWING, 45, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(12);

  chassis.set_swing_pid(ez::RIGHT_SWING, 0, SWING_SPEED);
  chassis.wait_drive();
}

///
// Interference example
///
void tug(int attempts)
{
  for (int i = 0; i < attempts - 1; i++)
  {
    // Attempt to drive backwards
    printf("i - %i", i);
    chassis.set_drive_pid(-12, 127);
    chassis.wait_drive();

    // If failsafed...
    if (chassis.interfered)
    {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(-2, 20);
      pros::delay(1000);
    }
    // If robot successfully drove back, return
    else
    {
      return;
    }
  }
}

// If there is no interference, robot will drive forward and turn 90 degrees.
// If interfered, robot will drive forward and then attempt to drive backwards.
void interfered_example()
{
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  if (chassis.interfered)
  {
    tug(3);
    return;
  }

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
}

/*
 * Autonomous Routines designed in EZ Template for 29295A's Team Bot.
 * LemLib is Currently Being Tested And Could Replace Current Code (false information)
 * Autos Are in the Following Order:
 * Defensive
 * Offensive
 * Skills
 * Remember, We Ball.
 */

const int defDriveSpeed = 110;
const int defTurnSpeed = 110;

void defAuton()
{
  // variables for intake, blocker, and wings
  pros::Motor intake(1, pros::E_MOTOR_GEARSET_06);
  pros::ADIDigitalOut blocker('A');
  pros::ADIDigitalOut rWing('B');
  pros::ADIDigitalOut lWing('C');

  int intakeTime = 320;

  // outtake preload
  intake = -70;

  pros::delay(300);
  intake = -127;
  pros::delay(intakeTime);
  intake = 0;

  // turn onto first triball]
  chassis.set_turn_pid(-70, defTurnSpeed);
  chassis.wait_drive();

  // drive to and intake it, while staying on line
  intake = 127;

  chassis.set_drive_pid(-114, defDriveSpeed);
  chassis.wait_drive();

  intake = 30;

  // turn to other ball to hit over with wing
  chassis.set_turn_pid(-179, defTurnSpeed);
  chassis.wait_drive();

  // drop left wing
  lWing.set_value(true);

  // drive at and hit over triball
  chassis.set_drive_pid(45, defDriveSpeed);
  chassis.wait_drive();

  // retrack wing
  lWing.set_value(false);

  // back up for turn
  chassis.set_drive_pid(-20, defDriveSpeed);
  chassis.wait_drive();

  // turn to point where triball will be dropped off
  chassis.set_turn_pid(-240, defTurnSpeed);
  chassis.wait_drive();

  // drive to outtake point there
  chassis.set_drive_pid(-55, defDriveSpeed);
  chassis.wait_drive();

  // turn to outtake
  chassis.set_turn_pid(-313, defTurnSpeed);
  chassis.wait_drive();

  // outtake middle ball
  intake = -127;
  pros::delay(intakeTime);
  intake = 0;

  // turn back to line up with back wall
  chassis.set_turn_pid(-10, 127);
  chassis.wait_drive();

  // drive there
  chassis.set_drive_pid(14, defDriveSpeed);
  chassis.wait_drive();

  // turn to get lined up with matchload zone
  chassis.set_turn_pid(-90, 127);
  chassis.wait_drive();

  // swing to grab corner ball
  chassis.set_swing_pid(RIGHT_SWING, -135, defTurnSpeed);
  chassis.wait_drive();

  // drop right wing to clear
  rWing.set_value(true);

  // clear
  chassis.set_drive_pid(14, defDriveSpeed);
  chassis.wait_drive();

  // close wing
  rWing.set_value(false);

  // swing to line up and clear
  chassis.set_swing_pid(RIGHT_SWING, -180, defTurnSpeed);
  chassis.wait_drive();

  // drive forward slightly
  chassis.set_drive_pid(27, defDriveSpeed);
  chassis.wait_drive();

  // left wing drops
  lWing.set_value(true);

  // clear side
  chassis.set_drive_pid(50, defDriveSpeed);
  chassis.wait_drive();
}

// runs 15 offensive autonomous routine for the game.
void offAuton()
{
  pros::Motor intake(1, pros::E_MOTOR_GEARSET_06);
  pros::ADIDigitalOut blocker('A');
  pros::ADIDigitalOut rWing('B');
  pros::ADIDigitalOut lWing('C');

  int intakeTime = 320;

  // intake and move to first triball
  intake.move(127);
  chassis.set_drive_pid(-163, 127);
  chassis.wait_drive();
  intake.move(30);

  // swing to outake
  chassis.set_turn_pid(-53, 127);
  chassis.wait_drive();

  lWing.set_value(true);

  // slam
  chassis.set_drive_pid(90, 127);
  chassis.wait_drive();

  // back
  chassis.set_drive_pid(-35, 127);
  chassis.wait_drive();

  lWing.set_value(false);

  chassis.set_turn_pid(-233, 127);
  chassis.wait_drive();

  intake.move(0);

  chassis.set_drive_pid(-54, 127);
  chassis.wait_drive();

  // back up slightly
  chassis.set_drive_pid(30, 127);
  chassis.wait_drive();

  // turn to snatch
  chassis.set_turn_pid(-79, 127);
  chassis.wait_drive();

  // grab 3rd ball
  intake.move(127);
  chassis.set_drive_pid(-68, 127);
  chassis.wait_drive();
  intake.move(30);

  // swing to outtake into corner
  chassis.set_turn_pid(-198, 127);
  chassis.wait_drive();

  // drive to drop off point
  chassis.set_drive_pid(-64, 127);
  chassis.wait_drive();

  // turn to outtake to corner
  chassis.set_turn_pid(-250, 127);
  chassis.wait_drive();

  // outtake
  intake.move(-127);
  pros::delay(250);
  intake.move(0);

  // turn to grab preload and corner ball
  chassis.set_turn_pid(-330, 127);
  chassis.wait_drive();

  // drive up
  chassis.set_drive_pid(40, 127);
  chassis.wait_drive();

  /*
    // turn
    chassis.set_turn_pid(-420, 127);
    chassis.wait_drive();
  */
  // swing to grab corner ball
  chassis.set_swing_pid(RIGHT_SWING, -466, 127);
  chassis.wait_drive();

  // right wing extend
  rWing.set_value(true);
  // extend left wing to grab pre load
  lWing.set_value(true);
  // drive and clear
  chassis.set_drive_pid(25, 127);
  chassis.wait_drive();

  // close corner wing
  rWing.set_value(false);

  chassis.set_drive_pid(32, 127);
  chassis.wait_drive();

  // swing to slam in the three triballs
  chassis.set_swing_pid(RIGHT_SWING, -510, 127);
  chassis.wait_drive();

  // slam in triballs
  chassis.set_drive_pid(32, 127);
  chassis.wait_drive();

  // backup
  chassis.set_drive_pid(-15, 127);
  chassis.wait_drive();

  // double slam
  chassis.set_drive_pid(20, 127);
  chassis.wait_drive();

  // close left wing
  lWing.set_value(false);

  while (1)
  {
    pros::delay(100);
  }

  chassis.set_drive_pid(-10, 127);
  chassis.wait_drive();

  chassis.set_swing_pid(RIGHT_SWING, -239, 127);
  chassis.wait_drive();

  chassis.set_drive_pid(50, 127);
  chassis.wait_drive();

  lWing.set_value(true);

  chassis.set_drive_pid(40, 127);
  chassis.wait_drive();
}
/*
  Psuedo-Code (Skills)
  - Allign With Location For Match Loads
  - Match Load Certain Ammount Using a Delay (Or Millis if Necessary) With Cata Going
  - Allign and Drive Forward on an Arc To Allign Perp With Middle Bar
  - Extend Pnuematic Arm and Grab Corner Ball
  - Close Arm and Ram in On Arc
  - Back Out and Arc To Far Side
  - Both Arms Extend And Push On A Slight Angle
  - Back Up and Arc to Final Position
  - Push Other Side In To Finish
*/

// skills autonomous routine //no while loop is needed as it is preset to 1 minute
void skillsAuton()
{ // drive into mogo and score preload
  // intakeChangeSpeed(127);
  chassis.set_drive_pid(-8, DRIVE_SPEED - 60);
  chassis.wait_drive();
  m.set_value(true);
  pros::delay(800);

  // turn to corner
  chassis.set_turn_pid(75, TURN_SPEED - 10);
  pros::delay(400);
  // intake
  chassis.wait_drive();

  chassis.set_drive_pid(-120, DRIVE_SPEED - 60);
  pros::delay(5000);
  chassis.set_drive_pid(0, 0);
  pros::delay(5000);
  // intakeStop();
}
#pragma endregion