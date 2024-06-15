/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       leoku                                                     */
/*    Created:      5/5/2024, 9:29:54 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace vex;


template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
controller Controller1 = controller(primary);
// motor flexwheelIntake = motor(PORT12, ratio18_1, false);
motor chainIntake = motor(PORT17, ratio18_1, false);
motor leftArm = motor(PORT13, ratio36_1, false);
motor rightArm = motor(PORT18, ratio36_1, true);
motor leftBack = motor(PORT5, ratio18_1, true);
motor leftFront = motor(PORT11, ratio18_1, true);
motor rightBack = motor(PORT10, ratio18_1, false);
motor rightFront = motor(PORT20, ratio18_1, false);
motor_group allMotors = motor_group(chainIntake, leftArm, rightArm, leftBack, leftFront, rightBack, rightFront);
motor_group driveMotors = motor_group(leftBack, leftFront, rightBack, rightFront);
motor_group leftDriveMotors = motor_group(leftFront, leftBack);
motor_group rightDriveMotors = motor_group(rightFront, rightBack);
motor_group nonDriveMotors = motor_group(chainIntake, leftArm, rightArm);

// motor_group intakeMotors = motor_group(flexwheelIntake, chainIntake);
motor_group armMotors = motor_group(leftArm, rightArm);
drivetrain robotDrive = drivetrain(leftDriveMotors, rightDriveMotors, 220, 317.5, 317.5, vex::distanceUnits::mm, 0.75); //vex::drivetrain::drivetrain	(	motor_group leftMotors, motor_group rightMotors, double	wheelTravel = wheel circumference, double trackWidth = middle of left wheel to middle of right wheel, double 	wheelBase = middle of front wheel to middle of back wheel, distanceUnits unit = distanceUnits::mm, double	externalGearRatio = 36:48 (driven gear:driver gear))		
triport myTriport = triport(Brain.ThreeWirePort);
bumper autonSelectionBumper = bumper(myTriport.A);
bumper autonConfirmationBumper = bumper(myTriport.B);

int autonSelector;
std::map<std::string, int> autonSelectorFrame;
bool waitingForUserInput = true;
/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                              CUSTOM CODE                                           */
/*                                                                                    */
/*  Code that is not the acutal functions that are run by VEX competition control.    */
/*  Includes:                                                                         */
/*  - Auton Turn function (based on hard-coded values)                                */
/*  - Auton Drive function (based on hard-coded values)                               */
/*  - PID Control                                                                     */
/*  - Auton Drive function (based on hard-coded values)                               */
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



/*---------------------------------------------------------------------------*/
/*                        PID CONTROL                                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// Constant Settings
double kP = 1.0;
double kI = 0.0;
double kD = 0.0;

double turnkP = 1.0;
double turnkI = 0.0;
double turnkD = 0.0;

//Dynamic Settings
int desiredValue = 600;
int desiredTurnValue = 0;

int error; //Sensor Value - Desired Value : Position
int prevError = 0; //Position 20msec ago
int derivative;
int totalError = 0;//totalError += error

int turnError; //Sensor Value - Desired Value : Position
int turnPrevError = 0; //Position 20msec ago
int turnDerivative;
int turnTotalError = 0;//totalError += error

bool resetDriveSensors = false;

bool enableDrivePID = true;

int drivePID(){
  while(enableDrivePID){

    if(resetDriveSensors){
      resetDriveSensors = false;
      driveMotors.resetPosition();
    }
    //Get Motor Positions
    int lfPosition = leftFront.position(vex::rotationUnits::deg);
    int rfPosition = rightFront.position(vex::rotationUnits::deg);
    int lbPosition = leftBack.position(vex::rotationUnits::deg);
    int rbPosition = rightBack.position(vex::rotationUnits::deg);

  ////////////////////////////////////////////////////////////////
  //                  Lateral Movement PID                      //
  ////////////////////////////////////////////////////////////////


    //Get Avg Motor Positions
    int frontMotorAvgPosition = (lfPosition + rfPosition)/2;
    int backMotorAvgPosition = (lbPosition + rbPosition)/2;

    //Potential
    error = backMotorAvgPosition - desiredValue;

    //Derivative
    derivative = error - prevError;

    //Integral
    totalError += error;

    double lateralMotorDegrees = (error * kP + derivative * kD + totalError * kI)/360;

  ////////////////////////////////////////////////////////////////
  //                  Turn Movement PID                         //
  ////////////////////////////////////////////////////////////////

    //Get Avg Motor Positions
    int frontMotorTurnDifference= lfPosition - rfPosition;
    int backMotorTurnDifference = lbPosition - rbPosition;

    //Potential
    turnError = backMotorTurnDifference - desiredValue;

    //Derivative
    turnDerivative = turnError - turnPrevError;

    //Integral
    turnTotalError += turnError;

    double turnMotorDegrees = (turnError * turnkP + turnDerivative * turnkD + turnTotalError * turnkI)/360;

    leftDriveMotors.spin(vex::directionType::fwd, lateralMotorDegrees + turnMotorDegrees, vex::voltageUnits::volt);
    rightDriveMotors.spin(vex::directionType::fwd, lateralMotorDegrees - turnMotorDegrees, vex::voltageUnits::volt);


    prevError = error;
    turnPrevError = turnError;
    vex::task::sleep(20);
  }
  return 1;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           Motor Collection Class                          */
