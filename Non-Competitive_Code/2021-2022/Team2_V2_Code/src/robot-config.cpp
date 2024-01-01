#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftBackWheel = motor(PORT16, ratio18_1, false);
motor rightBackWheel = motor(PORT19, ratio18_1, false);
motor leftFrontArm = motor(PORT1, ratio18_1, false);
motor rightFrontArm = motor(PORT3, ratio18_1, false);
motor29 leftBackClamp = motor29(Brain.ThreeWirePort.B, false);
motor29 rightBackClamp = motor29(Brain.ThreeWirePort.A, false);
controller Controller1 = controller(primary);

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