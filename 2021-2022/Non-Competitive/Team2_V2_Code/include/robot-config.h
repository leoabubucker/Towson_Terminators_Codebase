using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor leftBackWheel;
extern motor rightBackWheel;
extern motor leftFrontArm;
extern motor rightFrontArm;
extern motor29 leftBackClamp;
extern motor29 rightBackClamp;
extern controller Controller1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );