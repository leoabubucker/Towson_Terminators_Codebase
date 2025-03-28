/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Authors:      Leo Abubucker, Jack Deise                                 */
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
/*  - enum DriveConfigurations{} - enums representing the 4 drive configs             */
/*  - DriveConfigurations driveConfig - global instance of DriveConfigurations        */
/*  - enum Keywords{} - enums representing the 7 keywords                             */
/*  - enum GameStates{} - enums representing the 5 game states                        */
/*  - enum MovementDirections{} - enums representing the 4 movement directions        */
/*  Non-VEX Initializations:                                                          */
/*  - bool waitingForUserInput = false - boolean representing whether the user has    */
/*      provided input confirming the autonomous program to be run.                   */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

// VEX Declarations
competition Competition;
brain Brain;

// VEX Initializations
controller Controller1 = controller(primary);
motor leftIntake = motor(PORT10, ratio18_1, true);
motor rightIntake = motor(PORT2, ratio18_1, false);
motor rightArm = motor(PORT1, ratio36_1, false);
motor leftArm = motor(PORT9, ratio36_1, true);
motor leftBack = motor(PORT11, ratio18_1, true);
motor leftFront = motor(PORT15, ratio18_1, true);
motor rightBack = motor(PORT13, ratio18_1, false);
motor rightFront = motor(PORT14, ratio18_1, false);
motor_group allMotors = motor_group(rightIntake, rightArm, leftArm, leftBack, leftFront, rightBack, rightFront, leftIntake);
motor_group driveMotors = motor_group(leftBack, leftFront, rightBack, rightFront);
motor_group nonDriveMotors = motor_group(rightIntake, rightArm, leftArm);
motor_group armMotors = motor_group(rightArm, leftArm);
motor_group intakeMotors = motor_group(leftIntake, rightIntake);
triport myTriport = triport(Brain.ThreeWirePort);
pneumatics clamp = pneumatics(myTriport.A);
bumper autonSelectionBumper = bumper(myTriport.E);
bumper autonConfirmationBumper = bumper(myTriport.C);

inertial inertialSensor = inertial(PORT20);

// Non-VEX Declarations
int autonSelector;
std::map<std::string, int> autonSelectorFrame;
enum DriveConfigurations
{
  FOUR_WHEEL,
  REAR_WHEEL,
  FRONT_WHEEL,
  RFLB,
  LFRB
};
DriveConfigurations driveConfig;
enum Keywords
{
  GAME_STATE,
  MOTOR_CARTRIDGE,
  USER_INPUT,
  MOTOR_CONNECTION,
  CONTROLLER_CONNECTION,
  MOTOR_TEMPERATURE,
  BATTERY
};
enum GameStates
{
  AUTONOMOUS,
  PRE_AUTONOMOUS,
  USER_CONTROL,
  DISABLED,
  ENABLED
};
enum MovementDirections
{
  LEFT,
  RIGHT,
  FORWARD,
  REVERSE
};

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
/*  - Motor Collection Class - Provides additional functionality for VEX V5 motors    */
/*  - GameStates getCompetitionStatus() - gets the current competition state          */
/*  - vex::color getColorFromValue(std::string value, Keywords keyword) - returns     */
/*    a color based on the given value and keyword                                    */
/*  - vex::color getColorFromValue(bool value, Keywords keyword) - returns            */
/*    a color based on the given value and keyword                                    */
/*  - vex::color getColorFromValue(int value, Keywords keyword) - returns             */
/*    a color based on the given value and keyword                                    */
/*  - vex::color getColorFromValue(GameStates value, Keywords keyword) - returns      */
/*    a color based on the given value and keyword                                    */
/*  - void drawControlsFrame() - draws the control frame                              */
/*  - void drawAutonSelectorFrame() - draws the autononomous selection frame          */
/*  - void drawModeDisplayFrame() - draws the mode display frame                      */
/*  - void drawMotorDebugFrame() - draws the motor debug frame                        */
/*  - void drawBatteryInfoFrame() - draws the battery info frame                      */
/*  - void drawControllerInfoFrame() - draws the control frame                        */
/*  - void drawGUI() - thread function that calls the six draw functions above        */
/*  - void autonSelection() - Autonomous program selection function                   */
/*  - void motorTracking() - thread function that routinely check status of motors    */
/*  - void timeTracking() - thread function that alerts controller w/ time updates    */
/*  - void autonTracking() - thread function tracks and interprets motor positions    */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

/**
 * @brief Converts a value to a string in a method that allows printing to the VEX V5 Brain and/or Controller
 * @tparam T
 * @param value value to be converted to a std::string
 * @returns value converted to a std::string
 * @author James Pearman
 * @cite https://www.vexforum.com/t/std-tostring-not-working-in-vc-and-vcs/62962/7
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
void turn(double degrees, MovementDirections direction, int velocity)
{
  // Constant benchmarks
  const int motorDegreesFor90DegreeTurn = 257;
  const double motorDegreesPerDegreeTurn = motorDegreesFor90DegreeTurn / 90.0;

  // Calculates the degrees the motors have to turn to turn the robot for the inputted degrees
  double motorDegrees = motorDegreesPerDegreeTurn * degrees;

  // Inverts spin direction to turn left
  if (direction == LEFT)
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
void drive(double inches, MovementDirections direction, int velocity)
{
  // Constant benchmarks
  const int motorDegreesFor24Inches = 600;
  const double motorDegreesPerInch = motorDegreesFor24Inches / 24.25;

  // Calculate the degrees the motors have to turn to drive the robot inches Inches
  double motorDegrees = motorDegreesPerInch * inches;

  // Inverts spin direction to move backwards
  if (direction == REVERSE)
  {
    motorDegrees *= -1;
  }

  // Spins the robot's four drive motors based on the given and calculated parameters
  leftBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  rightBack.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  leftFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  rightFront.spinFor(vex::directionType::fwd, motorDegrees, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

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

    std::vector<bool> motorConnections;
    for (int i = 0; i < motorList.size(); i++)
    {
      motorConnections.push_back(motorList[i].installed());
      if (!motorList[i].installed())
      {
        Controller1.Screen.clearLine();
        Controller1.Screen.setCursor(0, 0);
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
  void checkMotors()
  {
    std::vector<bool> motorConnections = isConnected();
    std::vector<std::string> disconnectedMotorNames;
    driveConfig = FOUR_WHEEL;
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
      driveConfig = REAR_WHEEL;
    }

    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LB") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(),
                                                                                                                                   disconnectedMotorNames.end(), "RB") != disconnectedMotorNames.end())
    {
      driveConfig = FRONT_WHEEL;
    }

    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LF") != disconnectedMotorNames.end() && std::find(disconnectedMotorNames.begin(),
                                                                                                                                   disconnectedMotorNames.end(), "RB") != disconnectedMotorNames.end())
    {
      driveConfig = RFLB;
    }
    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(), "LB") != disconnectedMotorNames.end() || std::find(disconnectedMotorNames.begin(),
                                                                                                                                   disconnectedMotorNames.end(), "RF") != disconnectedMotorNames.end())
    {
      driveConfig = LFRB;
    }
  }

  /**
   * @brief retrieves motor attributes and returns them in a formatted 2D std::string
   * @relates drawMotorDebugFrame()
   * @returns 2D vector where each value is [name: cartridge, type, temperatureF, positiondeg]
   * @author Leo Abubucker
   * @date 10/03/2024
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
      returnedList.push_back({motorNamesList[i], ": ", motorCartridgeType, ", ", motorType, ", ", to_string(motorList[i].temperature(vex::temperatureUnits::celsius)), "C", ", ", to_string(motorList[i].position(vex::rotationUnits::deg)), "deg"});
    }
    return returnedList;
  }
};

// Initialization of MotorCollection
MotorCollection myMotorCollection;

/**
 * @brief gets the state of the VEX V5 Competition Control as a String
 * @relates drawModeDisplayFrame()
 * @returns GameStates AUTONOMOUS, USER_CONTROL, PRE_AUTONOMOUS, or DISABLED
 * @author Leo Abubucker
 * @date 07/21/2024
 */
