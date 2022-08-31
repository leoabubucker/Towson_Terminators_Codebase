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
// LeftFront            motor         1               
// LeftBack             motor         2               
// RightFront           motor         3               
// RightBack            motor         4               
// FrontArm1            motor         7               
// FrontArm2            motor         13              
// BackArm1             motor         18              
// BackArm2             motor         20              
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

  // All activities that occur before the competition starts
  LeftFront.resetRotation();
  LeftBack.resetRotation();
  RightFront.resetRotation();
  RightBack.resetRotation();
  FrontArm1.resetRotation();
  FrontArm2.resetRotation();
  BackArm1.resetRotation();
  BackArm2.resetRotation();
  
  //Intro Script
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Pre-Autonomous Period Has Begun");
  wait(5000, msec);
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Hi. My name is Leo and I coded for .\n"); 
  Brain.Screen.newLine();
  Brain.Screen.print("this vex competition. This should be used \n");
  Brain.Screen.newLine();
  Brain.Screen.print("for the JV 1-22-22 Competition.\n");
  Brain.Screen.newLine();
  Brain.Screen.print("I hope everything works as this is\n");
  Brain.Screen.newLine();
  Brain.Screen.print("my first time coding. Please let me know'\n");
  Brain.Screen.newLine();
  Brain.Screen.print("if there are any glitches/bugs. Enjoy!\n");
  wait(5000, msec);
  Brain.Screen.clearScreen();
  // Tells Battery Stats
  Brain.Screen.setCursor(1, 1); 
  Brain.Screen.print("Battery Capacity is: " "%f",Brain.Battery.capacity());
  Brain.Screen.newLine();
  Brain.Screen.print("Current Battery is: " "%f",Brain.Battery.current());
  Brain.Screen.newLine();
  Brain.Screen.print("Current Battery Voltage is: ""%f",Brain.Battery.voltage());
  wait(5000, msec);
  Brain.Screen.clearScreen();
  // Tells All Motor Temperatures
  Brain.Screen.setCursor(1, 1); 
  Brain.Screen.print("LeftFront Motor Temp is: " "%f",LeftFront.temperature(percent));
  Brain.Screen.newLine();
  Brain.Screen.print("LeftBack Motor Temp is: " "%f",LeftBack.temperature(percent));
  Brain.Screen.newLine();
  Brain.Screen.print("RightFront Motor Temp is: " "%f",RightFront.temperature(percent));
  Brain.Screen.newLine();
  Brain.Screen.print("RightBack Motor Temp is: " "%f",RightBack.temperature(percent));
  Brain.Screen.newLine();
  Brain.Screen.print("FrontArm1 Motor Temp is: " "%f",FrontArm1.temperature(percent));
  Brain.Screen.newLine();
  Brain.Screen.print("FrontArm2 Motor Temp is: " "%f",FrontArm2.temperature(percent));
  Brain.Screen.newLine();
  Brain.Screen.print("BackArm1 Motor Temp is: " "%f",BackArm1.temperature(percent));
  Brain.Screen.newLine();
  Brain.Screen.print("BackArm2 Motor Temp is: " "%f",BackArm2.temperature(percent));
  Brain.Screen.newLine();
  wait(5000, msec);
  Brain.Screen.clearScreen();
  //Sets Motor Velocity to 100%
  LeftFront.setVelocity(100, percent);
  LeftBack.setVelocity(100, percent);
  RightFront.setVelocity(100, percent);
  RightBack.setVelocity(100, percent);
  FrontArm1.setVelocity(100, percent);
  FrontArm2.setVelocity(100, percent);
  BackArm1.setVelocity(100, percent);
  BackArm2.setVelocity(100, percent);
  //Sets Motor Torque to 100%
  LeftFront.setMaxTorque(100, percent);
  LeftBack.setMaxTorque(100, percent);
  RightFront.setMaxTorque(100, percent);
  RightBack.setMaxTorque(100, percent);
  FrontArm1.setMaxTorque(100, percent);
  FrontArm2.setMaxTorque(100, percent);
  BackArm1.setMaxTorque(100, percent);
  BackArm2.setMaxTorque(100, percent);
  //Prints Results to Brain
  Brain.Screen.setCursor(1, 1); 
  Brain.Screen.print("All Motor Velocity and Torque");
  Brain.Screen.newLine();
  Brain.Screen.print("set to 100%");
  wait(5000, msec);
  Brain.Screen.clearScreen();
