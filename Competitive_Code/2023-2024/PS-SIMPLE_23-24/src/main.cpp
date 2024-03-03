/*-------------------------------------------------------------------------------------*/
/*                                                                                     */
/*    Module:       main.cpp                                                           */
/*    Author:       leoku                                                              */
/*    Created:      9/13/2023, 2:38:01 PM                                              */
/*    Description:  V5 project                                                         */
/*                                                                                     */
/*-------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       Global Initializations & References                          */
/*                                                                                    */
/*  Necessary #include and using calls, and initializations of global variables       */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

#include "vex.h" //VEX functions and subsequent call to robot-config.h
#include "auton.h" //Autonomous functions
using namespace vex; //VEX References

/*------------------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                                  */
/*                                                                                    */
/*  You may want to perform some actions before the competition starts.               */
/*  Do them in the following function.  You must return from this function            */
/*  or the autonomous and usercontrol tasks will not be started.  This                */
/*  function is only called once after the V5 has been powered on and                 */
/*  not every time that the robot is disabled.                                        */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/


void pre_auton(void) {
  //Resets Motor Encoder Values
  allMotors.resetPosition();

  //Sets Motor Velocities
  allMotors.setVelocity(100, percent);

  //Sets Motor Torque to 100%
  allMotors.setMaxTorque(100, percent);

  //Set Default Brake Type For Non-Drive Motors to Hold
  nonDriveMotors.setStopping(vex::brakeType::hold);
}

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Autonomous Task                                       */
/*                                                                                    */
/*  This task is used to control your robot during the autonomous phase of            */
/*  a VEX Competition.                                                                */
/*                                                                                    */
/*  You must modify the code to add your own robot specific commands here.            */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

void autonomous(void) 
{ 
  puncher.spinFor(25000, vex::rotationUnits::deg, 75, vex::velocityUnits::pct, false);  
  puncher2.spinFor(25000, vex::rotationUnits::deg, 75, vex::velocityUnits::pct, true);  
}

void usercontrol(void) {
  /*------------------------------------------------------------------------------------*/
  /*                                                                                    */
  /*                              USER CONTROL NOT USED                                 */
  /*                                                                                    */
  /*              USER CONTROL IS NOT USED DURING PROGRAMMING SKILLS                    */
  /*                                                                                    */
  /*------------------------------------------------------------------------------------*/

}

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Main Function                                         */
/*                                                                                    */
/*  Main will set up the competition functions and callbacks.                         */
/*------------------------------------------------------------------------------------*/

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