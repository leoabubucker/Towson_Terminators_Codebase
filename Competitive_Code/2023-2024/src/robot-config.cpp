#include "vex.h" //VEX functions and subsequent call to robot-config.h
using namespace vex; //VEX References
#include <string> //std::string
#include <sstream> //std::ostringstream
#include <vector> //std::vector
#include <algorithm> //std::find

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           Custom to_string Function                       */
/*                                                                           */
/*  Custom Function created by VEX official jpearman to convert values to    */
/*  std::string format as the std::to_string function is not reliable in     */
/*    C++11 which VEX API uses.                                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           Motor Collection Class                          */
/*                                                                           */
/*  Implementation of MotorCollection class declared in robot-config.h w/    */
/*  the following functions (function names abbreviated w/o parameters):     */
/*      void MotorCollection::addMotor() - adds the passed in motor object   */
/*          and std::string object motorName to the motorList and            */
/*          motorNamesList respectively.                                     */
/*      std::vector<bool> MotorCollection::isConnected() - Returns a list    */
/*          of boolean variables representing whether the motor is           */
/*          connected or disconnected. If motor is disconnected, prints      */
/*          the motor name on the Controller screen and rumbles it.          */ 
/*      std::string MotorCollection::checkDriveMotors() - Returns a          */
/*          std::string var representing the drive configuration as          */
/*          determined by which drive motors are connected/disconnected.     */
/*      std::vector<std::string> MotorCollections:returnPositions(),         */
/*      returnVelocities(), returnTorque(), returnTemperatures() -           */
/*          Returns a list of motor pos/vel/torq/temp in the format          */
/*          "name pos/vel/torq/temp"                                         */            
/*                                                                           */
/*---------------------------------------------------------------------------*/

    std::vector<vex::motor> motorList; //Init vector of motor objects
    std::vector<std::string> motorNamesList; //Init vector of std::strings representing motor names
    
    void MotorCollection::addMotor(motor newMotor, std::string newMotorName){
        motorList.push_back(newMotor);
        motorNamesList.push_back(newMotorName);
    }
    std::vector<bool> MotorCollection::isConnected(){
        std::vector<bool> motorConnections;
        for(int i=0; i<motorList.size(); i++){
            motorConnections.push_back(motorList[i].installed());
            if(!motorList[i].installed()){
                Controller1.Screen.print(motorNamesList[i].c_str());
                Controller1.Screen.newLine();
                Controller1.rumble("---   ");
            }
        } 
        return motorConnections;
    }

    std::string MotorCollection::checkDriveMotors(){
        std::vector<bool>motorConnections = isConnected();
        std::vector<std::string> disconnectedMotorNames;
        std::string updatedDriveConfig = "fourWheel";
        for(int i=0; i<motorConnections.size(); i++){
           if(motorConnections[i] == false){
                disconnectedMotorNames.push_back(motorNamesList[i]);
           }
        }
        if(std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LF") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "RF") != disconnectedMotorNames.end()){
            updatedDriveConfig = "rearWheel";
        }

        if(std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LB") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "RB") != disconnectedMotorNames.end()){
            updatedDriveConfig = "frontWheel";
        }

        if(std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LF") != disconnectedMotorNames.end() && std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "RB") != disconnectedMotorNames.end()){
            updatedDriveConfig = "RFLB";
        }
        if(std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LB") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "RF") != disconnectedMotorNames.end()){
            updatedDriveConfig = "LFRB";
        }
        return updatedDriveConfig;
    }
    std::vector<std::string> MotorCollection::returnPositions(){
        std::vector<std::string> motorPositions;
        for(int i=0; i<motorList.size(); i++){
            motorPositions.push_back(motorNamesList[i] + " " + to_string(motorList[i].position(vex::rotationUnits::deg)));
        }
        return motorPositions;
    }

    std::vector<std::string> MotorCollection::returnVelocities(){
        std::vector<std::string> motorVelocities;
        for(int i=0; i<motorList.size(); i++){
            motorVelocities.push_back(motorNamesList[i] + " "  + to_string(motorList[i].velocity(vex::velocityUnits::pct)));
        }
        return motorVelocities;
    }
    std::vector<std::string> MotorCollection::returnTorque(){
        std::vector<std::string> motorTorque;
        for(int i=0; i<motorList.size(); i++){
            motorTorque.push_back(motorNamesList[i] + " "  + to_string(motorList[i].torque(vex::torqueUnits::Nm)));
        }
        return motorTorque;
    }
    std::vector<std::string> MotorCollection::returnTemperatures(){
        std::vector<std::string> motorTemperatures;
        for(int i=0; i<motorList.size(); i++){
            motorTemperatures.push_back(motorNamesList[i] + " "  + to_string(motorList[i].temperature(vex::percentUnits::pct)));
        }
        return motorTemperatures;
    }

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