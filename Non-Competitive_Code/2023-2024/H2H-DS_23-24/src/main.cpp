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
#include "gui.h" //GUI functions
using namespace vex; //VEX References

MotorCollection myMotorCollection; //Init instance of MotorCollection class to use for recurring motor testing

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
  
  //Adds all motors to myMotorCollection instance of MotorCollection Class with respective shortNames
  myMotorCollection.addMotor(leftFront, "LF");
  myMotorCollection.addMotor(leftBack, "LB");
  myMotorCollection.addMotor(rightFront, "RF");
  myMotorCollection.addMotor(rightBack, "RB");
  myMotorCollection.addMotor(intake, "I");
  myMotorCollection.addMotor(puncher, "P");
  myMotorCollection.addMotor(leftWing, "LW");
  myMotorCollection.addMotor(rightWing, "RW");
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
  //Runs Autonomous for Red 2 & Blue 1 starting positions
    drawGUI(myMotorCollection, "r2b1AUTON"); // Calls/Updates GUI and provides match state "r2b1AUTON"
    
    /*------------------------------------------------------------------------------------*/
    /*                                                                                    */
    /*                         Starting A Match - Our Offensive Zone                      */
    /*                                                                                    */
    /*  1. Place robot in the middle of Blue Starting Position 1 (Between the Red Match   */
    /*  Load Zone and Blue Elevation Bar                                                  */
    /*                                                                                    */
    /*  2. Place the robot at the front of the tile with the intake-side facing forward.  */
    /*                                                                                    */
    /*  3. Manually spin the intake to intake the preload triball.                        */
    /*                                                                                    */
    /*  4. Ensure all motors are connected and the Controller is plugged in.              */
    /*                                                                                    */
    /*  5. Start the "Run" of the program via the Controller; it is not necessary to      */
    /*  click "Timed Run"                                                                 */
    /*                                                                                    */
    /*  6. The autonomous will hopefully score the preload into our goal before going to  */
    /*  our elevation bar and touching it. (2/3 auton bonus requirements achieved)        */
    /*                                                                                    */
    /*------------------------------------------------------------------------------------*/

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
  

/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*                              User Control Task                                       */
/*                                                                                      */
/*  This task is used to control your robot during the user control phase of            */
/*  a VEX Competition.                                                                  */
/*                                                                                      */
/*  You must modify the code to add your own robot specific commands here.              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

void usercontrol(void) {
  /*------------------------------------------------------------------------------------*/
  /*                                                                                    */
  /*                              Match Initializations                                 */
  /*                                                                                    */
  /*  Quick tasks that must be completed after autonomous and only once.                */
  /*                                                                                    */
  /*------------------------------------------------------------------------------------*/
  double seconds = 0.0; //Init seconds int to track seconds of user control execution (converted from msec)
  drawGUI(myMotorCollection, "USER CONTROL"); // Calls/Updates GUI and provides match state "USER CONTROL"
  std::string driveConfig = myMotorCollection.checkDriveMotors(); //Checks drive motor statuses and switches drive mode (4-wheel, front-wheel, rear-wheel, LFRB, RFLB)
  bool intakeFwdState = false;
  bool intakeFwdLastState = false;
  bool intakeRevState = false;
  bool intakeRevLastState = false;
  bool puncherState = false;
  bool puncherLastState = false;

  // User control code here, inside the loop
  while (1) {
    //Runs every 2 seconds
    if(int(seconds) % 2 == 0 && seconds > 1.0){
      drawGUI(myMotorCollection, "USER CONTROL"); // Calls/Updates GUI and provides match state "USER CONTROL"
      driveConfig = myMotorCollection.checkDriveMotors(); //Checks drive motor statuses and determines best drive mode (4-wheel, front-wheel, rear-wheel, LFRB, RFLB)
      
    }    
    /*------------------------------------------------------------------------------------*/
    /*                                                                                    */
    /*                              Wheel Controls                                        */
    /*                                                                                    */
    /*  Drive with the left motors being controlled by the left joystick and the right    */
    /*  motors being controlled by the right joystick. Different drive modes determined   */
    /*  by driveConfig str as switched if motors disconnect.                              */  
    /*                                                                                    */
    /*------------------------------------------------------------------------------------*/

    if(driveConfig == "rearWheel"){
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
    }
    else if(driveConfig == "frontWheel"){
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
    }
    else if(driveConfig == "RFLB"){
      rightFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);     
    }
    else if(driveConfig == "LFRB"){
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);     
    }
    else{
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
    }


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
    wait(20, msec); // Cooldown to prevent excess CPU usage and subsequent brain crashing
    seconds += 0.02; // Converts the msec wait time to seconds to best approximate the total seconds passed in the while loop
  } //End While Loop

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

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              DEPRECATIONS                                          */
/*                                                                                    */
/*  Deprecations after Hereford Competition, 12/2/2023                                */
/*                                                                                    */
/*     Four-Bar Lift Controls                                                         */ 
/*     Catapult Controls                                                              */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Four-Bar Lift Controls                                */
/*                                                                                    */
/*   Two-motor four-bar lift controls with manual movement, and automated             */
/*   movement to the highest and lowest elevations.                                   */  
/*                                                                                    */
/*                                                                                    */
/*                                                                                    */
/*   if(Controller1.ButtonX.pressing()){                                              */
/*     //Manual Lift                                                                  */
/*     leftFourBarLift.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);    */
/*     rightFourBarLift.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);   */
/*   }                                                                                */
/*   else if (Controller1.ButtonY.pressing()){                                        */
/*     //Manual Lower                                                                 */
/*     leftFourBarLift.spin(vex::directionType::rev, 50, vex::velocityUnits::pct);    */
/*     rightFourBarLift.spin(vex::directionType::rev, 50, vex::velocityUnits::pct);   */
/*   }                                                                                */
/*   else if(!Controller1.ButtonL1.pressing() && !Controller1.ButtonL2.pressing()){   */
/*     //Braking                                                                      */
/*     leftFourBarLift.stop(vex::brakeType::hold);                                    */
/*     rightFourBarLift.stop(vex::brakeType::hold);                                   */
/*   }                                                                                */
/*                                                                                    */
/*   if (Controller1.ButtonL1.pressing()){                                            */
/*     //Automatic Lift                                                               */ 
/*     leftFourBarLift.spinTo(680, vex::rotationUnits::deg, false);                   */
/*     rightFourBarLift.spinTo(680, vex::rotationUnits::deg, false);                  */
/*   }                                                                                */
/*   if( Controller1.ButtonL2.pressing()){                                            */
/*     //Automatic Lower                                                              */
/*     leftFourBarLift.spinTo(0, vex::rotationUnits::deg, false);                     */
/*     rightFourBarLift.spinTo(0, vex::rotationUnits::deg, false);                    */
/*   }                                                                                */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Catapult Controls                                     */
/*                                                                                    */
/*  Single-motor catapult controls where continuous spinning is toggled               */
/*  on by Button R1 and off by button R2.                                             */  
/*                                                                                    */
/*                                                                                    */
/*                                                                                    */
/*   bool catapultIsLaunching;                                                        */
/*                                                                                    */
/*   if (Controller1.ButtonR2.pressing()){                                            */
/*     catapultIsLaunching = false;                                                   */
/*   }                                                                                */
/*                                                                                    */
/*   if (catapultIsLaunching) {                                                       */
/*     catapult.spin(vex::directionType::fwd);                                        */
/*   }                                                                                */
/*   else{                                                                            */
/*     catapult.stop(vex::brakeType::hold);                                           */
/*   }                                                                                */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/