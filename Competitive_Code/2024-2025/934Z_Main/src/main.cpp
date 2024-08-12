/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Leo Abubucker                                             */
/*    Created:      05/05/2024                                                */
/*    Description:  The main code for VEX VRC Team 934Z's 2024-2025 Season    */
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

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                      GLOBAL DECLARATIONS AND INITIALIZATIONS                       */
/*  Declarations and initializations of VEX and non-VEX global variables              */
/*  VEX Declarations:                                                                 */
/*  - competition Competition - manages connection between the robot to the VEX       */
/*      competition manager                                                           */
/*  - brain Brain - represents the VEX V5 Brain                                       */
/*  VEX Initializations:                                                              */
/*  - controller Controller1 - represents the VEX v5 Controller constructed as the    */
/*      "primary" or the "partner" controller                                         */
/*  - motor motorName - represents a VEX V5 Motor constructed with a PORT from        */
/*      "PORT1" to "PORT21", a gear ratio (Green = "ratio18_1", Red = "ratio36_1"),   */
/*      and a boolean representing whether the motor should spin in reverse or not.   */
/*  - motor_group motorGroupName - represents a group of motors constructed with the  */
/*      motors in the group. primarily used for mass initializations of motor         */
/*      attributes.                                                                   */
/*  - triport myTriport - represents the 3-wire ports on the VEX V5 Brain             */
/*  - pneumatics pneumaticSolenoidName - represents a pneumatic solenoid constructed  */
/*      with a specified triport port. Solenoids are double-acting unless otherwise   */
/*      specified.                                                                    */
/*  - bumper bumperName - represents a VEX bumper constructed with a specified        */
/*      triport port.                                                                 */
/*  Non-VEX Declarations:                                                             */
/*  - int autonSelector - integer representing the autonomous program that the user   */
/*      selects to run.                                                               */
/*  - std::map<std::string, int> autonSelectorFrame - map representing the            */
/*      coordinates of the autonomous selector GUI button on the brain.               */
/*  Non-VEX Initializations:                                                          */
/*  - bool waitingForUserInput = false - boolean representing whether the user has    */
/*      provided input confirming the autonomous program to be run.                   */
/*------------------------------------------------------------------------------------*/

// VEX Declarations
competition Competition;
brain Brain;

// VEX Initializations
controller Controller1 = controller(primary);
motor leftIntake = motor(PORT14, ratio18_1, true);
motor rightIntake = motor(PORT16, ratio18_1, false);
motor leftArm = motor(PORT3, ratio36_1, false);
motor rightArm = motor(PORT18, ratio36_1, true);
motor leftBack = motor(PORT5, ratio18_1, true);
motor leftFront = motor(PORT11, ratio18_1, true);
motor rightBack = motor(PORT10, ratio18_1, false);
motor rightFront = motor(PORT19, ratio18_1, false);
motor_group allMotors = motor_group(rightIntake, leftArm, rightArm, leftBack, leftFront, rightBack, rightFront);
motor_group driveMotors = motor_group(leftBack, leftFront, rightBack, rightFront);
motor_group leftDriveMotors = motor_group(leftFront, leftBack);
motor_group rightDriveMotors = motor_group(rightFront, rightBack);
motor_group nonDriveMotors = motor_group(rightIntake, leftArm, rightArm);
motor_group armMotors = motor_group(leftArm, rightArm);
motor_group intakeMotors = motor_group(leftIntake, rightIntake);
triport myTriport = triport(Brain.ThreeWirePort);
pneumatics frontClamp = pneumatics(myTriport.A);
pneumatics backClamp = pneumatics(myTriport.C);
bumper autonSelectionBumper = bumper(myTriport.E);
bumper autonConfirmationBumper = bumper(myTriport.G);

// Non-VEX Declarations
int autonSelector;
std::map<std::string, int> autonSelectorFrame;

// Non-VEX Intializations
bool waitingForUserInput = false;

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       HELPER FUNCTIONS AND CLASSES                                 */
/*                                                                                    */
/*  Helper functions and classes that assist VEX competition controlled functions.    */
/*  Includes:                                                                         */
/*  - std::string to_string(T value) - To string template function                    */
/*  - void turn(int degrees, std::string direction, int velocity) - Autonomous turn   */
/*    function                                                                        */
/*  - void drive(int inches, std::string direction, int velocity) - Autonomous drive  */
/*    function                                                                        */
/*  - PID control - NOT FUNCTIONAL, LEAVE COMMENTED OUT                               */
/*  - Motor Collection Class - Provides additional functionality for VEX V5 motors    */
/*  - std::string getCompetitionStatus() - gets the current competition state         */
/*  - vex::color getColorFromValue(std::string value, std::string keyword) - returns  */
/*    a color based on the given value and keyword                                    */
/*  - vex::color getColorFromValue(bool value, std::string keyword) - returns         */
/*    a color based on the given value and keyword                                    */
/*  - vex::color getColorFromValue(int value, std::string keyword) - returns          */
/*    a color based on the given value and keyword                                    */
/*  - void drawControlsFrame() - draws the control frame                              */
/*  - void drawAutonSelectorFrame() - draws the autononomous selection frame          */
/*  - void drawModeDisplayFrame() - draws the mode display frame                      */
/*  - void drawMotorDebugFrame() - draws the motor debug frame                        */
/*  - void drawBatteryInfoFrame() - draws the battery info frame                      */
/*  - void drawControllerInfoFrame() - draws the control frame                        */
/*  - void drawGUI() - calls the six draw functions above                             */
/*  - void autonSelection() - Autonomous program selection function                   */
/*------------------------------------------------------------------------------------*/

