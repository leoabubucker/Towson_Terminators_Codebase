#include "vex.h" //VEX functions and subsequent call to robot-config.h
using namespace vex; //VEX References

brain Brain;
competition Competition;
controller Controller1 = controller(primary);
motor puncher = motor(PORT11, ratio36_1, false);
motor leftWing = motor(PORT12, false);
motor rightWing = motor(PORT15, false);
motor intake = motor(PORT16, ratio18_1, true);
motor leftBack = motor(PORT17, ratio18_1, true);
motor leftFront = motor(PORT18, ratio18_1, true);
motor rightBack = motor(PORT19, ratio18_1, false);
motor rightFront = motor(PORT20, ratio18_1, false);
motor_group allMotors = motor_group(puncher, leftWing, rightWing, intake, leftBack, leftFront, rightBack, rightFront);
motor_group driveMotors = motor_group(leftBack, leftFront, rightBack, rightFront);
motor_group nonDriveMotors = motor_group(puncher, leftWing, rightWing, intake);
motor_group wingMotors = motor_group(leftWing, rightWing);

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              DEPRECATIONS                                 */
/*                                                                           */
/*  Deprecations after Hereford Competition, 12/2/2023                       */
/*                                                                           */
/*     motor leftFourBarLift = motor(PORT9, ratio36_1, false);               */ 
/*     motor rightFourBarLift = motor(PORT10, ratio36_1, true);              */
/*     motor catapult = motor(PORT6, ratio36_1, true);                       */
/*---------------------------------------------------------------------------*/