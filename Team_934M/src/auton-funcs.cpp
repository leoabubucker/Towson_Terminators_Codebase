#include "auton-funcs.h"
#include "vex.h"
using namespace vex;

// void driveFwd(int deg){
//     leftFront.spinFor(vex::directionType::fwd, deg, vex::rotationUnits::deg, false); 
//     leftBack.spinFor(vex::directionType::fwd, deg, vex::rotationUnits::deg, false);
//     rightFront.spinFor(vex::directionType::fwd, deg, vex::rotationUnits::deg, false);
//     rightBack.spinFor(vex::directionType::fwd, deg, vex::rotationUnits::deg);
// }
// void driveRev(int deg){
//     leftFront.spinFor(vex::directionType::rev, deg, vex::rotationUnits::deg, false); 
//     leftBack.spinFor(vex::directionType::rev, deg, vex::rotationUnits::deg, false);
//     rightFront.spinFor(vex::directionType::rev, deg, vex::rotationUnits::deg, false);
//     rightBack.spinFor(vex::directionType::rev, deg, vex::rotationUnits::deg);
// }

const float WHEEL_DIAMETER = 4.125; // inches
const float WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * 3.1416;
const float GEAR_RATIO = 1; // 1 turn of motor = 1 turn of wheel
const int  AUTON_DRIVE_PCT = 50; // motors at 100% power during auton
// const float TURNING_DIAMETER = 25.0; // distance (in inches) from top-left wheel to bottom-right wheel
// void turn( float degrees ) {
//     // Note: +90 degrees is a right turn
//     float turningRatio = TURNING_DIAMETER / WHEEL_DIAMETER;
//     float wheelDegrees = turningRatio * degrees;    
//     // Divide by four because each wheel provides 1/4 the rotation
//     leftFront.spinFor(
//         wheelDegrees * GEAR_RATIO / 4, vex::rotationUnits::deg, 
//         AUTON_DRIVE_PCT, vex::velocityUnits::pct, false
//     );
//     leftBack.spinFor(
//         wheelDegrees * GEAR_RATIO / 4, vex::rotationUnits::deg, 
//         AUTON_DRIVE_PCT, vex::velocityUnits::pct, false
//     );
//     rightFront.spinFor(
//         wheelDegrees * GEAR_RATIO / 4, vex::rotationUnits::deg,
//         AUTON_DRIVE_PCT, vex::velocityUnits::pct, false
//     );
//     rightBack.spinFor(
//         wheelDegrees * GEAR_RATIO / 4, vex::rotationUnits::deg,
//         AUTON_DRIVE_PCT, vex::velocityUnits::pct
//     );
// }
void drive( float inches ) {
    float inchesPerDegree = WHEEL_CIRCUMFERENCE / 360;
    float degrees = inches / inchesPerDegree;

    leftFront.spinFor(
        degrees * GEAR_RATIO, vex::rotationUnits::deg, 
        AUTON_DRIVE_PCT, vex::velocityUnits::pct, false
    );
    leftBack.spinFor(
        degrees * GEAR_RATIO, vex::rotationUnits::deg, 
        AUTON_DRIVE_PCT, vex::velocityUnits::pct, false
    );
    rightFront.spinFor(
        degrees * GEAR_RATIO, vex::rotationUnits::deg,
        AUTON_DRIVE_PCT, vex::velocityUnits::pct, false
    );
    rightBack.spinFor(
        degrees * GEAR_RATIO, vex::rotationUnits::deg,
        AUTON_DRIVE_PCT, vex::velocityUnits::pct, true
    );
}   


// const float WheelCircumference = (4.0 * 3.1415);
const float  RevToAngle  = (1/93);// how many degrees does the whole robot turn if we turn the left motors one full revolution backwards while turning the right motors one full revolution forwards 
// When left goes back and right goes forward, the bot turns left 
void turnLeft(float degrees){
    float MotorDegrees = degrees * RevToAngle * 360;
    leftFront.spinFor(vex::directionType::rev, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::rev, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::fwd, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::fwd, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, true);
}

void turnRight(float degrees){
    float MotorDegrees = degrees * RevToAngle * 360;
    leftFront.spinFor(vex::directionType::fwd, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::fwd, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::rev, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::rev, MotorDegrees, vex::rotationUnits::deg, 45, vex::velocityUnits::pct, true);
}


//PID


//Settings
double kP = 0.0;
double kI = 0.0;
double kD = 0.0;

double turnkP = 0.0;
double turnkI = 0.0;
double turnkD = 0.0;
double degrees_to_ticks_conversion = 1/5; //gear ratio

int error; //Sensor value - desired value (Positional Value)
int prevError = 0; // Position 20 miliseconds ago
int derivative; //error - prev error (Calcs speed)
int totalError = 0; //total error = total error + error

int turnError; //Sensor value - desired value (Positional Value)
int turnPrevError = 0; // Position 20 miliseconds ago
int turnDerivative; //error - prev error (Calcs speed)
int turnTotalError = 0; //total error = total error + error

//Autonomous Settings
int desiredValue = 200;
int desiredTurnValue = 0;
//Variables modified for use
bool enableDrivePID = true; 
bool resetDriveSensors = false;
int drivePID(){
    while(enableDrivePID){
        if(resetDriveSensors){
            resetDriveSensors = false;
            leftBack.setPosition(0, degrees);
            rightBack.setPosition(0, degrees);
        }
        int leftMotorPos = leftBack.position(degrees); //position of left motor
        int rightMotorPos = rightBack.position(degrees); //position of right motor

        /* Lateral Movement PID */

        int avgPos = (leftMotorPos + rightMotorPos) / 2; //Get Avg of two motors

        error = avgPos - desiredValue; //Potential

        derivative = error - prevError; //Derivative

        //totalError += error; //Integral

        double lateralMotorPower = (error * kP + derivative * kD);

        /* Turning Movement PID */

        int turnDifference = leftMotorPos - rightMotorPos;

        turnError = turnDifference - desiredValue; //Potential

        turnDerivative = turnError - turnPrevError; //Derivative

        //turnTotalError += turnError; //Integral

        double turnMotorPower = (turnError * turnkP + turnDerivative * turnkD);

        leftBack.spin(vex::directionType::fwd, lateralMotorPower + turnMotorPower, vex::voltageUnits::volt);
        rightBack.spin(vex::directionType::fwd, lateralMotorPower - turnMotorPower, vex::voltageUnits::volt);
        prevError = error;
        turnPrevError = turnError;
        vex::task::sleep(20);
    }
    return 1;
}
void move_to_angle(double degrees){
    desiredValue = degrees * degrees_to_ticks_conversion;
}