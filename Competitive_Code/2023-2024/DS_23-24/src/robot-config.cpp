#include "vex.h" //VEX functions and subsequent call to robot-config.h
using namespace vex; //VEX References

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 Definitions                               */
/*                                                                           */
/*  Definitions of VEX components for use in other files. Details Below:     */
/*                                                                           */
/*      Controller1 is defined w/ the "primary" parameter as it is the only  */
/*      Controller being used.                                               */ 
/*      Motors are declared with the PORT parameter (PORT1 - PORT21),        */
/*      RATIO parameter(ratio18_1 for green motors and ratio36_1 for         */
/*      red motors, excluded for 5.5w motors), and isReversed Boolean        */
/*      parameter (true reverses motor's default spin direction              */
/*      motor catapult = motor(PORT6, ratio36_1, true);                      */
/*---------------------------------------------------------------------------*/

brain Brain;
competition Competition;
controller Controller1 = controller(primary);
motor puncher = motor(PORT12, ratio36_1, false);
motor leftWing = motor(PORT13, false);
motor rightWing = motor(PORT18, true);
motor intake = motor(PORT19, ratio18_1, true);
motor leftBack = motor(PORT5, ratio18_1, true);
motor leftFront = motor(PORT11, ratio18_1, true);
motor rightBack = motor(PORT10, ratio18_1, false);
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