/**
 * @brief Converts a value to a string in a method that allows printing to the VEX V5 Brain and/or Controller
 * @tparam T
 * @param value value to be converted to a std::string
 * @returns value converted to a std::string
 * @author James Pearman
 * @cite https://www.vexforum.com/t/std-to-string-not-working-in-vc-and-vcs/62962/7
 */
template <typename T>
std::string to_string(T value)
{
  std::ostringstream os;
  os << value;
  return os.str();
}

/**
 * @brief automated turn movement based on given parameters
 * @details This function uses the benchmark of how many motor degrees it took to turn 90 degrees
 * in order to convert the inputted degrees rotation to motor degrees. It then moves the
 * robot's four drive motors for the calculated motor degrees, the inputted direction,
 * at the inputted velocity percent.
 * @relates autonomous()
 * @param degrees integer degrees of robot movement
 * @param direction std::string either "left" or "right" representing direction for movement
 * @param velocity integer percent velocity for robot movement
 * @authors Leo Abubucker, Jack Deise
 * @date 07/28/2024
 */
void turn(double degrees, std::string direction, int velocity)
{
  // Constant benchmarks
  const int motorDegreesFor90DegreeTurn = 257;
  const double motorDegreesPerDegreeTurn = motorDegreesFor90DegreeTurn / 90.0;

  // Calculates the degrees the motors have to turn to turn the robot for the inputted degrees
  double motorDegrees = motorDegreesPerDegreeTurn * degrees;

  // Inverts spin direction to turn left
  if (direction == "left")
  {
    motorDegrees *= -1;
  }

  // Spins the robot's four drive motors based on the given and calculated parameters
  leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  rightBack.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  rightFront.spinFor(vex::directionType::fwd, motorDegrees * -1, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

/**
 * @brief automated linear movement based on given parameters
 * @details This function uses the benchmark of how many motor degrees it took to drive 24 inches
 * in order to convert the inputted inches to motor degrees. It then moves the robot's
 * four drive motors for the calculated motor degrees, the inputted direction, at the
 * inputted velocity percent.
 * @relates autonomous()
 * @param inches integer inches of robot movement
 * @param direction std::string either "fwd" or "rev" representing direction for movement
 * @param velocity integer percent velocity for robot movement
 * @author Leo Abubucker, Jack Deise
 * @date 07/28/2024
 */
void drive(double inches, std::string direction, int velocity)
{
  // Constant benchmarks
  const int motorDegreesFor24Inches = 600;
  const double motorDegreesPerInch = motorDegreesFor24Inches / 24.25;

  // Calculate the degrees the motors have to turn to drive the robot inches Inches
  double motorDegrees = motorDegreesPerInch * inches;

  // Inverts spin direction to move backwards
  if (direction == "rev")
  {
    motorDegrees *= -1;
  }

  // Spins the robot's four drive motors based on the given and calculated parameters
  leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  rightBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  rightFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

/*---------------------------------------------------------------------------*/
/*                        PID CONTROL                                        */
/*  NOT FUNCTIONAL - LEAVE COMMENTED OUT                                     */
/*---------------------------------------------------------------------------*/
/// @bug
/* START BLOCK COMMENT FOR BUG

// Constant Settings
double kP = 1.0;
double kI = 0.0;
double kD = 0.0;

double turnkP = 0.0;
double turnkI = 0.0;
double turnkD = 0.0;

//Dynamic Settings
int desiredValue = 0;
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

 END BLOCK COMMENT FOR BUG */

/**
 * @brief MotorCollection class provides additional functionality to VEX V5 motors
 * @details This class provides additional functionality in detecting the connection
 * status of motors in the collection, dynamically updating the drive configuration
 * based on the connection status of the drive motors, and returns attributes
 * of the motors.
 * @author Leo Abubucker
 * @date 06/14/2024
 */
class MotorCollection
{
public:
  // Init vector of motor objects
  std::vector<vex::motor> motorList;
  // Init vector of std::strings representing motor names
  std::vector<std::string> motorNamesList;

  /**
   * @brief adds the passed in motor object and std::string object motorName to respective lists
   * @relates pre_auton()
   * @param newMotor VEX V5 motor
   * @param newMotorName std::string name of newMotor
   * @author Leo Abubucker
   * @date 06/14/2024
   */
  void addMotor(motor newMotor, std::string newMotorName)
  {
    motorList.push_back(newMotor);
    motorNamesList.push_back(newMotorName);
  }

  /**
   * @brief returns motor connection values in a vector; prints disconnections to Controller
   * @details creates an std::vector containing booleans representing the connection status of
   * the motors. Prints the names of any disconnected motors to the Controller screen.
   * @relates checkMotors()
   * @returns std::vector containing booleans representing connections statuses of the motors
   * @author Leo Abubucker
   * @date 06/14/2024
   */
  std::vector<bool> isConnected()
  {
    Controller1.Screen.clearLine();
    Controller1.Screen.setCursor(0, 0);
    std::vector<bool> motorConnections;
    for (int i = 0; i < motorList.size(); i++)
    {
      motorConnections.push_back(motorList[i].installed());
      if (!motorList[i].installed())
      {
        Controller1.Screen.print(motorNamesList[i].c_str());
        Controller1.Screen.print(" ");
      }
    }
    return motorConnections;
  }

  /**
   * @brief determines the optimal drive configuration based on the connection of drive motors
   * @relates usercontrol()
   * @returns std::string keyword representing the drive configuration
   * @author Leo Abubucker
   * @date 06/14/2024
   */
  std::string checkMotors()
  {
    std::vector<bool> motorConnections = isConnected();
    std::vector<std::string> disconnectedMotorNames;
    std::string updatedDriveConfig = "fourWheel";
    for (int i = 0; i < motorConnections.size(); i++)
    {
      if (motorConnections[i] == false)
      {
        disconnectedMotorNames.push_back(motorNamesList[i]);
      }
    }
    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LF") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(),
                                                                                                                                   disconnectedMotorNames.end(), "RF") != disconnectedMotorNames.end())
    {
      updatedDriveConfig = "rearWheel";
    }

    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LB") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(),
                                                                                                                                   disconnectedMotorNames.end(), "RB") != disconnectedMotorNames.end())
    {
      updatedDriveConfig = "frontWheel";
    }

    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LF") != disconnectedMotorNames.end() && std::find(disconnectedMotorNames.begin(),
                                                                                                                                   disconnectedMotorNames.end(), "RB") != disconnectedMotorNames.end())
    {
      updatedDriveConfig = "RFLB";
    }
    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LB") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(),
                                                                                                                                   disconnectedMotorNames.end(), "RF") != disconnectedMotorNames.end())
    {
      updatedDriveConfig = "LFRB";
    }
    return updatedDriveConfig;
  }

  /**
   * @brief retrieves motor attributes and returns them in a formatted 2D std::string
   * @relates drawMotorDebugFrame()
   * @returns 2D vector where each value is [name: cartridge, type, temperatureF, positiondeg]
   * @author Leo Abubucker
   * @date 06/14/2024
   */
  std::vector<std::vector<std::string>> returnValues()
  {
    std::vector<std::vector<std::string>> returnedList;
    std::string motorCartridgeType;
    std::string motorType;

    for (int i = 0; i < motorList.size(); i++)
    {
      if (motorList[i].getMotorCartridge() == vex::gearSetting::ratio36_1)
      {
        motorCartridgeType = "Red";
      }
      else if (motorList[i].getMotorCartridge() == vex::gearSetting::ratio18_1)
      {
        motorCartridgeType = "Green";
      }
      else
      {
        motorCartridgeType = "Blue";
      }

      if (motorList[i].getMotorType() == 0)
      {
        motorType = "11W";
      }
      else
      {
        motorType = "5.5W";
      }
      returnedList.push_back({motorNamesList[i], ": ", motorCartridgeType, ", ", motorType, ", ", to_string(motorList[i].temperature(vex::temperatureUnits::fahrenheit)), "F", ", ", to_string(motorList[i].position(vex::rotationUnits::deg)), "deg"});
    }
    return returnedList;
  }
};

// Initialization of MotorCollection
MotorCollection myMotorCollection;

/**
 * @brief gets the state of the VEX V5 Competition Control as a String
 * @relates drawModeDisplayFrame()
 * @returns std::string "AUTON", "DRIVER", "PRE-AUTON", or "DISABLED"
 * @author Leo Abubucker
 * @date 07/21/2024
 */
std::string getCompetitionStatus()
{
  if (Competition.isEnabled())
  {
    if (Competition.isAutonomous())
    {
      return "AUTON";
    }
    else if (Competition.isDriverControl())
    {
      return "DRIVER";
    }
    else
    {
      return "PRE-AUTON";
    }
  }
  else
  {
    return "DISABLED";
  }
}

/**
 * @brief gets a vex::color based on an std::string value and std::string keyword
 * @details This function takes an std::string keyword and std::string value and returns a vex::color based
 * on specific condition(s) that the value meets. The conditions that are used are based on the keyword.
 * @relates drawModeDisplayFrame(), drawMotorDebugFrame()
 * @overload getColorFromValue(bool value, std::string keyword)
 * @overload getColorFromVaue(int value, std::string keyword)
 * @param value std::string value that determines color
 * @param keyword std::string keyword that determines which conditions value is checked. Valid keywords are "gameState" or "motorCartridge"
 * @returns vex::color determined by value. Returns color::white if no value, keyword pair matches.
 * @author Leo Abubucker
 * @date 07/21/2024
 */
vex::color getColorFromValue(std::string value, std::string keyword)
{
  if (keyword == "gameState")
  {
    if (value == "AUTON")
    {
      return color::orange;
    }
    else if (value == "DRIVER")
    {
      return color::green;
    }
    else if (value == "PRE-AUTON")
    {
      return color::yellow;
    }
    else
    {
      return color::red;
    }
  }
  else if (keyword == "motorCartridge")
  {
    if (value == "Green")
    {
      return color::green;
    }
    else if (value == "Red")
    {
      return color::red;
    }
    else
    {
      return color::blue;
    }
  }
  return color::white;
}

/**
 * @brief gets a vex::color based on a bool value and std::string keyword
 * @details This function takes a bool keyword and std::string value and returns a vex::color based
 * on specific condition(s) that the value meets. The conditions that are used are based on the keyword.
 * @relates drawAutonSelectorFrame(), drawMotorDebugFrame(), drawControllerInfoFrame()
 * @overload getColorFromValue(std::string value, std::string keyword)
 * @overload getColorFromVaue(int value, std::string keyword)
 * @param value bool value that determines color
 * @param keyword std::string keyword that determines which conditions value is checked. Valid keywords are "gameState" or "motorCartridge"
 * @returns vex::color determined by value. Returns color::white if no value, keyword pair matches.
 * @author Leo Abubucker
 * @date 07/28/2024
 */
vex::color getColorFromValue(bool value, std::string keyword)
{
  if (keyword == "userInput")
  {
    if (value)
    {
      return color::orange;
    }
    else
    {
      return color::green;
    }
  }
  else if (keyword == "motorConnection")
  {
    if (value)
    {
      return color::green;
    }
    else
    {
      return color::red;
    }
  }
  else if (keyword == "controllerConnection")
  {
    if (value)
    {
      return color::green;
    }
    else
    {
      return color::red;
    }
  }
  return color::white;
}

/**
 * @brief gets a vex::color based on an int value and std::string keyword
 * @details This function takes an int keyword and std::string value and returns a vex::color based
 * on specific condition(s) that the value meets. The conditions that are used are based on the keyword.
 * @relates drawMotorDebugFrame(), drawBatteryInfoFrame()
 * @overload getColorFromValue(std::string value, std::string keyword)
 * @overload getColorFromVaue(bool value, std::string keyword)
 * @param value int value that determines color
 * @param keyword std::string keyword that determines which conditions value is checked. Valid keywords are "gameState" or "motorCartridge"
 * @returns vex::color determined by value. Returns color::white if no value, keyword pair matches.
 * @author Leo Abubucker
 * @date 07/21/2024
 */
vex::color getColorFromValue(int value, std::string keyword)
{
  if (keyword == "motorTemperature")
  {
    if (value < 104)
    {
      return color::green;
    }
    else if (value < 122)
    {
      return color::orange;
    }
    else
    {
      return color::red;
    }
  }
  else if (keyword == "battery")
  {
    if (value > 80)
    {
      return color::green;
    }
    else if (value > 50)
    {
      return color::orange;
    }
    else
    {
      return color::red;
    }
  }
  return color::white;
}

/**
 * @brief draws the GUI controls frame
 * @relates drawGUI()
 * @author Leo Abubucker
 * @date 07/28/2024
 */
void drawControlsFrame()
{
  // Coordinates of the control frame
  std::map<std::string, int> controlsFrame = {
      {"x-left", 0},
      {"x-right", 182},
      {"y-top", 0},
      {"y-bottom", 180}};

  // Draws maroon rectangle at above coordinates
  Brain.Screen.drawRectangle(controlsFrame["x-left"], controlsFrame["y-top"], controlsFrame["x-right"] - controlsFrame["x-left"], controlsFrame["y-bottom"] - controlsFrame["y-top"], color(128, 0, 0));

  // Prints Controls in the frame
  std::vector<std::string> controls = {"Driving - Tank", "Lift Arm - R1", "Lower Arm - R2", "Intake - L1", "Outtake - L2", "Clamp - X", "Toggle - Up", "Update GUIs - Y"};
  Brain.Screen.setFillColor(color(128, 0, 0));
  Brain.Screen.setPenColor(color::white);
  Brain.Screen.setCursor(1, 5);
  Brain.Screen.print("Controls:");
  int row = 2;
  for (int i = 0; i < controls.size(); i++)
  {
    Brain.Screen.setCursor(row, 2);
    Brain.Screen.print(controls[i].c_str());
    row++;
  }
}

