#include "vex.h"
using namespace vex;
// using signature = vision::signature;
// using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;
// A global instance of competition
competition Competition;
// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftFront = motor(PORT14, ratio36_1, false);
motor leftBack = motor(PORT6, ratio36_1, false);
motor rightFront = motor(PORT17, ratio36_1, true);
motor rightBack = motor(PORT20, ratio36_1, true);
rotation sensor1 = rotation(PORT1, false);
rotation sensor2 = rotation(PORT1, false);
motor intakeMotor = motor(PORT8, ratio18_1, false);
motor rFlywheel = motor(PORT15, ratio18_1, true);
motor lFlywheel = motor(PORT12, ratio18_1, false);
motor rollerMotor = motor(PORT11, ratio18_1, false);
motor rollerMotor2 = motor(PORT21, ratio18_1, true);
motor endgameMotor = motor(PORT19, ratio36_1, false);
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