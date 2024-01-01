/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftBackWheel        motor         16              
// rightBackWheel       motor         19              
// leftFrontArm         motor         1               
// rightFrontArm        motor         3               
// Controller1          controller                    
// leftBackClamp        motor29       B               
// rightBackClamp       motor29       A               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

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
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  //Resets Motors

  leftBackWheel.resetRotation();
  leftBackWheel.resetPosition(); 


  rightBackWheel.resetRotation();
  rightBackWheel.resetPosition();

  leftFrontArm.resetRotation();
  leftFrontArm.resetPosition();

  rightFrontArm.resetRotation();
  rightFrontArm.resetPosition();
  //Sets Motor Velocity
    //Wheel Velocity
    leftBackWheel.setVelocity(50, percent);
    rightBackWheel.setVelocity(50, percent);
    //Other Motor Velocity
    leftFrontArm.setVelocity(100, percent);
    rightFrontArm.setVelocity(100, percent);
    leftBackClamp.setVelocity(100, percent);
    rightBackClamp.setVelocity(100, percent);
  //Sets all motor torque to 100%
    leftBackWheel.setMaxTorque(100, percent);
    rightBackWheel.setMaxTorque(100, percent);
    leftFrontArm.setMaxTorque(100, percent);
    rightFrontArm.setMaxTorque(100, percent);
    //Confirms end of pre-autonomous
Brain.Screen.print("Pre-Autonomous period completed successfully");
Controller1.Screen.print("Pre-Autonomous period completed successfully");
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
//Lowers Front Arm
leftFrontArm.startRotateFor(vex::directionType::fwd, 700, vex::rotationUnits::deg);
rightFrontArm.rotateFor(vex::directionType::rev, 700, vex::rotationUnits::deg);
//Moves forward towards neutral zone
leftBackWheel.startRotateFor(vex::directionType::fwd, 1500, vex::rotationUnits::deg);
rightBackWheel.rotateFor(vex::directionType::rev, 1500, vex::rotationUnits::deg);
//Turns Right 90 Degrees
leftBackWheel.startRotateFor(vex::directionType::fwd, 750, vex::rotationUnits::deg);
rightBackWheel.rotateFor(vex::directionType::fwd, 750, vex::rotationUnits::deg);
//Moves forwards towards left neutral goal
leftBackWheel.startRotateFor(vex::directionType::fwd, 500, vex::rotationUnits::deg);
rightBackWheel.rotateFor(vex::directionType::rev, 500, vex::rotationUnits::deg);
//Lifts Front Arm
leftFrontArm.startRotateFor(vex::directionType::rev, 700, vex::rotationUnits::deg);
rightFrontArm.rotateFor(vex::directionType::fwd, 700, vex::rotationUnits::deg);
//Lowers Front Arm
leftFrontArm.startRotateFor(vex::directionType::fwd, 700, vex::rotationUnits::deg);
rightFrontArm.rotateFor(vex::directionType::rev, 700, vex::rotationUnits::deg);
//Moves forwards towards middle neutral goal
leftBackWheel.startRotateFor(vex::directionType::fwd, 500, vex::rotationUnits::deg);
rightBackWheel.rotateFor(vex::directionType::rev, 500, vex::rotationUnits::deg);
//Lifts Front Arm
leftFrontArm.startRotateFor(vex::directionType::rev, 700, vex::rotationUnits::deg);
rightFrontArm.rotateFor(vex::directionType::fwd, 700, vex::rotationUnits::deg);
//Lowers Front Arm
leftFrontArm.startRotateFor(vex::directionType::fwd, 700, vex::rotationUnits::deg);
rightFrontArm.rotateFor(vex::directionType::rev, 700, vex::rotationUnits::deg);
//Moves forwards towards right neutral goal
leftBackWheel.startRotateFor(vex::directionType::fwd, 500, vex::rotationUnits::deg);
rightBackWheel.rotateFor(vex::directionType::rev, 500, vex::rotationUnits::deg);
//Lifts Front Arm
leftFrontArm.startRotateFor(vex::directionType::rev, 700, vex::rotationUnits::deg);
rightFrontArm.rotateFor(vex::directionType::fwd, 700, vex::rotationUnits::deg);
//Turns Right 90 Degrees
leftBackWheel.startRotateFor(vex::directionType::fwd, 750, vex::rotationUnits::deg);
rightBackWheel.rotateFor(vex::directionType::fwd, 750, vex::rotationUnits::deg);
//Moves forward towards alliance zone
leftBackWheel.startRotateFor(vex::directionType::fwd, 1500, vex::rotationUnits::deg);
rightBackWheel.rotateFor(vex::directionType::rev, 1500, vex::rotationUnits::deg);
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
// wheel controls
   leftBackWheel.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct); 
   rightBackWheel.spin(vex::directionType::rev, Controller1.Axis2.position(), vex::velocityUnits::pct); 
// Front Arm Controls
if  (Controller1.ButtonUp.pressing())
{//Lifts Front Arm
  leftFrontArm.spin(vex::directionType::rev, 50, vex::velocityUnits::pct);
  rightFrontArm.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);  
}
else if (Controller1.ButtonDown.pressing())
{ //Lowers Front Arm
  leftFrontArm.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);
  rightFrontArm.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
}
else 
{//Holds Arm Position
  leftFrontArm.stop(vex::brakeType::hold);
  rightFrontArm.stop(vex::brakeType::hold);
}

//Back Clamp Controls
if  (Controller1.ButtonL1.pressing())
{//Lifts Clamp
  leftBackClamp.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
  rightBackClamp.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
}
else if (Controller1.ButtonL2.pressing())
{ //Lowers Clamp
  leftBackClamp.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
  rightBackClamp.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
}
else 
{//Holds Clamp Position
  leftBackClamp.stop();
  rightBackClamp.stop();
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