/**
 * @brief draws the GUI autonomous selector frame
 * @relates drawGUI()
 * @author Leo Abubucker
 * @date 07/21/2024
 */
void drawAutonSelectorFrame()
{
  // Coordinates of the autonomous selector frame
  autonSelectorFrame = {
      {"x-left", 0},
      {"x-right", 99},
      {"y-top", 179},
      {"y-bottom", 240}};

  // Draws maroon rectangle at above coordinates
  Brain.Screen.drawRectangle(autonSelectorFrame["x-left"], autonSelectorFrame["y-top"], autonSelectorFrame["x-right"] - autonSelectorFrame["x-left"], autonSelectorFrame["y-bottom"] - autonSelectorFrame["y-top"], color(128, 0, 0));

  // Gets and prints the color-coordinated current autonomous selection and whether the selection is locked or not in the frame
  Brain.Screen.setCursor(10, 2);
  Brain.Screen.print("Auton: ");
  Brain.Screen.print(autonSelector);
  Brain.Screen.setCursor(11, 2);
  Brain.Screen.setPenColor(getColorFromValue(waitingForUserInput, "userInput"));
  if (waitingForUserInput)
  {
    Brain.Screen.print("UNLOCKED");
  }
  else
  {
    Brain.Screen.print("LOCKED");
  }

  Brain.Screen.setPenColor(color::white);
}

/**
 * @brief draws the GUI mode display frame
 * @relates drawGUI()
 * @author Leo Abubucker
 * @date 07/28/2024
 */
