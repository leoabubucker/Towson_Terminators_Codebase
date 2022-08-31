#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftFront = motor(PORT4, ratio18_1, false);
motor leftBack = motor(PORT2, ratio18_1, false);
motor rightFront = motor(PORT3, ratio18_1, false);
motor rightBack = motor(PORT19, ratio18_1, false);
motor frontLeftClaw = motor(PORT9, ratio36_1, false);
motor frontRightClaw = motor(PORT10, ratio36_1, false);
motor backLeftClaw = motor(PORT12, ratio36_1, false);
motor backRightClaw = motor(PORT5, ratio36_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}