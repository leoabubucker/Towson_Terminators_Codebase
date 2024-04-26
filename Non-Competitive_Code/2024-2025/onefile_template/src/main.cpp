/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       leoku                                                     */
/*    Created:      4/17/2024, 5:09:59 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <string>
using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
controller Controller1 = controller(primary);
motor puncher = motor(PORT12, ratio36_1, false);
motor puncher2 = motor(PORT17, ratio36_1, true);
motor leftWing = motor(PORT13, false);
motor rightWing = motor(PORT18, true);
motor intake = motor(PORT19, ratio18_1, true);
motor leftBack = motor(PORT5, ratio18_1, true);
motor leftFront = motor(PORT11, ratio18_1, true);
motor rightBack = motor(PORT10, ratio18_1, false);
motor rightFront = motor(PORT20, ratio18_1, false);
motor_group allMotors = motor_group(puncher, puncher2, leftWing, rightWing, intake, leftBack, leftFront, rightBack, rightFront);
motor_group driveMotors = motor_group(leftBack, leftFront, rightBack, rightFront);
motor_group nonDriveMotors = motor_group(puncher, leftWing, rightWing, intake);
motor_group wingMotors = motor_group(leftWing, rightWing);

// Custom Functions

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Turn Function                                         */
/*                                                                                    */
/*  Turns degrees Degrees in direction Direction at velocity Velocity based on a      */
/*  hard-coded const of how many degrees a motor has to turn to turn the robot 90     */
/*  degrees. Hard-coded const found through testing. Returns void.                    */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

void turn(int degrees, std::string direction, int velocity){
    //Constants determined through testing
    const int motorDegreesFor90DegreeTurn = 362;
    const int motorDegreesPerDegreeTurn = motorDegreesFor90DegreeTurn/90;

    //Calculates the degrees the motors have to turn to turn the robot degrees Degrees
    int motorDegrees = motorDegreesPerDegreeTurn * degrees;

    //Inverts spin direction to turn left
    if(direction == "left"){
        motorDegrees *= -1;
    }
    leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Drive Function                                        */
/*                                                                                    */
/*  Drives inches Inches in direction Direction at velocity Velocity based on a       */
/*  hard-coded const of how many degrees a motor has to turn to drive the robot 24in  */
/*  (the length/width of one square tile). Hard-coded const found through testing.    */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

void drive(int inches, std::string direction, int velocity){
    //Constants determined through testing
    const int motorDegreesFor24Inches = 951;
    const int motorDegreesPerInch = motorDegreesFor24Inches/24;

    //Calculate the degrees the motors have to turn to drive the robot inches Inches
    int motorDegrees = motorDegreesPerInch * inches;

    //Inverts spin direction to move backwards
    if(direction == "rev"){
        motorDegrees *= -1;
    }
    leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
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
  //Resets Motor Encoder Values
  allMotors.resetPosition();

  //Sets Motor Velocities
  allMotors.setVelocity(100, percent);

  //Sets Motor Torque to 100%
  allMotors.setMaxTorque(100, percent);

  //Set Default Brake Type For Non-Drive Motors to Hold
  nonDriveMotors.setStopping(vex::brakeType::hold);
  wingMotors.setTimeout(2, vex::timeUnits::sec);
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
    drive(36, "rev", 100); 
    drive(10, "fwd", 100);
    drive(26, "rev", 100);
    drive(30, "fwd", 100);
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
  bool puncherState = true; ///TTTTTTEEEESSSTTT
  bool puncherLastState = false;

  // User control code here, inside the loop
  while (1) {  
    /*------------------------------------------------------------------------------------*/
    /*                                                                                    */
    /*                              Wheel Controls                                        */
    /*                                                                                    */
    /*  Drive with the left motors being controlled by the left joystick and the right    */
    /*  motors being controlled by the right joystick. Different drive modes determined   */
    /*  by driveConfig str as switched if motors disconnect.                              */  
    /*                                                                                    */
    /*------------------------------------------------------------------------------------*/

      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 

    /*------------------------------------------------------------------------------------*/
    /*                                                                                    */
    /*                              Intake Controls                                       */
    /*                                                                                    */
    /*  One green motor that turns fwd/rev at 100% velocity to intake/outtake triballs    */
    /*  Controls: Intake - X, Outtake - B                                                 */  
    /*                                                                                    */
    /*------------------------------------------------------------------------------------*/
 if(Controller1.ButtonL2.pressing()){
      intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    }
    else if(Controller1.ButtonR2.pressing()){
      intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
    }
    else{
      intake.stop(vex::brakeType::coast);
    }
 
    /*------------------------------------------------------------------------------------*/
    /*                                                                                    */
    /*                              Puncher Controls                                      */
    /*                                                                                    */
    /*  One red motor gear-ratioed for torque that turns fwd/rev at 100% velocity         */
    /*  to launch triballs through a puncher on a slip gear system.                       */
    /*  Controls: Toggle - Y                                                              */  
    /*                                                                                    */
    /*------------------------------------------------------------------------------------*/ 


    if(Controller1.ButtonY.pressing() && !puncherLastState) {
      puncherState = !puncherState;
      puncherLastState = true;
   } else if(!Controller1.ButtonY.pressing()) {
      puncherLastState = false;
   }

   if(puncherState) {
          puncher.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
          puncher2.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
   } else {
      puncher.stop();
      puncher2.stop();
   }
    /*------------------------------------------------------------------------------------*/
    /*                                                                                    */
    /*                              Wing Controls                                         */
    /*                                                                                    */
    /*  Two 5.5w motor that control individual mechanisms that turns fwd/rev at 100%      */
    /*  velocity to open/close wings that move triballs                                   */
    /*  Manual Controls (Limited Mechanically): Open - L1/R1, Close - L2/R2               */
    /*                                                                                    */
    /*------------------------------------------------------------------------------------*/ 

    if(Controller1.ButtonL1.pressing()){
      leftWing.spinFor(vex::directionType::fwd, 100, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
      rightWing.spinFor(vex::directionType::fwd, 100, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, true);
    }
    else if(Controller1.ButtonR1.pressing()){
      leftWing.spinFor(vex::directionType::rev, 100, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, false);
      rightWing.spinFor(vex::directionType::rev, 100, vex::rotationUnits::deg, 50, vex::velocityUnits::pct, true);
    }
    else{
      leftWing.stop(vex::brakeType:: coast);
      rightWing.stop(vex::brakeType::coast);
    }
    // }
    wait(20, msec); // Cooldown to prevent excess CPU usage and subsequent brain crashing
  } //End While Loop

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

