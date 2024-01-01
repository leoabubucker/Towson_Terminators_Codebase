/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       leoku                                                     */
/*    Created:      9/13/2023, 2:38:01 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

//Vex References
#include "vex.h"
#include "auton.h"
using namespace vex;

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
  //Resets Motor Encoder Values
  leftFront.resetPosition();
  leftBack.resetPosition();
  rightFront.resetPosition();
  rightBack.resetPosition(); 
  leftSixBarLift.resetPosition();
  rightSixBarLift.resetPosition();
  leftCatapult.resetPosition();
  rightCatapult.resetPosition();

  //Sets Motor Velocities
  leftFront.setVelocity(100, percent);
  leftBack.setVelocity(100, percent);
  rightFront.setVelocity(100, percent);
  rightBack.setVelocity(100, percent);
  leftSixBarLift.setVelocity(50, percent);
  rightSixBarLift.setVelocity(50, percent);
  leftCatapult.setVelocity(100, percent);
  rightCatapult.setVelocity(100, percent);

  //Sets Motor Torque to 100%
  leftFront.setMaxTorque(100, percent);
  leftBack.setMaxTorque(100, percent);
  rightFront.setMaxTorque(100, percent);
  rightBack.setMaxTorque(100, percent);
  leftSixBarLift.setMaxTorque(100, percent);
  rightSixBarLift.setMaxTorque(100, percent);
  leftCatapult.setMaxTorque(100, percent);
  rightCatapult.setMaxTorque(100, percent);
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
// bool enableDrivePID;
void autonomous(void) 
{
  //Blue Starting Position 2 & Red Starting Position 1
  leftSixBarLift.spinTo(645, vex::rotationUnits::deg, false);
  rightSixBarLift.spinTo(645, vex::rotationUnits::deg); 
  // Degrees per wheel = desired turn angle in degrees / gear ratio
  // Degreees per wheel = 90 / (60/48)
  // leftFront.spinFor(vex::directionType::fwd, (90/1.25), vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
  // leftBack.spinFor(vex::directionType::fwd, (90/1.25), vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
  // rightFront.spinFor(vex::directionType::fwd, -(90/1.25), vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
  // rightBack.spinFor(vex::directionType::fwd, -(90/1.25), vex::rotationUnits::deg, 50, vex::velocityUnits::pct, true);
  // enableDrivePID = true;
  // vex::task startDrivePID(drivePID);
  

  
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
  //Reset Six Bar Lift
  leftSixBarLift.spinTo(0, vex::rotationUnits::deg, false);
  rightSixBarLift.spinTo(0, vex::rotationUnits::deg); 
  // User control code here, inside the loop
  while (1) {

  // wheel controls
  leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
  leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
  rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
  rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 

  // if(Controller1.ButtonX.pressing()){
  //   leftFront.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct);
  //   leftBack.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct);
  //   rightFront.spin(vex::directionType::fwd,10, vex::velocityUnits::pct); 
  //   rightBack.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct); 
  //   Brain.Screen.print(leftFront.position(vex::rotationUnits::deg));
  //   Brain.Screen.print(" ");
  //   Brain.Screen.print(leftBack.position(vex::rotationUnits::deg));
  //   Brain.Screen.print(" ");
  //   Brain.Screen.print(rightFront.position(vex::rotationUnits::deg));
  //   Brain.Screen.print(" ");
  //   Brain.Screen.print(rightBack.position(vex::rotationUnits::deg));
  //   Brain.Screen.print(" ");
  // }

  // if(Controller1.ButtonY.pressing()){
  //   Brain.Screen.clearScreen();
  //   Brain.Screen.setCursor(1,1);
  // }
  // if(Controller1.ButtonUp.pressing()){
  //   leftFront.spinFor(vex::directionType::fwd, 951, vex::rotationUnits::deg, false);
  //   leftBack.spinFor(vex::directionType::fwd, 951, vex::rotationUnits::deg, false);
  //   rightFront.spinFor(vex::directionType::fwd, 951, vex::rotationUnits::deg, false);
  //   rightBack.spinFor(vex::directionType::fwd, 951, vex::rotationUnits::deg, true);
  // }

  // if(Controller1.ButtonDown.pressing()){
  //   leftFront.spinTo(0, vex::rotationUnits::deg, false);
  //   rightFront.spinTo(0, vex::rotationUnits::deg, false);
  //   leftBack.spinTo(0, vex::rotationUnits::deg, false);
  //   rightBack.spinTo(0, vex::rotationUnits::deg, true);
  // }

  //Six Bar Lift Controls

  // if(Controller1.ButtonX.pressing()){
  //   if(leftSixBarLift.position(vex::rotationUnits::deg) < 645 && rightSixBarLift.position(vex::rotationUnits::deg) < 645){
  //     leftSixBarLift.spin(vex::directionType::fwd);
  //     rightSixBarLift.spin(vex::directionType::fwd);
  //   }
  // }
  // else if(Controller1.ButtonY.pressing()){
  //   if(leftSixBarLift.position(vex::rotationUnits::deg) > 0 && rightSixBarLift.position(vex::rotationUnits::deg) > 0){
  //     leftSixBarLift.spin(vex::directionType::rev);
  //     rightSixBarLift.spin(vex::directionType::rev);
  //   }
  // }
  if(Controller1.ButtonA.pressing()){
    leftSixBarLift.spinTo(645, vex::rotationUnits::deg, false);
    rightSixBarLift.spinTo(645, vex::rotationUnits::deg);   
  }
  else if(Controller1.ButtonB.pressing()){
    leftSixBarLift.spinTo(0, vex::rotationUnits::deg, false);
    rightSixBarLift.spinTo(0, vex::rotationUnits::deg);     
  }
  else{
    leftSixBarLift.stop(vex::brakeType::hold);
    rightSixBarLift.stop(vex::brakeType::hold);
  }
  // if(Controller1.ButtonA.pressing()){
  // leftSixBarLift.spinTo(645, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
  // rightSixBarLift.spinTo(645, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);  
  // }
  // else if(Controller1.ButtonB.pressing()){
  //   leftSixBarLift.spinTo(0, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
  //   rightSixBarLift.spinTo(0, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);  
  // }
  // else{
  //   leftSixBarLift.stop(vex::brakeType::hold);
  //   rightSixBarLift.stop(vex::brakeType::hold);
  // }


  // if(Controller1.ButtonY.pressing() && isEndgame){
  // Brain.Screen.print(leftSixBarLift.position(vex::rotationUnits::deg));
  // Brain.Screen.print(" ");
  // leftSixBarLift.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct);
  // rightSixBarLift.spin(vex::directionType::fwd, 10, vex::velocityUnits::pct);
  // }
  // else{
  //   leftSixBarLift.stop(vex::brakeType::hold);
  //   rightSixBarLift.stop(vex::brakeType::hold);
  // }




  wait(20, msec);
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
