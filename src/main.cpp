#include "main.h"
#include "variables.hpp"
#include <vector>

#include "ToggleButton.hpp"
#include "HoldButton.hpp"
#include "ShiftedButton.hpp"
#include "MogoMech.hpp"
#include "Motor.hpp"
#include "Intake.hpp"
#include "Stick.hpp"
#include "RotationSensor.hpp"
#include "Arm.hpp"
#include "TapButton.hpp"

/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
Drive chassis(
    // Left Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    {-17, -18, -20} // real
    //{-9, -20} // old

    // Right Chassis Ports (negative port will reverse it!)
    //   the first port is the sensored port (when trackers are not used!)
    ,
    {7, 8, 10} // real
    //{9, 10} // old

    // IMU Port
    ,
    19

    // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
    //    (or tracking wheel diameter)
    ,
    3.25

    // Cartridge RPM
    //   (or tick per rotation if using tracking wheels)
    ,
    600

    // External Gear Ratio (MUST BE DECIMAL)
    //    (or gear ratio of tracking wheel)
    // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
    // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
    ,
    0.57143

    // Uncomment if using tracking wheels
    /*
    // Left Tracking Wheel Ports (negative port will reverse it!)
    // ,{1, 2} // 3 wire encoder
    // ,8 // Rotation sensor

    // Right Tracking Wheel Ports (negative port will reverse it!)
    // ,{-3, -4} // 3 wire encoder
    // ,-9 // Rotation sensor
    */

    // Uncomment if tracking wheels are plugged into a 3 wire expander
    // 3 Wire Port Expander Smart Port
    // ,1
);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
  // Print our branding over your terminal :D
  ez::print_ez_template();

  pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
  chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0.1);                     // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0);                   // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)
  default_constants();                               // Set the drive to your own constants from autons.cpp!
  exit_condition_defaults();                         // Set the exit conditions to your own constants from autons.cpp!

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.set_left_curve_buttons (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used.
  // chassis.set_right_curve_buttons(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({

      // Auton("Example Drive\n\nDrive forward and come back.", drive_example),
      // Auton("Example Turn\n\nTurn 3 times.", turn_example),
      // Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
      // Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
      // Auton("Swing Example\n\nSwing, drive, swing.", swing_example),
      // Auton("Combine all 3 movements", combining_movements),
      // Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
      // Auton("Runs defensive In-Game Autonomous.", defAuton),
      Auton("Runs offensive In-Game Autonomous.", offAuton),
      // Auton("Runs Skills Autonomous.", skillsAuton),
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled()
{
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
  chassis.reset_pid_targets();               // Resets PID targets to 0
  chassis.reset_gyro();                      // Reset gyro position to 0
  chassis.reset_drive_sensor();              // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.

  ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{

  // drive variables to calulate speeds with curve
  double power;
  double powerC;
  double turn;
  double turnC;
  //  variable for # of motors per side
  const int sideMotors = 3;

  // input curve constants
  float pCurve = 0.6;        // curve for fwd/back
  float tCoefficient = 0.75; // curve for turn
  float tCurve = 1.1;        // coefficient for turn

  double e = exp(1); // Euler's constant

  chassis.set_drive_brake(MOTOR_BRAKE_COAST);

  //
  //  set up module drivers
  //
  Stick leftY(master, ANALOG_LEFT_Y);
  Stick leftX(master, ANALOG_LEFT_X);
  Stick rightY(master, ANALOG_RIGHT_Y);
  Stick rightX(master, ANALOG_RIGHT_X);

  ToggleButton button_L2(master, DIGITAL_L2);
  HoldButton shift_Button(master, DIGITAL_R1);
  HoldButton button_R2(master, DIGITAL_R2);
  TapButton button_L1(master, DIGITAL_L1);
  TapButton button_up(master, DIGITAL_UP);
  TapButton button_down(master, DIGITAL_DOWN);

  // ShiftedButton clampActivator(button_L2, shift_Button);

  MogoMech mogo('A');
  Intake intake(Motor(-6, pros::E_MOTOR_GEARSET_06));
  RotationSensor rotSen(5);
  Arm arm(Motor(12, pros::E_MOTOR_GEARSET_36), rotSen);

  while (true)
  {
    // tick everything

    // raw sticks
    leftY.Tick();
    leftX.Tick();
    rightY.Tick();
    rightX.Tick();

    // raw buttons
    button_L2.Tick();
    button_R2.Tick();
    shift_Button.Tick();
    button_L1.Tick();
    button_up.Tick();
    button_down.Tick();

    // motors
    intake.Tick();

    //sensors
    rotSen.Tick();

    // arm
    arm.Tick();

#pragma region driver code

    // calculates power curve for joystick
    power = leftY.GetPosition();
    powerC = ((1 - pCurve) * power) + ((pCurve * pow(power, 3)) / 16129); // 16129 is 127^2
    // modelled after https://www.desmos.com/calculator/asjs86sdpy

    // gets turn value and calculates curve
    turn = -rightX.GetPosition();
    turnC = tCurve * ((1 - tCoefficient) * turn) + ((tCoefficient * pow(turn, 3)) / 16129);

    // assigns motor speeds
    for (int i = 0; i < sideMotors; i++)
    {
      chassis.left_motors[i].move(powerC - turnC);
      chassis.right_motors[i].move(powerC + turnC);
    }
#pragma endregion

    if (button_L2.IsOn())
    {
      mogo.Activate();
    }
    else
    {
      mogo.Deactivate();
    }

    //
    //  Intake
    //

    if (button_R2.IsPressed())
    {
      if (shift_Button.IsPressed())
      {
        intake.Reverse();
      }
      else
      {
        intake.Forward();
      }
    }
    else
    {
      intake.Stop();
    }

    //
    //    Arm
    //

    if (shift_Button.IsPressed())
    {
      arm.ManualMove(rightY.GetPosition());
    }
    else
    {
      arm.ManualMove(0);
    }

    //
    // temp pid adjust values
    //

    if (button_up.IsPressed())
    {
      arm.ChangeP(0.1);
    }
    if (button_down.IsPressed())
    {
      arm.ChangeP(-0.1);
    }

    arm.SetTarget((Arm::State)(0)); // button_L1.TimesPressed() % 4));

    // ez::print_to_screen("Drive Motor Temp: " + std::to_string(static_cast<int>(chassis.left_motors[0].get_temperature())), 2);
    master.set_text(1, 1, "Rot: " + std::to_string(rotSen.GetPosition()));
   // master.set_text(0, 1, "Arm State: " + (std::to_string(arm.GetState())));
   // master.set_text(1, 1, "Rot: " + std::to_string(arm.rtnPidValue()));
    ez::print_to_screen(std::to_string(rotSen.GetPosition()));
    ez::print_to_screen(std::to_string(arm.GetState()));
    //ez::print_to_screen(std::to_string(arm.rtnPidValue()));

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
