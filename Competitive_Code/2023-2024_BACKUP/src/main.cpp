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
  drive(48, "fwd", 100);
  turn(90, "right", 50);
  drive(24, "fwd", 100);
  intake.spinFor(vex::directionType::rev, 400, vex::rotationUnits::deg, true);
  drive(24, "rev", 100);
  turn(90, "right", 50);
  drive(48, "fwd", 100);
  turn(90, "right", 50);
  drive(24, "fwd", 100);
}



void usercontrol(void) {
  bool intakeFwdState = false;
  bool intakeFwdLastState = false;
  bool intakeRevState = false;
  bool intakeRevLastState = false;
  bool puncherState = false;
  bool puncherLastState = false;

  /*------------------------------------------------------------------------------------*/
  /*                                                                                    */
  /*                              Wheel Controls                                        */
  /*                                                                                    */
  /*  Drive with the left motors being controlled by the left joystick and the right    */
  /*  motors being controlled by the right joystick.                                    */  
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

    if(Controller1.ButtonX.pressing() && !intakeFwdLastState) {
      intakeFwdState = !intakeFwdState;
      intakeFwdLastState = true;
   } else if(!Controller1.ButtonX.pressing()) {
      intakeFwdLastState = false;
   }
   if(intakeFwdState) {
          if(intakeRevState){
            intakeRevState = false;
            intakeRevLastState = false;
          }
          intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
   } 

  if(Controller1.ButtonB.pressing() && !intakeRevLastState) {
      intakeRevState = !intakeRevState;
      intakeRevLastState = true;
   } else if(!Controller1.ButtonB.pressing()) {
      intakeRevLastState = false;
   }
   if(intakeRevState) {
          if(intakeFwdState){
            intakeFwdState = false;
            intakeFwdLastState = false;
          }
          intake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
   } 

   if(!intakeFwdState && !intakeRevState){
    intake.stop();
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
   } else {
      puncher.stop();
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
      leftWing.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    }
    else if(Controller1.ButtonL2.pressing()){
      leftWing.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
    }
    else{
      leftWing.stop(vex::brakeType::hold);
    }

    if(Controller1.ButtonR1.pressing()){
      rightWing.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    }
    else if(Controller1.ButtonR2.pressing()){
      rightWing.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
    }
    else{
      rightWing.stop(vex::brakeType::hold);
    }
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