GameStates getCompetitionStatus()
{
  if (Competition.isEnabled())
  {
    if (Competition.isAutonomous())
    {
      return AUTONOMOUS;
    }
    else if (Competition.isDriverControl())
    {
      return USER_CONTROL;
    }
    else
    {
      return PRE_AUTONOMOUS;
    }
  }
  else
  {
    return DISABLED;
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
vex::color getColorFromValue(std::string value, Keywords keyword)
{
  if (keyword == GAME_STATE)
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
  else if (keyword == MOTOR_CARTRIDGE)
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
vex::color getColorFromValue(bool value, Keywords keyword)
{
  if (keyword == USER_INPUT)
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
  else if (keyword == MOTOR_CONNECTION)
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
  else if (keyword == CONTROLLER_CONNECTION)
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
 * @date 10/03/2024
 */
vex::color getColorFromValue(int value, Keywords keyword)
{
  if (keyword == MOTOR_TEMPERATURE)
  {
    // Motors will cut their maxium current to 50% at 55 degrees Celsius, to 25% at 60 degrees Celsius,
    // to 12.5% at 65 degrees Celsius, and turn off at 70 degrees celsius
    if (value < 55)
    {
      return color::green;
    }
    else if (value < 70)
    {
      return color::orange;
    }
    else
    {
      return color::red;
    }
  }
  else if (keyword == BATTERY)
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
vex::color getColorFromValue(GameStates value, Keywords keyword)
{
  if (keyword == GAME_STATE)
  {
    if (value == AUTONOMOUS)
    {
      return color::orange;
    }
    else if (value == USER_CONTROL)
    {
      return color::green;
    }
    else if (value == PRE_AUTONOMOUS)
    {
      return color::yellow;
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
  std::vector<std::string> controls = {"Driving - Tank", "Lift Arm - R1", "Lower Arm - R2", "Intake - L1", "Outtake - L2", "Clamp - X", "Toggle - Up"};
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
  Brain.Screen.setPenColor(getColorFromValue(waitingForUserInput, USER_INPUT));
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
  GameStates currentMode = getCompetitionStatus();
  Brain.Screen.setPenColor(getColorFromValue(currentMode, GAME_STATE));
  if (currentMode == DISABLED)
  {
    Brain.Screen.setCursor(11, 11);
  }
  else
  {
    Brain.Screen.setCursor(11, 12);
  }
  Brain.Screen.print(currentMode);
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
    Brain.Screen.setPenColor(getColorFromValue(motorConnections[i], MOTOR_CONNECTION));

    for (int j = 0; j < motorInfo[0].size(); j++)
    {
      switch (j)
      {
      case (2):
        Brain.Screen.setPenColor(getColorFromValue(motorInfo[i][j], MOTOR_CARTRIDGE));
        break;

      case (6):
      case (7):
        int motorTemp = atoi(motorInfo[i][j].c_str());
        Brain.Screen.setPenColor(getColorFromValue(motorTemp, MOTOR_TEMPERATURE));
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
  Brain.Screen.setPenColor(getColorFromValue((int)Brain.Battery.capacity(), BATTERY));
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
  Brain.Screen.setPenColor(getColorFromValue(Controller1.installed(), CONTROLLER_CONNECTION));
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

void drawLogo(){
    // Coordinates of the motor debug frame
  std::map<std::string, int> guiFrame = {
      {"x-left", 405},
      {"x-right", 479},
      {"y-top", 179},
      {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coordinates
    Brain.Screen.drawRectangle(guiFrame["x-left"], guiFrame["y-top"], guiFrame["x-right"] - guiFrame["x-left"], guiFrame["y-bottom"] - guiFrame["y-top"], color(128, 0, 0));
  //   std::string file = "934Logo";
  // bool var = Brain.Screen.drawImageFromFile('934Logo.png',0, 0);
  //   std::cout << var << std::endl;
}

/**
 * @brief Threaded function that calls the six individual drawing functions to draw the entire GUI every 5 seconds
 * @relates autonSelection(), pre_auton(), autonomous(), usercontrol()
 * @author Leo Abubucker
 * @date 09/19/2024
 */
void drawGUI()
{
  while (true)
  {
    drawControlsFrame();
    drawAutonSelectorFrame();
    drawModeDisplayFrame();
    drawMotorDebugFrame();
    drawBatteryInfoFrame();
    drawControllerInfoFrame();
    drawLogo();
    wait(5, vex::timeUnits::sec);
  }
}

/**
 * @brief Non-threaded function that calls the six individual drawing functions to draw the entire GUI
 * @relates autonSelection(), pre_auton(), autonomous(), usercontrol()
 * @author Leo Abubucker
 * @date 09/19/2024
 */
void manuallyDrawGUI()
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
 * @date 10/11/2024
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
        manuallyDrawGUI();
        wait(200, msec);
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
      manuallyDrawGUI();
      wait(200, msec);
    }

    // Checks if the user presses the physical auton confirmation bumper
    if (autonConfirmationBumper.pressing() == 1)
    {
      waitingForUserInput = false;
      manuallyDrawGUI();
    }
    wait(20, msec);
  }
}

/**
 * @brief Thread function that checks the motor statuses every 5 seconds
 * @relates pre_auton
 * @author Leo Abubucker
 * @date 09/19/2024
 */
void motorTracking()
{
  while (true)
  {

    myMotorCollection.checkMotors();
    wait(5, vex::timeUnits::sec);
  }
}

/**
 * @brief Thread function that provides time updates to the controller at set intervals during user control
 * @relates usercontrol()
 * @author Leo Abubucker
 * @date 09/19/2024
 */
void timeTracking()
{
  int timeCheck = 0;
  int time = 0;
  bool isH2H = false;

  while (true)
  {
    if (Competition.isAutonomous() && !isH2H)
    {
      isH2H = true;
    }
    else if (Competition.isDriverControl() && isH2H)
    {
      Controller1.Screen.print("User Control - H2H");
      while (Competition.isDriverControl())
      {
        time++;
        // Time update on controller at 1 minute, 30 seconds, and 10 seconds
        if (time >= 45 && timeCheck == 0)
        {
          Controller1.Screen.clearLine();
          Controller1.Screen.setCursor(0, 0);
          Controller1.rumble(".");
          Controller1.Screen.print("1 Minute Remaining");
          timeCheck++;
        }
        else if (time >= 75 && timeCheck == 1)
        {
          Controller1.Screen.clearLine();
          Controller1.Screen.setCursor(0, 0);
          Controller1.rumble(". .");
          Controller1.Screen.print("30 Seconds Remaining");
          timeCheck++;
        }
        else if (time >= 95 && timeCheck == 2)
        {
          Controller1.Screen.clearLine();
          Controller1.Screen.setCursor(0, 0);
          Controller1.rumble(". . .");
          Controller1.Screen.print("10 Seconds Remaining");
          timeCheck++;
        }
        wait(1, sec);
      }
    }
    else if (Competition.isDriverControl() && !isH2H)
    {
      Controller1.Screen.print("User Control - Skills");
      while (Competition.isDriverControl())
      {
        time++;
        // Time update on controller at 45 seconds, 30 seconds, and 15 seconds
        if (time >= 15 && timeCheck == 0)
        {
          Controller1.Screen.clearLine();
          Controller1.Screen.setCursor(0, 0);
          Controller1.rumble(".");
          Controller1.Screen.print("45 Seconds Remaining");
          timeCheck++;
        }
        else if (time >= 30 && timeCheck == 1)
        {
          Controller1.Screen.clearLine();
          Controller1.Screen.setCursor(0, 0);
          Controller1.rumble(". .");
          Controller1.Screen.print("30 Seconds Remaining");
          timeCheck++;
        }
        else if (time >= 45 && timeCheck == 2)
        {
          Controller1.Screen.clearLine();
          Controller1.Screen.setCursor(0, 0);
          Controller1.rumble(". . .");
          Controller1.Screen.print("15 Seconds Remaining");
          timeCheck++;
        }
        wait(1, sec);
      }
    }
    wait(1, sec);
  }
}

/**
 * @brief Thread function that records and interprets changes in motor positions and prints code that will replicate said movement to console
 * @relates pre_auton()
 * @author Leo Abubucker
 * @date 10/11/2024
 * @bug NONFUNCTIONAL - VEX V5 System can not handle the quantity of output to console and function doesn't properly interpret positions
 */
void autonomousTracking()
{
  const int AXIS_DEVIATION = 5;
  double currentLeftDrivePosition = (leftFront.position(vex::rotationUnits::deg) + leftBack.position(vex::rotationUnits::deg)) / 2.0;
  double currentRightDrivePosition = (rightFront.position(vex::rotationUnits::deg) + rightBack.position(vex::rotationUnits::deg)) / 2.0;
  double currentArmPosition = (rightArm.position(vex::rotationUnits::deg) + rightArm.position(vex::rotationUnits::deg)) / 2.0;
  double currentIntakePosition = (leftIntake.position(vex::rotationUnits::deg) + rightIntake.position(vex::rotationUnits::deg)) / 2.0;
  bool currentClampPosition = clamp.value(); // True (1) is extended, False (0) is retracted

  while (true)
  {

    // If drive motors aren't spinning and there is a difference in either position
    if ((abs(Controller1.Axis3.value()) < AXIS_DEVIATION) && (abs(Controller1.Axis2.value()) < AXIS_DEVIATION) && ((currentLeftDrivePosition != (leftFront.position(vex::rotationUnits::deg) + leftBack.position(vex::rotationUnits::deg)) / 2.0) || (currentRightDrivePosition != (rightFront.position(vex::rotationUnits::deg) + rightBack.position(vex::rotationUnits::deg)) / 2.0)))
    {
      currentLeftDrivePosition = (leftFront.position(vex::rotationUnits::deg) + leftBack.position(vex::rotationUnits::deg)) / 2.0;
      currentRightDrivePosition = (rightFront.position(vex::rotationUnits::deg) + rightBack.position(vex::rotationUnits::deg)) / 2.0;
      std::string lFPrint = "leftFront.spinToPosition(" + to_string(currentLeftDrivePosition) + ", vex::rotationUnits::deg, false);";
      std::string lBPrint = "leftBack.spinToPosition(" + to_string(currentLeftDrivePosition) + ", vex::rotationUnits::deg, false);";
      std::string rFPrint = "rightFront.spinToPosition(" + to_string(currentRightDrivePosition) + ", vex::rotationUnits::deg, false);";
      std::string rBPrint = "rightBack.spinToPosition(" + to_string(currentRightDrivePosition) + ", vex::rotationUnits::deg, true);";
      std::cout << lFPrint << std::endl;
      std::cout << lBPrint << std::endl;
      std::cout << rFPrint << std::endl;
      std::cout << rBPrint << std::endl;

      // Printing to SD Card if Attached
      if (Brain.SDcard.isInserted())
      {
        FILE *autonProg = fopen("autonProg.txt", "a");
        std::string leftDriveStr = "LD" + to_string(currentLeftDrivePosition) + "\n";
        std::string rightDriveStr = "RD" + to_string(currentRightDrivePosition) + "\n";
        fprintf(autonProg, leftDriveStr.c_str());
        fprintf(autonProg, rightDriveStr.c_str());
        fclose(autonProg);
      }
    }

    // Arm Tracking

    // If arm motors aren't spinning and there is a difference in position
    if ((!armMotors.isSpinning()) && (currentArmPosition != (rightArm.position(vex::rotationUnits::deg) + rightArm.position(vex::rotationUnits::deg)) / 2.0))
    {
      currentArmPosition = (rightArm.position(vex::rotationUnits::deg) + rightArm.position(vex::rotationUnits::deg)) / 2.0;
      std::string armPrint = "armMotors.spinTo(" + to_string(currentArmPosition) + ", vex::rotationUnits::deg, false);";
      std::cout << armPrint << std::endl;

      // Printing to SD Card if Attached
      if (Brain.SDcard.isInserted())
      {
        FILE *autonProg = fopen("autonProg.txt", "a");
        std::string armStr = "A" + to_string(currentArmPosition) + "\n";
        fprintf(autonProg, armStr.c_str());
        fclose(autonProg);
      }
    }

    // Intake Tracking

    // If intake motors aren't spinning and there is a difference in position
    if ((!intakeMotors.isSpinning()) && (currentIntakePosition != (leftIntake.position(vex::rotationUnits::deg) + rightIntake.position(vex::rotationUnits::deg)) / 2.0))
    {
      currentIntakePosition = (leftIntake.position(vex::rotationUnits::deg) + rightIntake.position(vex::rotationUnits::deg)) / 2.0;
      std::string intakePrint = "intakeMotors.spinTo(" + to_string(currentIntakePosition) + ", vex::rotationUnits::deg, false);";
      std::cout << intakePrint << std::endl;

      // Printing to SD Card if Attached
      if (Brain.SDcard.isInserted())
      {
        FILE *autonProg = fopen("autonProg.txt", "a");
        std::string intakeStr = "I" + to_string(currentIntakePosition) + "\n";
        fprintf(autonProg, intakeStr.c_str());
        fclose(autonProg);
      }
    }

    // Clamp Tracking

    // If there is a difference in position
    if (currentClampPosition != clamp.value())
    {
      currentClampPosition = clamp.value();

      std::string clampPositionStr = currentClampPosition ? "true" : "false";
      std::string clampPrint = "clamp.set(" + clampPositionStr + ");";
      std::cout << clampPrint << std::endl;

      // Printing to SD Card if Attached
      if (Brain.SDcard.isInserted())
      {
        FILE *autonProg = fopen("autonProg.txt", "a");
        std::string clampStr = "C" + to_string(currentClampPosition) + "\n";
        fprintf(autonProg, clampStr.c_str());
        fclose(autonProg);
      }
    }
    wait(100, msec);
  }
}

/**
 * @brief Function that calibrates inertial sensor
 * @relates pre_auton()
 * @author Leo Abubucker
 * @date 09/22/2024
 */
void calibrateInertial()
{
  if (inertialSensor.installed())
  {
    inertialSensor.calibrate();
    while (inertialSensor.isCalibrating())
    {
      wait(100, msec);
    }
    Controller1.Screen.print("INERTIAL CALIBRATED");
    std::string prn = "INERTIAL CALIBRATED";
    std::cout << prn << std::endl;
  }
}

/**
 * @brief Function that turns a certain direction a certain amount of degrees using the inertial sensor
 * @relates usercontrol()
 * @author Leo Abubucker
 * @date 09/22/2024
 */
void inertialTurn(float degrees, MovementDirections dir)
{
  float actualHeading = inertialSensor.heading(vex::rotationUnits::deg);
  float targetHeading = degrees;
  if(dir == LEFT){
    targetHeading += 180;
  }
  float error = targetHeading - actualHeading;
  float motorSpeed = 0.5 * std::abs(error);
  if(inertialSensor.installed()){
    while(inertialSensor.isCalibrating()){
      wait(100, msec);
    }
  while (std::abs(error) > 1.0)
  {
    actualHeading = inertialSensor.heading(vex::rotationUnits::deg);
    std::string prn1 = "Actual: ";
    std::cout << prn1 << actualHeading << std::endl;
    std::string prn2 = "Target: ";
    std::cout << prn2 << targetHeading << std::endl;
    error = targetHeading - actualHeading;
    motorSpeed = 0.5 * std::abs(error);
    if (dir == RIGHT)
    {
      leftFront.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
    }
    else
    {
      leftFront.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::rev, motorSpeed, vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, motorSpeed, vex::velocityUnits::pct);
    }

    wait(150, msec);
  }
  }
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
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

/**
 * @brief VEX Competition Controlled Function: pre-game initializations, thread initializations, auton selection prompting
 * @relates main()
 * @author Leo Abubucker
 * @date 9/11/2024
 */
void pre_auton()
{
  calibrateInertial();
  // MotorCollection Initialization
  myMotorCollection.addMotor(rightArm, "LA");
  myMotorCollection.addMotor(leftArm, "RA");
  myMotorCollection.addMotor(leftBack, "LB");
  myMotorCollection.addMotor(leftFront, "LF");
  myMotorCollection.addMotor(rightBack, "RB");
  myMotorCollection.addMotor(rightFront, "RF");
  myMotorCollection.addMotor(rightIntake, "RI");
  myMotorCollection.addMotor(leftIntake, "LI");

  // Auton Selection
  autonSelector = 0;

  // Motor Initialization
  allMotors.setMaxTorque(100, vex::percentUnits::pct);
  allMotors.setVelocity(100, vex::percentUnits::pct);
  armMotors.setTimeout(2, vex::timeUnits::sec);
  nonDriveMotors.setStopping(vex::brakeType::hold);
  allMotors.resetPosition();

  // Thread Intialization
  thread guiUpdatingThread = thread(drawGUI);
  thread motorTrackingThread = thread(motorTracking);
  /** thread autonTrackingThread = thread(autonomousTracking); @bug */
  thread timeTrackingThread = thread(timeTracking);
  autonSelection();
  Controller1.Screen.print("PREAUTON FINISHED");
}

/**
 * @brief VEX Competition Controlled Function: 15 seconds of autonomous robot movement
 * @relates main()
 * @authors Leo Abubucker, Jack Deise
 * @date 10/11/2024
 */
void autonomous()
{

  if (autonSelector == 0)
  {
    // armMotors.spinToPosition(440, vex::rotationUnits::deg, true);
    // drive(24, FORWARD, 80);
    // armMotors.spinToPosition(250, vex::rotationUnits::deg, true);
    // intakeMotors.spinToPosition(500, vex::rotationUnits::deg, false);
    // drive(6, REVERSE, 80);
    // drive(6, FORWARD, 80);
    // drive(14, REVERSE, 80);
    // armMotors.spinToPosition(0, vex::rotationUnits::deg, true);
    // turn(60, LEFT, 50);
    // drive(24, FORWARD, 40);

    // inertialTurn
  }
  else if (autonSelector == 1)
  {
    // // Auton skills code
    // drive(12, FORWARD, 50);
    // turn(90, RIGHT, 50);
    // drive(48, FORWARD, 80);
    // turn(45, RIGHT, 50);
    // drive(30, FORWARD, 80);
    // drive(24, REVERSE, 80);

    // turn(45, LEFT, 50);
    // drive(60, REVERSE, 80);
    // turn(30, LEFT, 50);
    // drive(60, REVERSE, 80);
    // turn(5, LEFT, 50);
    // drive(50, FORWARD, 80);
    // armMotors.spinToPosition(970, vex::rotationUnits::deg, true);
    

    drive(12, FORWARD, 50);
    inertialTurn(90, RIGHT);
    drive(20, REVERSE, 80);
    inertialTurn(135, LEFT);
    // drive(50, REVERSE, 80);
    // drive(39, FORWARD, 80);
    // inertialTurn(90, RIGHT);
    // drive(60, FORWARD, 80);
    // inertialTurn(45, RIGHT);
    // drive(50, FORWARD, 80);

// inertialTurn(45, LEFT);

    // turn(135, LEFT, 50);
    // drive(48, FORWARD, 80);
    // turn(45, LEFT, 50);
    // drive(24, FORWARD, 80);
    // armMotors.spinToPosition(480, vex::rotationUnits::deg, true);
    // drive(9, FORWARD, 20);
    // intakeMotors.spinFor(50, vex::rotationUnits::deg, 40, vex::velocityUnits::pct, true);
    // armMotors.spinToPosition(480, vex::rotationUnits::deg, true);
    // drive(1, FORWARD, 20);
    // armMotors.spinToPosition(360, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
    // drive(4, REVERSE, 20);
    // drive(4, FORWARD, 100);
    // drive(10, REVERSE, 20);
    // turn(45, RIGHT, 50);
    // drive(50, FORWARD, 80);
    // turn(20, LEFT, 50);
    // drive(30, RIGHT, 80);
    // turn(20, RIGHT, 50);
    // drive(50, REVERSE, 80);
    // turn(20, RIGHT, 50);
    // drive(30, FORWARD, 80);

  }
  else if (autonSelector == 2)
  {
    // NO AUTON - LEAVE BLANK
  }
}

/**
 * @brief VEX Competition Controlled Function: 1 minute 45 second loop of user-controlled robot movement
 * @relates main()
 * @author Leo Abubucker
 * @date 10/11/2024
 */
void usercontrol()
{
  bool clampState = false;
  bool clampLastState = false;

  // User control code here, inside the loop
  while (1)
  {
    // Drive Controls
    if (driveConfig == REAR_WHEEL)
    {
      leftBack.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }
    else if (driveConfig == FRONT_WHEEL)
    {
      leftFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      rightFront.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }
    else if (driveConfig == RFLB)
    {
      rightFront.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
      leftBack.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
    }
    else if (driveConfig == LFRB)
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

    // Clamp Controls
    if (Controller1.ButtonX.pressing() && !clampLastState)
    {
      clampState = !clampState;
      clampLastState = true;
    }
    else if (!Controller1.ButtonX.pressing())
    {
      clampLastState = false;
    }
    if (clampState)
    {
      clamp.set(true);
    }
    else
    {
      clamp.set(false);
    }

    // Intake Controls
    if (Controller1.ButtonL1.pressing())
    {
      intakeMotors.spin(vex::directionType::rev);
    }
    else if (Controller1.ButtonL2.pressing())
    {
      intakeMotors.spin(vex::directionType::fwd);
    }
    else
    {
      intakeMotors.stop();
    }

    // Arm Controls
    if (Controller1.ButtonUp.pressing())
    {
      // Positition to go to the top of a neutral wall stake
      armMotors.spinToPosition(720, vex::rotationUnits::deg, true);
    }
    else if (Controller1.ButtonDown.pressing())
    {
      armMotors.spinToPosition(0, vex::rotationUnits::deg, true);
    }
    else if (Controller1.ButtonR1.pressing())
    {
      armMotors.spin(vex::directionType::fwd);
    }
    else if (Controller1.ButtonR2.pressing())
    {
      armMotors.spin(vex::directionType::rev);
    }
    else if (Controller1.ButtonB.pressing())
    {
      // Position to go to the top of an alliance wall stake
      armMotors.spinToPosition(600, vex::rotationUnits::deg, true);
    }
    else if (Controller1.ButtonA.pressing())
    {
      // Position to go to the top ring of a fully filled mobile goal stake
      armMotors.spinToPosition(320, vex::rotationUnits::deg, true);
    }
    else if (Controller1.ButtonLeft.pressing())
    {
      // Position to go to climb position
      armMotors.spinToPosition(970, vex::rotationUnits::deg, true);
    }
    else if (Controller1.ButtonRight.pressing())
    {
      // Position to go to climb position
      armMotors.spinToPosition(970, vex::rotationUnits::deg, true);
    }
    else
    {
      armMotors.stop();
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