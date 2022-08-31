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
// Controller1          controller                    
// leftFront            motor         4               
// leftBack             motor         2               
// rightFront           motor         3               
// rightBack            motor         19              
// frontLeftClaw        motor         9               
// frontRightClaw       motor         10              
// backLeftClaw         motor         12              
// backRightClaw        motor         5               
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

  //Resets Motor Rotations
  leftFront.resetRotation();
  leftBack.resetRotation();
  rightFront.resetRotation();
  rightBack.resetRotation();
  frontLeftClaw.resetRotation();
  frontRightClaw.resetRotation();
  backLeftClaw.resetRotation();
  backRightClaw.resetRotation();
  //Sets Motor Velocity to 100%
  leftFront.setVelocity(70, percent);
  leftBack.setVelocity(70, percent);
  rightFront.setVelocity(70, percent);
  rightBack.setVelocity(70, percent);
  frontLeftClaw.setVelocity(100, percent);
  frontRightClaw.setVelocity(100, percent);
  backLeftClaw.setVelocity(100, percent);
  backRightClaw.setVelocity(100, percent);
  //Sets Motor Torque to 100%
  leftFront.setMaxTorque(100, percent);
  leftBack.setMaxTorque(100, percent);
  rightFront.setMaxTorque(100, percent);
  rightBack.setMaxTorque(100, percent);
  frontLeftClaw.setMaxTorque(100, percent);
  frontRightClaw.setMaxTorque(100, percent);
  backLeftClaw.setMaxTorque(100, percent);
  backRightClaw.setMaxTorque(100, percent);
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

void autonomous(void) 
{

// Lifts Claws
frontLeftClaw.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg);
frontRightClaw.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg); 
backLeftClaw.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg);
backRightClaw.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg);
wait(1, sec);
// Moves Forward To Right Neutral Goal
 leftFront.startRotateFor(vex::directionType::fwd, 1500, vex::rotationUnits::deg); 
 leftBack.startRotateFor(vex::directionType::fwd, 1500, vex::rotationUnits::deg);
 rightFront.startRotateFor(vex::directionType::rev, 1500, vex::rotationUnits::deg);
 rightBack.rotateFor(vex::directionType::rev, 1500, vex::rotationUnits::deg);
 wait(1, sec);
 //Lowers Front Claws
 frontLeftClaw.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg);
frontRightClaw.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg); 
wait(1 ,sec);
/* Turns Right 90 Degrees
leftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
leftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(1 ,sec);
*/
//Goes Backwards to reach middle neutral goal
 leftFront.startRotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg); 
 leftBack.startRotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg);
 rightFront.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg);
 rightBack.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg);
 wait(1, sec);
 /*
//Lowers Back Claws
backLeftClaw.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg);
backRightClaw.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg);
wait(1 ,sec); 
//Turns Right 90 Degrees
leftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
leftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(1, sec); 
//
//Goes Forward
 leftFront.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg); 
 leftBack.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg);
 rightFront.startRotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg);
 rightBack.startRotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg);
 wait(1, sec);
 //Turns Right 90 Degrees
leftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
leftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(1, sec); 
//Turns Right 90 Degrees
leftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
leftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(1, sec); 
*/
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
   leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
   leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
   rightFront.spin(vex::directionType::rev, Controller1.Axis2.position(), vex::velocityUnits::pct); 
   rightBack.spin(vex::directionType::rev, Controller1.Axis2.position(), vex::velocityUnits::pct); 
//Front Claw Controls

if  (Controller1.ButtonL1.pressing())
{//Lifts Claw
  frontLeftClaw.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
  frontRightClaw.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
}
else if (Controller1.ButtonL2.pressing())
{ //Lowers Claw
  frontLeftClaw.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
  frontRightClaw.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
}
else 
{//Holds Claw Position
  frontLeftClaw.stop(vex::brakeType::hold);
  frontRightClaw.stop(vex::brakeType::hold);
}
if  (Controller1.ButtonR1.pressing())
{//Lifts Claw
  backLeftClaw.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
  backRightClaw.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
}
else if (Controller1.ButtonR2.pressing())
{ //Lowers Claw
  backLeftClaw.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
  backRightClaw.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
}
else 
{//Holds Claw Position
  backLeftClaw.stop(vex::brakeType::hold);
  backLeftClaw.stop(vex::brakeType::hold);
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
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