//Announces end of Pre-Autonomous
Brain.Screen.setCursor(1, 1); 
Brain.Screen.print("Pre-Autonomous Period Has Ended.");
wait(5000, msec);
Brain.Screen.clearScreen();
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

//Goes Forward For a 365 Degree Axle Rotation
// Lowers Arms
FrontArm1.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg);
FrontArm2.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg); 
BackArm1.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg);
BackArm2.startRotateFor(vex::directionType::rev, 365, vex::rotationUnits::deg);
wait(1, sec);
 LeftFront.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg); 
 LeftBack.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg);
 RightFront.startRotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg);
 RightBack.rotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg);
 wait(1, sec);
 //Lifts Front Arm
 FrontArm1.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg);
FrontArm2.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg); 
wait(1 ,sec);
// Turns Right 90 Degrees
LeftFront.setVelocity(100, percent);
LeftBack.setVelocity(100, percent);
RightFront.setVelocity(100, percent);
RightBack.setVelocity(100, percent);
LeftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
LeftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(2 ,sec);
//Goes Backwards For a 180 Degrees Axle Rotation 
 LeftFront.startRotateFor(vex::directionType::rev, 500, vex::rotationUnits::deg); 
 LeftBack.startRotateFor(vex::directionType::rev, 500, vex::rotationUnits::deg);
 RightFront.startRotateFor(vex::directionType::fwd, 500, vex::rotationUnits::deg);
 RightBack.startRotateFor(vex::directionType::fwd, 500, vex::rotationUnits::deg);
 wait(1, sec);
// Lifts Back Arm
BackArm1.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg);
BackArm1.startRotateFor(vex::directionType::fwd, 365, vex::rotationUnits::deg);
wait(1 ,sec); 
//Turns Right 90 Degrees
LeftFront.setVelocity(100, percent);
LeftBack.setVelocity(100, percent);
RightFront.setVelocity(100, percent);
RightBack.setVelocity(100, percent);
LeftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
LeftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(1, sec); 
//Goes Forward For a 365 Degrees Axle Rotation 
 LeftFront.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg); 
 LeftBack.startRotateFor(vex::directionType::fwd, 1000, vex::rotationUnits::deg);
 RightFront.startRotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg);
 RightBack.startRotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg);
 wait(1, sec);
 //Turns Right 90 Degrees
LeftFront.setVelocity(100, percent);
LeftBack.setVelocity(100, percent);
RightFront.setVelocity(100, percent);
RightBack.setVelocity(100, percent);
LeftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
LeftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(1, sec); 
//Turns Right 90 Degrees
LeftFront.setVelocity(100, percent);
LeftBack.setVelocity(100, percent);
RightFront.setVelocity(100, percent);
RightBack.setVelocity(100, percent);
LeftFront.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
LeftBack.startRotateFor(vex::directionType::fwd, 900, vex::rotationUnits::deg);
wait(1, sec); 
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
   LeftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
   LeftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
   RightFront.spin(vex::directionType::rev, Controller1.Axis2.position(), vex::velocityUnits::pct); 
   RightBack.spin(vex::directionType::rev, Controller1.Axis2.position(), vex::velocityUnits::pct); 
//Front Arm Controls

if  (Controller1.ButtonL1.pressing())
{//Lifts Arm
  FrontArm1.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
  FrontArm2.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
}
else if (Controller1.ButtonR1.pressing())
{ //Lowers Arm
  FrontArm1.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
  FrontArm2.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
}
else 
{//Holds Arm Position
  FrontArm1.stop(vex::brakeType::hold);
  FrontArm2.stop(vex::brakeType::hold);
}
if  (Controller1.ButtonL2.pressing())
{//Lifts Arm
  BackArm1.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
  BackArm2.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct); 
}
else if (Controller1.ButtonR2.pressing())
{ //Lowers Arm
  BackArm1.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
  BackArm2.spin(vex::directionType::rev, 50, vex::velocityUnits::pct); 
}
else 
{//Holds Arm Position
  BackArm1.stop(vex::brakeType::hold);
  BackArm2.stop(vex::brakeType::hold);
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
