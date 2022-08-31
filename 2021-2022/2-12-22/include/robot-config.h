using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor leftFront;
extern motor leftBack;
extern motor rightFront;
extern motor rightBack;
extern motor frontLeftClaw;
extern motor frontRightClaw;
extern motor backLeftClaw;
extern motor backRightClaw;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );