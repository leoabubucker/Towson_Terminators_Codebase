/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Daksh Sangar                                              */
/*    Created:      9/22/2024, 7:12:21 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "vex.h"
#include <cmath>


using namespace vex;


// A global instance of competition
competition Competition;


// define your global instances of motors and other devices here
//working ports for old brain: 6, 7, 8, 13, 14, 18, 19, 21
brain Brain;
controller Controller1 = controller(primary);
motor rightFront = motor(PORT8, ratio18_1, false);
motor rightBack = motor(PORT19, ratio18_1, false);
motor leftFront = motor(PORT14, ratio18_1, true);
motor leftBack = motor(PORT18, ratio18_1, true);
motor intake = motor(PORT1, ratio18_1, true);
motor clamp = motor(PORT6, ratio18_1, true);
motor claw = motor(PORT7, ratio18_1, true);




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
 // ..........................................................................
 // Insert autonomous user code here.
 // ..........................................................................
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


 //sensitivity for turning motor (higher value = more volts to turn and less volts to forward)
 double movementSensitivity = 0.5;


 while (1) {


   /*     
               DRIVE CONTROLS
  
     Left joystick for forward and backward movement
     Right joystick for turning
      
   */
   double rightJoystickForward = Controller1.Axis3.position();
   double leftJoystickTurn = Controller1.Axis1.position();
  
  
   double turn = leftJoystickTurn * 0.12;
   double forward = rightJoystickForward * 0.12 * (1 - (std::abs(turn)/12.0) * movementSensitivity);
  
   rightFront.spin(vex::directionType::fwd, forward - turn, vex::voltageUnits::volt);
   rightBack.spin(vex::directionType::fwd, forward - turn, vex::voltageUnits::volt);
   leftFront.spin(vex::directionType::fwd, forward + turn, vex::voltageUnits::volt);
   leftBack.spin(vex::directionType::fwd, forward + turn, vex::voltageUnits::volt);


   /*
               INTAKE CONTROLS
      
       L1 for forward
       L2 for reverse
   */
   bool L1pressing = Controller1.ButtonL1.pressing();
   bool L2pressing = Controller1.ButtonL2.pressing();


   if(L1pressing)
   {
     intake.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
   }else if (L2pressing)
   {
     intake.spin(vex::directionType::fwd, -12.0, vex::voltageUnits::volt);
   }else
   {
     intake.spin(vex::directionType::fwd, 0, vex::voltageUnits::volt);
   }




   /*
               CLAW CONTROLS
      
       R1 for forward
       R2 for reverse
   */
   bool R1pressing = Controller1.ButtonR1.pressing();
   bool R2pressing = Controller1.ButtonR2.pressing();


   if(R1pressing)
   {
     claw.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
   }else if (R2pressing)
   {
     claw.spin(vex::directionType::fwd, -12.0, vex::voltageUnits::volt);
   }else
   {
     claw.spin(vex::directionType::fwd, 0, vex::voltageUnits::volt);
   }


   /*
               CLAMP CONTROLS


       X for forward    
       B for reverse
   */
   bool Xpressing = Controller1.ButtonX.pressing();
   bool Bpressing = Controller1.ButtonB.pressing();


   if(Xpressing)
   {
     clamp.spin(vex::directionType::fwd, 12.0, vex::voltageUnits::volt);
   }else if (Bpressing)
   {
     clamp.spin(vex::directionType::fwd, -12.0, vex::voltageUnits::volt);
   }else
   {
     clamp.spin(vex::directionType::fwd, 0, vex::voltageUnits::volt);
   }


   // ........................................................................


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



