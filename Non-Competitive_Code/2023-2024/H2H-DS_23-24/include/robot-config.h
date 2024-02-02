#ifndef ROBOT_CONFIG_H //Header File Guard
#define ROBOT_CONFIG_H //Header File Guard

using namespace vex; //VEX References
#include <string> //std::string
#include <vector> //std::vector

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

//Custom Made MotorCollection Class Declaration
extern class MotorCollection{
    public:
        std::vector<vex::motor> motorList;
        std::vector<std::string> motorNamesList;
        void addMotor(motor newMotor, std::string newMotorName);
        std::vector<bool> isConnected();
        std::string checkDriveMotors();
        std::vector<std::string> returnPositions();
        std::vector<std::string> returnVelocities();
        std::vector<std::string> returnTorque();
        std::vector<std::string> returnTemperatures();

};

//Declaration of MotorCollection Class Instance
extern MotorCollection myMotorConnection;

#endif //Header File Guard

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              DEPRECATIONS                                 */
/*                                                                           */
/*  Deprecations after Hereford Competition, 12/2/2023                       */
/*                                                                           */
/*     extern motor leftFourBarLift;                                         */
/*     extern motor rightFourBarLift;                                        */
/*     extern motor catapult;                                                */
/*---------------------------------------------------------------------------*/