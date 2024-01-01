#include "vex.h"
using namespace vex;
// using signature = vision::signature;
// using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;
// A global instance of competition
competition Competition;
// VEXcode device constructors

//FOR DRIVE FLIP LEFT FRONT AND RIGHT FRONT
controller Controller1 = controller(primary);
motor leftFront = motor(PORT1, ratio18_1, true);
motor leftBack = motor(PORT3, ratio18_1, true);
motor rightFront = motor(PORT2, ratio18_1, false);
motor rightBack = motor(PORT5, ratio18_1, false);
motor leftSixBarLift = motor(PORT9, ratio36_1, false);
motor rightSixBarLift = motor(PORT10, ratio36_1, true);
motor leftCatapult = motor(PORT11, ratio36_1, false);
motor rightCatapult = motor(PORT13, ratio36_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
// bool RemoteControlCodeEnabled = true;

// /**
//  * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
//  * 
//  * This should be called at the start of your int main function.
//  */
// void vexcodeInit( void ) {
//   // nothing to initialize
// }