void drawModeDisplayFrame()
{
  // Coordinates of the mode display frame
  std::map<std::string, int> modeDisplayFrame = {
      {"x-left", 98},
      {"x-right", 182},
      {"y-top", 179},
      {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coordinates
  Brain.Screen.drawRectangle(modeDisplayFrame["x-left"], modeDisplayFrame["y-top"], modeDisplayFrame["x-right"] - modeDisplayFrame["x-left"], modeDisplayFrame["y-bottom"] - modeDisplayFrame["y-top"], color(128, 0, 0));

  // Gets and prints the color-coordinated current game state in the frame
  Brain.Screen.setCursor(10, 13);
  Brain.Screen.print("Mode: ");
  std::string currentMode = getCompetitionStatus();
  Brain.Screen.setPenColor(getColorFromValue(currentMode, "gameState"));
  if (currentMode == "DISABLED")
  {
    Brain.Screen.setCursor(11, 11);
  }
  else
  {
    Brain.Screen.setCursor(11, 12);
  }
  Brain.Screen.print(currentMode.c_str());
  Brain.Screen.setPenColor(color::white);
}

/**
 * @brief draws the GUI motor debug frame
 * @relates drawGUI()
 * @author Leo Abubucker
 * @date 07/28/2024
 */
void drawMotorDebugFrame()
{
  // Coordinates of the motor debug frame
  std::map<std::string, int> motorDebugFrame = {
      {"x-left", 181},
      {"x-right", 479},
      {"y-top", 0},
      {"y-bottom", 180}};

  // Draws a maroon rectangle at the above coordinates
  Brain.Screen.drawRectangle(motorDebugFrame["x-left"], motorDebugFrame["y-top"], motorDebugFrame["x-right"] - motorDebugFrame["x-left"], motorDebugFrame["y-bottom"] - motorDebugFrame["y-top"], color(128, 0, 0));

  // Gets and prints color-coordinated debug information for all the motors in the frame
  Brain.Screen.setCursor(1, 24);
  Brain.Screen.print("Motor Information:");
  std::vector<std::vector<std::string>> motorInfo = myMotorCollection.returnValues();
  std::vector<bool> motorConnections = myMotorCollection.isConnected();
  int row = 2;
  for (int i = 0; i < motorInfo.size(); i++)
  {
    Brain.Screen.setCursor(row, 20);
    Brain.Screen.setPenColor(getColorFromValue(motorConnections[i], "motorConnection"));

    for (int j = 0; j < motorInfo[0].size(); j++)
    {
      switch (j)
      {
      case (2):
        Brain.Screen.setPenColor(getColorFromValue(motorInfo[i][j], "motorCartridge"));
        break;

      case (6):
      case (7):
        int motorTemp = atoi(motorInfo[i][j].c_str());
        Brain.Screen.setPenColor(getColorFromValue(motorTemp, "motorTemperature"));
        break;
      }
      Brain.Screen.print(motorInfo[i][j].c_str());
      Brain.Screen.setPenColor(color::white);
    }
    row++;
  }
}

/**
 * @brief draws the GUI battery info frame
 * @relates drawGUI()
 * @author Leo Abubucker
 * @date 07/28/2024
 */
void drawBatteryInfoFrame()
{
  // Coordinates of the battery info frame
  std::map<std::string, int> batteryInfoFrame = {
      {"x-left", 181},
      {"x-right", 276},
      {"y-top", 179},
      {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coordinates
  Brain.Screen.drawRectangle(batteryInfoFrame["x-left"], batteryInfoFrame["y-top"], batteryInfoFrame["x-right"] - batteryInfoFrame["x-left"], batteryInfoFrame["y-bottom"] - batteryInfoFrame["y-top"], color(128, 0, 0));

  // Gets and prints the color-coordinated battery percent in the frame
  Brain.Screen.setCursor(10, 20);
  Brain.Screen.print("Battery:");
  Brain.Screen.setCursor(11, 22);
  Brain.Screen.setPenColor(getColorFromValue((int)Brain.Battery.capacity(), "battery"));
  Brain.Screen.print("%d%%", Brain.Battery.capacity());
  Brain.Screen.setPenColor(color::white);
}

/**
 * @brief draws the GUI controller info frame
 * @relates drawGUI()
 * @author Leo Abubucker
 * @date 07/28/2024
 */
void drawControllerInfoFrame()
{
  // Coordinates of the controller info frame
  std::map<std::string, int> controllerInfoFrame = {
      {"x-left", 275},
      {"x-right", 405},
      {"y-top", 179},
      {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coords
  Brain.Screen.drawRectangle(controllerInfoFrame["x-left"], controllerInfoFrame["y-top"], controllerInfoFrame["x-right"] - controllerInfoFrame["x-left"], controllerInfoFrame["y-bottom"] - controllerInfoFrame["y-top"], color(128, 0, 0));
  Brain.Screen.setCursor(10, 29);
  Brain.Screen.print("Controller:");
  Brain.Screen.setCursor(11, 29);

  // Gets and prints the color-coordinated controller connection in the frame
  Brain.Screen.setPenColor(getColorFromValue(Controller1.installed(), "controllerConnection"));
  if (Controller1.installed())
  {
    Brain.Screen.print("CONNECTED");
  }
  else
  {
    Brain.Screen.print("DISCONNECTED");
  }

  Brain.Screen.setPenColor(color::white);
}

/**
 * @brief calls the six individual drawing functions to draw the entire GUI
 * @relates autonSelection(), pre_auton(), autonomous(), usercontrol()
 * @author Leo Abubucker
 * @date 07/21/2024
 */
void drawGUI()
{
  drawControlsFrame();
  drawAutonSelectorFrame();
  drawModeDisplayFrame();
  drawMotorDebugFrame();
  drawBatteryInfoFrame();
  drawControllerInfoFrame();
}

/**
 * @brief manages user input for the auton selector
 * @relates pre_auton()
 * @author Leo Abubucker
 * @date 07/21/2024
 */
void autonSelection()
{
  // Init int to log the x-coordinate where the user last touched the Brain's screen
  int localLastTouchX = -1;
  // Init int to log the y-coordinate where the user last touched the Brain's screen
  int localLastTouchY = -1;

  waitingForUserInput = true;
  while (waitingForUserInput)
  {
    if (Competition.isEnabled())
    {
      waitingForUserInput = false;
      break;
    }
    // Logs the last x and y position of the user's touch to the localLastTouchX, localLastTouchY vars respectively
    if (Brain.Screen.pressing())
    {
      localLastTouchX = Brain.Screen.xPosition();
      localLastTouchY = Brain.Screen.yPosition();

      // Checks if the user touches the on-screen auton selection button
      if (localLastTouchX >= autonSelectorFrame["x-left"] && localLastTouchX <= autonSelectorFrame["x-right"] && localLastTouchY >= autonSelectorFrame["y-top"] && localLastTouchY <= autonSelectorFrame["y-bottom"])
      {
        if (autonSelector < 2)
        {
          autonSelector++;
        }
        else
        {
          autonSelector = 0;
        }

        wait(200, msec);
        drawGUI();
      }
    }

    // Checks if the user presses the physical auton selection bumper
    if (autonSelectionBumper.pressing() == 1)
    {
      if (autonSelector < 2)
      {
        autonSelector++;
      }
      else
      {
        autonSelector = 0;
      }
      drawGUI();
    }

    // Checks if the user presses the physical auton confirmation bumper
    if (autonConfirmationBumper.pressing() == 1)
    {
      waitingForUserInput = false;
    }
    wait(20, msec);
  }

  drawGUI();
}

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       VEX COMPETITION CONTROLLED FUNCTIONS                         */
/*                                                                                    */
/*  VEX competition controlled functions are those that are automatically called by   */
/*  VEX tournament management systems and should not be manually called except by     */
/*  the VEX competition controlled function main().                                    */
/*  Includes:                                                                         */
/*  - void pre_auton() - pre-game initializations, GUI loading, auton selection       */
/*  - void autonomous() - update GUI, check motors, 15 sec autonomous robot movement  */
/*  - void usercontrol() - update GUI, check motors, 1m45s loop of user-controlled    */
/*    robot movement                                                                  */
/*  - int main() - controls all other VEX controlled functions - DO NOT EDIT          */
/*------------------------------------------------------------------------------------*/

/**
 * @brief VEX Competition Controlled Function: pre-game initializations, GUI loading, auton selection prompting
 * @relates main()
 * @author Leo Abubucker
 * @date 07/21/2024
 */
void pre_auton(void)
{
  myMotorCollection.addMotor(leftArm, "LA");
  myMotorCollection.addMotor(rightArm, "RA");
  myMotorCollection.addMotor(leftBack, "LB");
  myMotorCollection.addMotor(leftFront, "LF");
  myMotorCollection.addMotor(rightBack, "RB");
  myMotorCollection.addMotor(rightFront, "RF");
  myMotorCollection.addMotor(rightIntake, "I");
  autonSelector = 0;
  drawGUI();
  autonSelection();
  allMotors.setMaxTorque(100, vex::percentUnits::pct);
  allMotors.setVelocity(100, vex::percentUnits::pct);
  armMotors.setVelocity(100, vex::velocityUnits::pct);
  allMotors.setTimeout(5, vex::timeUnits::sec);
  nonDriveMotors.setStopping(vex::brakeType::hold);
  allMotors.resetPosition();
}

/**
 * @brief VEX Competition Controlled Function: update GUI, check motors, 15 seconds of autonomous robot movement
 * @relates main()
 * @authors Leo Abubucker, Jack Deise
 * @date 08/04/2024
 */
void autonomous(void)
{
  drawGUI();
  myMotorCollection.isConnected();

  /// @bug
  /* START BLOCK COMMENT FOR BUG

  vex::task PIDTask(drivePID);
  resetDriveSensors = true;
  desiredValue = 600;
  vex::task::sleep(100);
  resetDriveSensors = true;
  desiredTurnValue = 300;

  END BLOCK COMMENT FOR BUG */

  if (autonSelector == 0)
  {
    // Main Auton
    armMotors.spinToPosition(505, vex::rotationUnits::deg, true);
    drive(22.5, "fwd", 75);
    rightIntake.spinToPosition(-225, vex::rotationUnits::deg, true);
    armMotors.spinToPosition(350, vex::rotationUnits::deg, true);
    rightIntake.spinToPosition(-725, vex::rotationUnits::deg, true);
    drive(1, "rev", 75);
    drive(1, "fwd", 75);
    drive(22.5, "rev", 75);
    turn(90, "left", 50);
    armMotors.spinToPosition(0, vex::rotationUnits::deg, true);
    //drive(33.5, "fwd", 75);
    armMotors.spinToPosition(130, vex::rotationUnits::deg, true);
    //drive(20.5, "rev", 75);
    // turn(140, "right", 75);
    // armMotors.spinToPosition(450, vex::rotationUnits::deg, true);
    // drive(10, "fwd", 75);
    // armMotors.spinToPosition(215,vex::rotationUnits::deg, true);
    // chainIntake.spinToPosition(200 ,vex::rotationUnits::deg, true);
    //turn(70, "left", 50);
    //armMotors.spinToPosition(505 ,vex::rotationUnits::deg, true);
    //drive(25, "fwd", 100);
    //chainIntake.spinToPosition(-300, vex::rotationUnits::deg, true);
    //armMotors.spinToPosition(350, vex::rotationUnits::deg, true);
    //chainIntake.spinToPosition(-800, vex::rotationUnits::deg, true);
    //drive(7, "rev", 25);

  }
  else if (autonSelector == 1)
  {
    // Alternative Auton
  }
  else if (autonSelector == 2)
  {
    // NO AUTON - LEAVE BLANK
  }
}

/**
 * @brief VEX Competition Controlled Function: update GUI, check motors, 1 minute 45 second loop of user-controlled robot movement
 * @relates main()
 * @author Leo Abubucker
 * @date 07/28/2024
 */
void usercontrol(void)
{
  drawGUI();
  bool frontClampState = false;
  bool frontClampLastState = false;
  bool backClampState = false;
  bool backClampLastState = false;

  /// @bug enableDrivePID = false;

  int timeCheck = 0;
  // Checks motor statuses and switches drive mode (4-wheel, front-wheel, rear-wheel, LFRB, RFLB)
  std::string driveConfig = myMotorCollection.checkMotors();

  // User control code here, inside the loop
  while (1)
  {
    // Check Motors and Update GUI
    if (Controller1.ButtonY.pressing())
    {
      driveConfig = myMotorCollection.checkMotors();
      drawGUI();
    }

    // Time update on controller at 1 minute, 30 seconds, and 10 seconds
    if (atoi(to_string(Brain.timer(vex::timeUnits::sec)).c_str()) >= 63 && timeCheck == 0)
    {
      Controller1.Screen.clearLine();
      Controller1.rumble(".");
      Controller1.Screen.print("1 Minute Remaining");
      timeCheck++;
    }
    else if (atoi(to_string(Brain.timer(vex::timeUnits::sec)).c_str()) >= 93 && timeCheck == 1)
    {
      Controller1.Screen.clearLine();
      Controller1.rumble(". .");
      Controller1.Screen.print("30 Seconds Remaining");
      timeCheck++;
    }
    else if (atoi(to_string(Brain.timer(vex::timeUnits::sec)).c_str()) >= 113 && timeCheck == 2)
    {
      Controller1.Screen.clearLine();
      Controller1.rumble(". . .");
      Controller1.Screen.print("10 Seconds Remaining");
      timeCheck++;
    }

    // Drive Controls
    if (driveConfig == "rearWheel")
    {
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }
    else if (driveConfig == "frontWheel")
    {
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }
    else if (driveConfig == "RFLB")
    {
      rightFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }
    else if (driveConfig == "LFRB")
    {
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }
    else
    {
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }

    // Front Clamp Controls
    if (Controller1.ButtonX.pressing() && !frontClampLastState)
    {
      frontClampState = !frontClampState;
      frontClampLastState = true;
    }
    else if (!Controller1.ButtonX.pressing())
    {
      frontClampLastState = false;
    }
    if (frontClampState)
    {
      frontClamp.set(true);
    }
    else
    {
      frontClamp.set(false);
    }

    // Back Clamp Controls
    if (Controller1.ButtonB.pressing() && !backClampLastState)
    {
      backClampState = !backClampState;
      backClampLastState = true;
    }
    else if (!Controller1.ButtonB.pressing())
    {
      backClampLastState = false;
    }

    if (backClampState)
    {
      backClamp.set(true);
    }
    else
    {
      backClamp.set(false);
    }

    // Intake Controls
    if (Controller1.ButtonL1.pressing())
    {
      intakeMotors.spin(vex::directionType::fwd);
    }
    else if (Controller1.ButtonL2.pressing())
    {
      intakeMotors.spin(vex::directionType::rev);
    }
    else
    {
      intakeMotors.stop();
    }

    // Arm Controls
    if (Controller1.ButtonUp.pressing())
    {
      if (Controller1.ButtonR1.pressing())
      {
        if (armMotors.position(vex::rotationUnits::deg) < 520)
        {
          armMotors.spinToPosition(520, vex::rotationUnits::deg);
        }
        else if (armMotors.position(vex::rotationUnits::deg) < 600)
        {
          armMotors.spinToPosition(600, vex::rotationUnits::deg);
        }
        else if (armMotors.position(vex::rotationUnits::deg) < 711)
        {
          armMotors.spinToPosition(711, vex::rotationUnits::deg);
        }
        else if (armMotors.position(vex::rotationUnits::deg) < 922)
        {
          armMotors.spinToPosition(922, vex::rotationUnits::deg);
        }
      }
      else if (Controller1.ButtonR2.pressing())
      {
        if (armMotors.position(vex::rotationUnits::deg) >= 922)
        {
          armMotors.spinToPosition(711, vex::rotationUnits::deg);
        }
        else if (armMotors.position(vex::rotationUnits::deg) >= 711)
        {
          armMotors.spinToPosition(600, vex::rotationUnits::deg);
        }
        else if (armMotors.position(vex::rotationUnits::deg) >= 600)
        {
          armMotors.spinToPosition(520, vex::rotationUnits::deg);
        }
        else if (armMotors.position(vex::rotationUnits::deg) >= 520)
        {
          armMotors.spinToPosition(0, vex::rotationUnits::deg);
        }
      }
      else
      {
        armMotors.stop();
      }
    }
    else
    {
      if (Controller1.ButtonR1.pressing())
      {
        armMotors.spin(vex::directionType::fwd);
      }
      else if (Controller1.ButtonR2.pressing())
      {
        armMotors.spin(vex::directionType::rev);
      }
      else
      {
        armMotors.stop();
      }
    }

    // Sleep the task for a short amount of time to prevent wasted resources - DO NOT REMOVE
    wait(20, msec);
  }
}

/**
 * @brief VEX Competition Controlled Function: controls all other VEX controlled functions - DO NOT EDIT
 * @author VEX
 * @date 05/05/2024
 */
int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}