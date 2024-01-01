  /*------------------------------------------------------------------------------------*/
  /*                                                                                    */
  /*                              Global References                                     */
  /*                                                                                    */
  /*  Necessary #include and using calls                                                */
  /*                                                                                    */
  /*------------------------------------------------------------------------------------*/

#include "vex.h" //VEX functions and subsequent call to robot-config.h
#include "auton.h" //Autonomous Functions
#include <string> //std::string
using namespace vex; //VEX references

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Turn Function                                         */
/*                                                                                    */
/*  Turns degrees Degrees in direction Direction at velocity Velocity based on a      */
/*  hard-coded const of how many degrees a motor has to turn to turn the robot 90     */
/*  degrees. Hard-coded const found through testing. Returns void.                    */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

void turn(int degrees, std::string direction, int velocity){
    //Constants determined through testing
    const int motorDegreesFor90DegreeTurn = 362;
    const int motorDegreesPerDegreeTurn = motorDegreesFor90DegreeTurn/90;

    //Calculates the degrees the motors have to turn to turn the robot degrees Degrees
    int motorDegrees = motorDegreesPerDegreeTurn * degrees;

    //Inverts spin direction to turn left
    if(direction == "left"){
        motorDegrees *= -1;
    }
    leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              Drive Function                                        */
/*                                                                                    */
/*  Drives inches Inches in direction Direction at velocity Velocity based on a       */
/*  hard-coded const of how many degrees a motor has to turn to drive the robot 24in  */
/*  (the length/width of one square tile). Hard-coded const found through testing.    */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

void drive(int inches, std::string direction, int velocity){
    //Constants determined through testing
    const int motorDegreesFor24Inches = 951;
    const int motorDegreesPerInch = motorDegreesFor24Inches/24;

    //Calculate the degrees the motors have to turn to drive the robot inches Inches
    int motorDegrees = motorDegreesPerInch * inches;

    //Inverts spin direction to move backwards
    if(direction == "rev"){
        motorDegrees *= -1;
    }
    leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}