/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       leoku                                                     */
/*    Created:      6/25/2024, 3:14:06 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <map>
#include <stdlib.h>
using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
controller Controller1 = controller(primary);
triport myTriport = triport(Brain.ThreeWirePort);
bumper checkpointBumper = bumper(myTriport.A);
bumper finalBumper = bumper(myTriport.C);
int stage = 0;
bool checkpointReached = false;
bool raceFinished = false;
bool checkpointTouched = false;
double checkpointTime = 0.0;
void drawGUI(){
  std::map<std::string, int> bgFrame = {
          {"x-left", 0},
          {"x-right", 479},
          {"y-top", 0},
          {"y-bottom", 240}
      };
  color bgColor = color::red;
  if(raceFinished){
    bgColor = color::green;
    stage = 2;
  }
  else if(checkpointReached){
    bgColor = color::orange;
    stage = 1;

  }
  Brain.Screen.drawRectangle(bgFrame["x-left"], bgFrame["y-top"], bgFrame["x-right"] - bgFrame["x-left"], bgFrame["y-bottom"] - bgFrame["y-top"], bgColor);
  Brain.Screen.setFillColor(bgColor);
  Brain.Screen.setFont(fontType::mono60);
  Brain.Screen.setCursor(2, 7);
  Brain.Screen.print(Brain.timer(vex::timeUnits::sec));  
  Brain.Screen.setFont(fontType::mono40);
  if(raceFinished){
    Brain.Screen.setCursor(4, 5);
  }
  else if(checkpointReached){
    Brain.Screen.setCursor(4, 4);
  }
  else{
  Brain.Screen.setCursor(4, 2);
  }
  if(stage == 0){
    Brain.Screen.print("Checkpoint Not Reached");
  }
  else if(stage == 1){
    Brain.Screen.print("Checkpoint Reached");
  }
  else if(stage == 2){
    Brain.Screen.print("Race Finished");
  }
  if(checkpointTouched){
    checkpointTime = Brain.timer(vex::timeUnits::sec);
    checkpointTouched = false;
  }

  if(checkpointReached){
    Brain.Screen.setFont(fontType::mono30);
    Brain.Screen.setCursor(7, 5);
    Brain.Screen.print("Checkpoint Time: ");
    Brain.Screen.print(checkpointTime);
  }

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
  while (!raceFinished) {
    if(checkpointBumper.pressing() == 1 && !checkpointReached){
      checkpointReached = true;
      checkpointTouched = true;
    }
    else if(finalBumper.pressing() == 1 && !raceFinished && checkpointReached){
      raceFinished = true;
    }
    drawGUI();

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(100, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }

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
  while (!raceFinished) {
    if(checkpointBumper.pressing() == 1){
      checkpointReached = true;
    }
    else if(finalBumper.pressing() == 1){
      raceFinished = true;
    }
    drawGUI();
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(100, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the comptition functions and callbacks.
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
