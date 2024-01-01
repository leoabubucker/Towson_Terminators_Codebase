#include "vex.h"
#include "auton.h"
using namespace vex;
#include <string>
void turn(int degrees, std::string direction, int velocity){
    const int motorDegreesFor90DegreeTurn = 362;
    const int motorDegreesPerDegreeTurn = motorDegreesFor90DegreeTurn/90;

    int motorDegrees = motorDegreesPerDegreeTurn * degrees;

    if(direction == "left"){
        motorDegrees *= -1;
    }
    leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

void drive(int inches, std::string direction, int velocity){
    const int motorDegreesFor24Inches = 951;
    const int motorDegreesPerInch = motorDegreesFor24Inches/24;

    int motorDegrees = motorDegreesPerInch * inches;

    if(direction == "rev"){
        motorDegrees *= -1;
    }
    leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
    rightBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

// /*
// Calculus Notes:

// Derivative Chain: d/dx position = velocity, d/dx velocity = acceleration

// Integral (reverse derivative): integral accelaration = velocity, integral velocity = position, integral position = Absement


// */


// //Settings
// double kP = 0.0; //Air
// double kI = 0.0; //Minor misc Corrections
// double kD = 0.0; //Speed Corrections 

// double turnkP = 0.0;
// double turnkI = 0.0;
// double turnkD = 0.0;
// //Auton Settings
// int desiredValue = 200;

// int error; // Sensor Value - Desired Value (Positional Value)
// int prevError = 0; // Position 20ms ago
// int derivative; // error - prevError = speed (d/dx position = velocity)
// int totalError = 0; // totalError += error (absement)

// int turnDesiredValue = 200;

// int turnError; // Sensor Value - Desired Value (Positional Value)
// int turnPrevError = 0; // Position 20ms ago
// int turnDerivative; // error - prevError = speed (d/dx position = velocity)
// int turnTotalError = 0; // totalError += error (absement)

// int drivePID(){
//     while(enableDrivePID){
//         //Get Position of Motors
//         // int leftFrontMotorPosition = leftFront.position(degrees);
//         int leftBackMotorPosition = leftBack.position(degrees);
//         // int rightFrontMotorPosition = rightFront.position(degrees);
//         int rightBackMotorPosition = rightBack.position(degrees);


// /*                                                                            */
// /*                            Lateral Movement PID                            */
// /*                                                                            */

//         int avgPosition = (leftBackMotorPosition + rightBackMotorPosition)/2;

//         //Potential
//         error = avgPosition - desiredValue;
        
//         //Derivative
//         derivative = error - prevError;
        
//         // Integral
//         // totalError += error;

//         double lateralMotorPower = error * kP + derivative * kD;

// /*                                                                         */
// /*                            Turn Movement PID                            */
// /*                                                                         */

//         int turnDifference = (leftBackMotorPosition + rightBackMotorPosition);

//         //Potential
//         turnError = turnDifference - turnDesiredValue;
        
//         //Derivative
//         turnDerivative = turnError - turnPrevError;
        
//         // Integral
//         // turnTotalError += turnError;

//         double turnMotorPower = turnError * turnkP + turnDerivative * turnkD;

//         leftBack.spin(forward, turnMotorPower + turnVolts, voltageUnits::volt);
//         rightBack.spin(forward, turnMotorPower - turnVolts, voltageUnits::volt);

        
//         prevError = error;
//         turnPrevError = turnError;
//         vex::task::sleep(20);
//     }
//     return 1;
// }

