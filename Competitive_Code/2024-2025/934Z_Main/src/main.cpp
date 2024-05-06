/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       leoku                                                     */
/*    Created:      5/5/2024, 9:29:54 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <string>
using namespace vex;
using namespace std;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
controller Controller1 = controller(primary);
motor flexwheelIntake = motor(PORT12, ratio18_1, false);
motor chainIntake = motor(PORT17, ratio18_1, false);
motor leftArm = motor(PORT13, ratio36_1, false);
motor rightArm = motor(PORT18, ratio36_1, true);
motor leftBack = motor(PORT5, ratio18_1, false);
motor leftFront = motor(PORT11, ratio18_1, false);
motor rightBack = motor(PORT10, ratio18_1, true);
motor rightFront = motor(PORT20, ratio18_1, true);
motor_group allMotors = motor_group(flexwheelIntake, chainIntake, leftArm, rightArm, leftBack, leftFront, rightBack, rightFront);
motor_group driveMotors = motor_group(leftBack, leftFront, rightBack, rightFront);
motor_group leftDriveMotors = motor_group(leftFront, leftBack);
motor_group rightDriveMotors = motor_group(rightFront, rightBack);
motor_group nonDriveMotors = motor_group(flexwheelIntake, chainIntake, leftArm, rightArm);
motor_group intakeMotors = motor_group(flexwheelIntake, chainIntake);
motor_group armMotors = motor_group(leftArm, rightArm);
drivetrain robotDrive = drivetrain(leftDriveMotors, rightDriveMotors, 220, 406, 406, vex::distanceUnits::mm, 0.5); //vex::drivetrain::drivetrain	(	motor_group leftMotors, motor_group rightMotors, double 	wheelTravel = wheel circumference, double trackWidth = middle of left wheel to middle of right wheel, double 	wheelBase = middle of front wheel to middle of back wheel, distanceUnits unit = distanceUnits::mm, double	externalGearRatio = nondriven gear / driven gear)		

void drive(int inches, std::string direction){
  if(direction == "fwd"){
    robotDrive.driveFor(vex::directionType::fwd, inches, vex::distanceUnits::in);
  }
  else{
    robotDrive.driveFor(vex::directionType::rev, inches, vex::distanceUnits::in);
  }
}

void turn(int degrees, std::string direction){
  if(direction == "left"){
    robotDrive.turnFor(vex::turnType::left, degrees, vex::rotationUnits::deg);
  }
  else{
    robotDrive.turnFor(vex::turnType::right, degrees, vex::rotationUnits::deg);
  }
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  allMotors.setMaxTorque(100, vex::percentUnits::pct);
  allMotors.setVelocity(100, vex::percentUnits::pct);
  armMotors.setVelocity(50, vex::velocityUnits::pct);
  allMotors.setTimeout(5, vex::timeUnits::sec);
  nonDriveMotors.setStopping(vex::brakeType::hold);
  robotDrive.setTurnVelocity(50, vex::velocityUnits::pct);
  allMotors.resetPosition();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  drive(24, "fwd");
  turn(90, "left");
  drive(12, "rev");
  turn(45, "right");
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    //Drive Controls
    leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
    leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
    rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
    rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 


    //Intake Controls
    if(Controller1.ButtonX.pressing() && Controller1.ButtonL1.pressing()){
      flexwheelIntake.spin(vex::directionType::fwd);
    }
    else if(Controller1.ButtonX.pressing() && Controller1.ButtonL2.pressing()){
      flexwheelIntake.spin(vex::directionType::rev);
    }
    else if(Controller1.ButtonA.pressing() && Controller1.ButtonL1.pressing()){
      chainIntake.spin(vex::directionType::fwd);
    }
    else if(Controller1.ButtonA.pressing() && Controller1.ButtonL2.pressing()){
      chainIntake.spin(vex::directionType::rev);
    }
    else if(Controller1.ButtonL1.pressing()){
      intakeMotors.spin(vex::directionType::rev);
    }
    else if(Controller1.ButtonL2.pressing()){
      intakeMotors.spin(vex::directionType::rev);
    }
    else{
      intakeMotors.stop();
    }

    //Arm Controls
    if(Controller1.ButtonR1.pressing()){
      if(armMotors.position(vex::rotationUnits::deg) < 200){
        armMotors.spinToPosition(200, vex::rotationUnits::deg);
      }
      else if(armMotors.position(vex::rotationUnits::deg) < 400){
        armMotors.spinToPosition(400, vex::rotationUnits::deg);
      }
      else if(armMotors.position(vex::rotationUnits::deg) < 600){
        armMotors.spinToPosition(600, vex::rotationUnits::deg);
      }
      else if(armMotors.position(vex::rotationUnits::deg) < 800){
        armMotors.spinToPosition(800, vex::rotationUnits::deg);
      }
    } 
    else if(Controller1.ButtonR2.pressing()){
      if(armMotors.position(vex::rotationUnits::deg) >=800){
        armMotors.spinToPosition(600, vex::rotationUnits::deg);
      }
      else if(armMotors.position(vex::rotationUnits::deg) >= 600){
        armMotors.spinToPosition(400, vex::rotationUnits::deg);
      }
      else if(armMotors.position(vex::rotationUnits::deg) >= 400){
        armMotors.spinToPosition(200, vex::rotationUnits::deg);
      }
      else if(armMotors.position(vex::rotationUnits::deg) >= 200){
        armMotors.spinToPosition(0, vex::rotationUnits::deg);
      }
    }    
    
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