/*                                                                           */
/*  Implementation of MotorCollection class w/                               */
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

class MotorCollection{
  public:
    std::vector<vex::motor> motorList; //Init vector of motor objects
    std::vector<std::string> motorNamesList; //Init vector of std::strings representing motor names
    
    void addMotor(motor newMotor, std::string newMotorName){
        motorList.push_back(newMotor);
        motorNamesList.push_back(newMotorName);
    }
    std::vector<bool> isConnected(){
      Controller1.Screen.clearLine();
      Controller1.Screen.setCursor(0,0);
        std::vector<bool> motorConnections;
        for(int i=0; i<motorList.size(); i++){
            motorConnections.push_back(motorList[i].installed());
            if(!motorList[i].installed()){
                Controller1.Screen.print(motorNamesList[i].c_str());
                Controller1.Screen.print(" ");
            }
        } 
        return motorConnections;
    }

    std::string checkMotors(){
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
  
  std::vector<std::vector<std::string>> returnValues(){
    std::vector<std::vector<std::string>> returnedList;
    std::string individualValue;
    std::string motorCartridgeType;
    std::string motorType;

    for(int i = 0; i < motorList.size(); i++){
      if(motorList[i].getMotorCartridge() == vex::gearSetting::ratio36_1){
        motorCartridgeType = "Red";
      }
      else if(motorList[i].getMotorCartridge() == vex::gearSetting::ratio18_1){
        motorCartridgeType = "Green";
      }
      else{
        motorCartridgeType = "Blue";
      }

      if(motorList[i].getMotorType() == 0){
        motorType = "11W";
      }
      else{
        motorType = "5.5W";
      }

      individualValue = motorNamesList[i] + ": " + motorCartridgeType + ", " + motorType + ", " + to_string(motorList[i].temperature(vex::temperatureUnits::fahrenheit)) + "F, " + to_string(motorList[i].position(vex::rotationUnits::deg)) + "deg";
      returnedList.push_back({motorNamesList[i], ": ", motorCartridgeType, ", ", motorType, ", ", to_string(motorList[i].temperature(vex::temperatureUnits::fahrenheit)), "F", ", ", to_string(motorList[i].position(vex::rotationUnits::deg)), "deg" });
    }
    return returnedList;
  }

};

MotorCollection myMotorCollection;

/*---------------------------------------------------------------------------*/
/*                        CUSTOM GUI                                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void drawGUI(){
      //Coordinates of the R2B1 Auton Btn
    std::map<std::string, int> controlsFrame = {
        {"x-left", 0},
        {"x-right", 200},
        {"y-top", 0},
        {"y-bottom", 180}
    };
    color maroon = color(128, 0 , 0);
    //Creates red Btn at previously declared coords
    Brain.Screen.drawRectangle(controlsFrame["x-left"], controlsFrame["y-top"], controlsFrame["x-right"] - controlsFrame["x-left"], controlsFrame["y-bottom"] - controlsFrame["y-top"], maroon);
    //Prints R2B1 Auton in the middle of created btn
    Brain.Screen.setFillColor(maroon);
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.setCursor(1, 8);
    Brain.Screen.print("Controls:");
    Brain.Screen.setCursor(3, 2);
    Brain.Screen.print("Driving - Tank");
    Brain.Screen.setCursor(4, 2);
    Brain.Screen.print("Intake - L1");
    Brain.Screen.setCursor(5, 2);
    Brain.Screen.print("Outtake - L2");
    Brain.Screen.setCursor(6, 2);
    Brain.Screen.print("Lift Arm - R1");
    Brain.Screen.setCursor(7, 2);
    Brain.Screen.print("Lower Arm - R2");
    Brain.Screen.setCursor(8, 2);
    Brain.Screen.print("Toggle Manual - Up");
    Brain.Screen.setCursor(9, 2);
    Brain.Screen.print("Update GUIs - X");

    //Auton Selector
    autonSelectorFrame = {
        {"x-left", 0},
        {"x-right", 100},
        {"y-top", 179},
        {"y-bottom", 240}     
    };
    Brain.Screen.drawRectangle(autonSelectorFrame["x-left"], autonSelectorFrame["y-top"], autonSelectorFrame["x-right"] - autonSelectorFrame["x-left"], autonSelectorFrame["y-bottom"] - autonSelectorFrame["y-top"], maroon);

    Brain.Screen.setCursor(10, 2);
    Brain.Screen.print("Auton: ");
    Brain.Screen.print(autonSelector);
    Brain.Screen.setCursor(11, 2);
    if(waitingForUserInput){
      Brain.Screen.print("UNLOCKED");
    }
    else{
      Brain.Screen.print("LOCKED");
    }

    //Current Mode Display
    std::map <std::string, int> modeDisplayFrame = {
        {"x-left", 99},
        {"x-right", 200},
        {"y-top", 179},
        {"y-bottom", 240}     
    };  
    Brain.Screen.drawRectangle(modeDisplayFrame["x-left"], modeDisplayFrame["y-top"], modeDisplayFrame["x-right"] - modeDisplayFrame["x-left"], modeDisplayFrame["y-bottom"] - modeDisplayFrame["y-top"], maroon);
    Brain.Screen.setCursor(10, 12);
    Brain.Screen.print("Mode: ");
    std::string currentMode;
    if(Competition.isEnabled()){
      if(Competition.isAutonomous()){
        Brain.Screen.setPenColor(color::orange);
        currentMode = "AUTON";
      }
      else if(Competition.isDriverControl()){
        Brain.Screen.setPenColor(color::green);
        currentMode = "DRIVER";
      }
      else{
        Brain.Screen.setPenColor(color::yellow);
        currentMode = "PRE-AUTON";
      }
    }
    else{
      Brain.Screen.setPenColor(color::red);
      currentMode = "DISABLED";
    }
  Brain.Screen.setCursor(11, 12);
  Brain.Screen.print(currentMode.c_str());
  Brain.Screen.setPenColor(color::white);

  //Motor Debug
  std::map <std::string, int> motorDebugFrame = {
        {"x-left", 199},
        {"x-right", 480},
        {"y-top", 0},
        {"y-bottom", 240}     
    };  
  Brain.Screen.drawRectangle(motorDebugFrame["x-left"], motorDebugFrame["y-top"], motorDebugFrame["x-right"] - motorDebugFrame["x-left"], motorDebugFrame["y-bottom"] - motorDebugFrame["y-top"], maroon);
  int row = 3;
  Brain.Screen.setCursor(1, 24);
  Brain.Screen.print("Motor Information:");
  std::vector<std::vector<std::string>> motorInfo = myMotorCollection.returnValues();
  std::vector<bool> motorConnections = myMotorCollection.isConnected();
  for(int i = 0; i < motorInfo.size(); i++){
    Brain.Screen.setCursor(row, 21);
    if(motorConnections[i] == true){
      Brain.Screen.setPenColor(color::green);
    }
    else{
      Brain.Screen.setPenColor(color::red);
    }
    Brain.Screen.print(motorInfo[i][0].c_str());
    Brain.Screen.print(motorInfo[i][1].c_str());
    if(motorInfo[i][2] == "Green"){
      Brain.Screen.setPenColor(color::green);
    }
    else{
      Brain.Screen.setPenColor(color::red);
    }
    Brain.Screen.print(motorInfo[i][2].c_str());
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.print(motorInfo[i][3].c_str());
    Brain.Screen.print(motorInfo[i][4].c_str());
    Brain.Screen.print(motorInfo[i][5].c_str());
    int motorTemp = atoi(motorInfo[i][6].c_str());
    if(motorTemp < 104){
      Brain.Screen.setPenColor(color::green);
    }
    else if(motorTemp < 122){
      Brain.Screen.setPenColor(color::orange);
    }
    else{
      Brain.Screen.setPenColor(color::red);
    }
    Brain.Screen.print(motorInfo[i][6].c_str());
    //Add color based on temp
    Brain.Screen.print(motorInfo[i][7].c_str());
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.print(motorInfo[i][8].c_str());
    Brain.Screen.print(motorInfo[i][9].c_str());
    Brain.Screen.print(motorInfo[i][10].c_str());
    row++;
  }
}

void autonSelection(){
    int localLastTouchX = -1; //Init int to log the x-coordinate where the user last touched the Brain's screen
    int localLastTouchY = -1; //Init int to log the y-coordinate where the user last touched the Brain's screen
    waitingForUserInput = true;
    // drawGUI();
    while(waitingForUserInput){
      if(Competition.isEnabled()){
        waitingForUserInput = false;
        break;
      }
      //Logs the last x and y position of the user's touch to the localLastTouchX, localLastTouchY vars respectively
      if(Brain.Screen.pressing()){
          localLastTouchX = Brain.Screen.xPosition();
          localLastTouchY = Brain.Screen.yPosition();
      
        //Checks if the user touches the R2B1 Auton Btn
        if(localLastTouchX >= autonSelectorFrame["x-left"] && localLastTouchX <= autonSelectorFrame["x-right"] && localLastTouchY >= autonSelectorFrame["y-top"] && localLastTouchY <= autonSelectorFrame["y-bottom"]){
            if(autonSelector < 2){
              autonSelector++;
            }
            else{
              autonSelector = 0;
            }

            wait(200, msec);
            drawGUI();
        }
      }
      if(autonSelectionBumper.pressing() == 1){
          if(autonSelector < 2){
            autonSelector++;
          }
          else{
            autonSelector = 0;
          }  
          drawGUI();   
      }
      if(autonConfirmationBumper.pressing() == 1){
        waitingForUserInput = false; 
      }
      wait(20, msec);
    }
  drawGUI();
}



/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  myMotorCollection.addMotor(leftArm, "LA");
  myMotorCollection.addMotor(rightArm, "RA");
  myMotorCollection.addMotor(leftBack, "LB");
  myMotorCollection.addMotor(leftFront, "LF");
  myMotorCollection.addMotor(rightBack, "RB");
  myMotorCollection.addMotor(rightFront, "RF");

  autonSelector = 0;
  drawGUI();
  autonSelection();
  allMotors.setMaxTorque(100, vex::percentUnits::pct);
  allMotors.setVelocity(100, vex::percentUnits::pct);
  armMotors.setVelocity(50, vex::velocityUnits::pct);
  allMotors.setTimeout(5, vex::timeUnits::sec);
  nonDriveMotors.setStopping(vex::brakeType::hold);
  robotDrive.setTurnVelocity(50, vex::velocityUnits::pct);
  allMotors.resetPosition();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}





/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  drawGUI();
  myMotorCollection.isConnected();
  // drive(24, "fwd", 100);
  // turn(90, "left", 50);
  // drive(12, "rev", 100);
  // turn(45, "right", 50);
  
  if(autonSelector == 0){
    vex::task PIDTask(drivePID);
    resetDriveSensors = true;
    desiredValue = 600;
    vex::task::sleep(100);

    resetDriveSensors = true;
    desiredTurnValue = 300;
  }
  else if(autonSelector == 1){
    //Alternative Auton
  }
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  drawGUI();
  enableDrivePID = false;
  std::string driveConfig = myMotorCollection.checkMotors(); //Checks motor statuses and switches drive mode (4-wheel, front-wheel, rear-wheel, LFRB, RFLB)
  // User control code here, inside the loop
  while (1) {
    //Drive Controls
    if(driveConfig == "rearWheel"){
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
    }
    else if(driveConfig == "frontWheel"){
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
    }
    else if(driveConfig == "RFLB"){
      rightFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);     
    }
    else if(driveConfig == "LFRB"){
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);     
    }
    else{
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct); 
    }

    //Testing Degree Values for Drive
    // Brain.Screen.print(leftFront.position(vex::rotationUnits::deg) + leftBack.position(vex::rotationUnits::deg) + rightFront.position(vex::rotationUnits::deg) + rightBack.position(vex::rotationUnits::deg));
   
    //Testing Degree Values for Arm
    // Brain.Screen.print(armMotors.position(vex::rotationUnits::deg));
    
    if(Controller1.ButtonB.pressing()){
      Brain.Screen.clearScreen();
    }

    if(Controller1.ButtonX.pressing()){
      driveConfig = myMotorCollection.checkMotors();
      drawGUI();
    }

    //Intake Controls
    if(Controller1.ButtonL1.pressing()){
      chainIntake.spin(vex::directionType::fwd);
    }
    else if(Controller1.ButtonL2.pressing()){
      chainIntake.spin(vex::directionType::rev);
    }
    else{
      chainIntake.stop();
    }

    //Arm Controls
    if(Controller1.ButtonUp.pressing()){
      if(Controller1.ButtonR1.pressing()){
        armMotors.spin(vex::directionType::fwd);
      }
      else if(Controller1.ButtonR2.pressing()){
        armMotors.spin(vex::directionType::rev);
      }
      else{
        armMotors.stop();
      }
    }
    else{
      if(Controller1.ButtonR1.pressing()){
        if(armMotors.position(vex::rotationUnits::deg) < 200){
          armMotors.spinToPosition(200, vex::rotationUnits::deg);
        }
        else if(armMotors.position(vex::rotationUnits::deg) < 400){
          armMotors.spinToPosition(400, vex::rotationUnits::deg);
        }
        else if(armMotors.position(vex::rotationUnits::deg) < 600){
          armMotors.spinToPosition(600, vex::rotationUnits::deg);
        }
        else if(armMotors.position(vex::rotationUnits::deg) < 800){
          armMotors.spinToPosition(800, vex::rotationUnits::deg);
        }
      } 
      else if(Controller1.ButtonR2.pressing()){
        if(armMotors.position(vex::rotationUnits::deg) >=800){
          armMotors.spinToPosition(600, vex::rotationUnits::deg);
        }
        else if(armMotors.position(vex::rotationUnits::deg) >= 600){
          armMotors.spinToPosition(400, vex::rotationUnits::deg);
        }
        else if(armMotors.position(vex::rotationUnits::deg) >= 400){
          armMotors.spinToPosition(200, vex::rotationUnits::deg);
        }
        else if(armMotors.position(vex::rotationUnits::deg) >= 200){
          armMotors.spinToPosition(0, vex::rotationUnits::deg);
        }
      }
      else{
        armMotors.stop();
      }
    }
    
    
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}