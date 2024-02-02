#ifndef ROBOT_CONFIG_H //Header File Guard
#define ROBOT_CONFIG_H //Header File Guard

using namespace vex; //VEX References

//START - VEX Component Declarations
extern brain Brain;
extern competition Competition;
extern controller Controller1;
extern motor leftFront;
extern motor leftBack;
extern motor rightFront;
extern motor rightBack;
extern motor intake;
extern motor puncher;
extern motor leftWing;
extern motor rightWing;
extern motor_group allMotors;
extern motor_group driveMotors;
extern motor_group nonDriveMotors;
extern motor_group wingMotors;
//END - VEX Component Declarations

#endif //Header File Guard
