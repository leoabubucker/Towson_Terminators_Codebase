/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       leoku                                                     */
/*    Created:      Mon Oct 03 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

//Vex References
#include "vex.h"
using namespace vex;

//Special Program References
#include <iostream>
#include "special_prog.h"
#include <stdlib.h>
#include "auton-funcs.h"

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
  lFlywheel.resetPosition();
  rFlywheel.resetPosition();
  intakeMotor.resetPosition();
  endgameMotor.resetPosition();
  rollerMotor.resetPosition();
  

  //Sets Motor Velocity to 100%
  leftFront.setVelocity(100, percent);
  leftBack.setVelocity(100, percent);
  rightFront.setVelocity(100, percent);
  rightBack.setVelocity(100, percent);
  lFlywheel.setVelocity(100, percent);
  rFlywheel.setVelocity(100, percent);
  intakeMotor.setVelocity(100, percent);
  endgameMotor.setVelocity(100, percent);
  rollerMotor.setVelocity(100, percent);
  


  //Sets Motor Torque to 100%
  leftFront.setMaxTorque(100, percent);
  leftBack.setMaxTorque(100, percent);
  rightFront.setMaxTorque(100, percent);
  rightBack.setMaxTorque(100, percent);
  lFlywheel.setMaxTorque(100, percent);
  rFlywheel.setMaxTorque(100, percent);
  intakeMotor.setMaxTorque(100, percent);
  endgameMotor.setMaxTorque(100, percent);
  rollerMotor.setMaxTorque(100, percent);
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
  drive(12);
  turnRight(90);
  drive(-36);
  rollerMotor.spinFor(vex::directionType::rev, 100, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
}
  // drive(-(2 * 12)); // two ft rev
  // vex::task testpid(drivePID);
  // resetDriveSensors = true;
  // move_to_angle(90);
// 0  // vex::task::sleep(1000);
  // leftFront.setVelocity(50, percent);
  // leftBack.setVelocity(50, percent);
  // rightFront.setVelocity(50, percent);
  // rightBack.setVelocity(50, percent);

  // leftFront.spin(reverse, 50, percent);
  // leftBack.spin(reverse, 50, percent);
  // rightFront.spin(forward, 50, percent);
  // rightBack.spin(forward, 50, percent);

  // task::sleep(1500);
  
  // leftFront.stop();
  // leftBack.stop();
  // rightFront.stop();
  // rightBack.stop();
  // driveFwd(1500);
  // driveRev(1500);
  // drive(2 * 12); //drives 2 feet fwd
  // turn(90); //90 Degrees Right Turn
  // turnLeft(90);


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
    // pos_to_deg_program();

  // wheel controls
  leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
  leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
  rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
  rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
/*leftF.spin(fwd.axi3)
leftb.spin(fwd.axi3)
rightF.spin(rvs.axi3)
ridghB.spin(rvs.axis3)

leftF.spin(fwd.axi4)
leftb.spin(rvs.axi4)
rightF.spin(fwd.axi4)
ridghB.spin(rvs.axis4)*/
  // intake controls
  if(Controller1.ButtonL1.pressing()){
    intakeMotor.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  }
  else if(Controller1.ButtonL2.pressing()){
    intakeMotor.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  }
  else{
    intakeMotor.stop(vex::brakeType::hold);
  }

  // Fly wheel controls
  if(Controller1.ButtonUp.pressing()){
    rFlywheel.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    lFlywheel.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  }
  else if(Controller1.ButtonDown.pressing()){
     rFlywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
     lFlywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  }
  else{
    rFlywheel.stop(vex::brakeType::hold);
    lFlywheel.stop(vex::brakeType::hold);
  }

  //Roller Motor Controls
  if(Controller1.ButtonX.pressing()){
    rollerMotor.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    rollerMotor2.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  }
  else if(Controller1.ButtonB.pressing()){
     rollerMotor.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
     rollerMotor2.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  }
  else{
    rollerMotor.stop(vex::brakeType::hold);
    rollerMotor2.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  }
  
  //Endgame Motor Controls
  bool isEndgame = false;
  if(Controller1.ButtonA.pressing()){
    isEndgame = true;
  }
  else{
    isEndgame = false;
  }
  if(Controller1.ButtonY.pressing()){
    if(isEndgame){
      endgameMotor.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    }
  }
  else{
    endgameMotor.stop(vex::brakeType::hold);
  }

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
