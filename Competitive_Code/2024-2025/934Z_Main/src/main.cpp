#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>


/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                      GLOBAL DECLARATIONS AND INITIALIZATIONS */
/*  Declarations and initializations of PROS, LEMLIB, and OTHER global variables
 */
/*  PROS Initializations: */
/*  - pros::Controller controller - represents the VEX v5 Controller constructed
 * as   */
/*      the "primary" or the "partner" controller */
/*  - pros::Motor motorName - represents a VEX V5 Motor constructed with a port
 * from  */
/*      1-21 with negatives representing reversed motors and either a red or
 * green    */
/*      gearset. */
/*  - pros::MotorGroup motorGroupName - represents a group of motors constructed
 * with */
/*      the motors in the group. primarily used for mass initializations of
 * motor     */
/*      attributes. */
/*  - pros::Imu imuName - represents a VEX V5 Inertial Sensor */
/*  - pros::adi::DigitalOut digitalOutDevice - represents a 3-wire Digital Out
 * Device */
/*  - pros::adi::DigitalIn digitalInDevice - represents a 3-wire Digital In
 * Device    */
/*  - bumper bumperName - represents a VEX bumper constructed with a specified
 */
/*      triport port. */
/*  LEMLIB Initializations: */
/*  - lemlib::ControllerSettings linearController - lateral motion controller */
/*  - lemlib::ControllerSettings angularController - angular motion controller
 */
/*  - lemlib::OdomSensors sensors - odometry sensors */
/*  - lemlib::ExpoDriveCurve throttleCurve - input throttle curve for user
 * control    */
/*  - lemlib::ExpoDriveCurve steerCurve - input steer curve for user control */
/*  - lemlib::Chassis chassis - chassis */
/*  OTHER Declarations: */
/*  - int autonSelector - integer representing the autonomous program that the
 * user   */
/*      selects to run. */
/*  - std::map<std::string, int> autonSelectorFrame - map representing the */
/*      coordinates of the autonomous selector GUI button on the brain. */
/*  - enum DriveConfigurations{} - enums representing the 4 drive configs */
/*  - DriveConfigurations driveConfig - global instance of DriveConfigurations
 */
/*  - enum Keywords{} - enums representing the 7 keywords */
/*  - enum GameStates{} - enums representing the 5 game states */
/*  - enum MovementDirections{} - enums representing the 4 movement directions
 */
/*  OTHER Initializations: */
/*  - bool waitingForUserInput = false - boolean representing whether the user
 * has    */
/*      provided input confirming the autonomous program to be run. */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

// Paths
ASSET(initPath_txt);
// Controllers
pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Controller secondaryController(pros::E_CONTROLLER_PARTNER);

// Motors
pros::Motor rightArm(1, pros::MotorGearset::red);
pros::Motor leftArm(-9, pros::MotorGearset::red);
pros::Motor leftBack(-11, pros::MotorGearset::blue);
pros::Motor leftFront(-15, pros::MotorGearset::blue);
pros::Motor rightBack(13, pros::MotorGearset::blue);
pros::Motor rightFront(15, pros::MotorGearset::blue);
pros::Motor leftIntake(-10, pros::MotorGearset::green);
pros::Motor rightIntake(2, pros::MotorGearset::green);

// Motor Groups
pros::MotorGroup allMotors({-15, -11, 14, 13, -10, 2, -9, 1});

pros::MotorGroup
    leftMotors({-15, -11},
               pros::MotorGearset::blue); // left motor group - ports 3
                                          // (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors(
    {14, 13},
    pros::MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)

pros::MotorGroup intakeMotors({-10, 2}, pros::MotorGearset::green);

pros::MotorGroup armMotors({-9, 1}, pros::MotorGearset::red);

// Inertial Sensor
pros::Imu imu(20);

// Piston
pros::adi::DigitalOut clamp(1);

// Auton Selection Bumper
pros::adi::DigitalIn autonSelectionBumper(6);

// Auton Confirmation Bumper
pros::adi::DigitalIn autonConfirmationBumper(3);

// Drivetrain Settings
lemlib::Drivetrain drivetrain(
    &leftMotors,                // left motor group
    &rightMotors,               // right motor group
    9.2,                        // 9.2 inch track width
    lemlib::Omniwheel::OLD_275, // using old 2.75" omnis
    450,                        // drivetrain rpm is 333.33
    2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// Lateral Motion Controller
lemlib::ControllerSettings
    linearController(10,  // proportional gain (kP)
                     0,   // integral gain (kI)
                     3,   // derivative gain (kD)
                     3,   // anti windup
                     1,   // small error range, in inches
                     100, // small error range timeout, in milliseconds
                     3,   // large error range, in inches
                     500, // large error range timeout, in milliseconds
                     20   // maximum acceleration (slew)
    );

// Angular Motion Controller
lemlib::ControllerSettings
    angularController(2,   // proportional gain (kP)
                      0,   // integral gain (kI)
                      10,  // derivative gain (kD)
                      3,   // anti windup
                      1,   // small error range, in degrees
                      100, // small error range timeout, in milliseconds
                      3,   // large error range, in degrees
                      500, // large error range timeout, in milliseconds
                      0    // maximum acceleration (slew)
    );

// Sensors for Odometry
lemlib::OdomSensors sensors(
    nullptr, // vertical tracking wheel,  set to nullptr as we don't have one
    nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a
             // second one
    nullptr, // horizontal tracking wheel,  set to nullptr as we don't have one
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a
             // second one
    &imu     // inertial sensor
);

// Input Curve for Throttle Input During Driver Control
lemlib::ExpoDriveCurve
    throttleCurve(3,    // joystick deadband out of 127
                  10,   // minimum output where drivetrain will move out of 127
                  1.019 // expo curve gain
    );

// Input Curve for Steer Input During Driver Control
lemlib::ExpoDriveCurve
    steerCurve(3,    // joystick deadband out of 127
               10,   // minimum output where drivetrain will move out of 127
               1.019 // expo curve gain
    );

// Create the Chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController,
                        sensors, &throttleCurve, &steerCurve);

// User Input Bool
bool waitingForUserInput = false;

// Auton Selector
int autonSelector;
std::map<std::string, int> autonSelectorFrame;

enum DriveConfigurations { FOUR_WHEEL, REAR_WHEEL, FRONT_WHEEL, RFLB, LFRB };
DriveConfigurations driveConfig;
enum Keywords {
  GAME_STATE,
  MOTOR_CARTRIDGE,
  USER_INPUT,
  MOTOR_CONNECTION,
  CONTROLLER_BATTERY,
  MOTOR_TEMPERATURE,
  BATTERY,
  CONTROLLER_CONNECTION,
  MOTOR_TYPE
};
enum GameStates { PRE_AUTONOMOUS, AUTONOMOUS, USER_CONTROL, DISABLED, ENABLED };

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       HELPER FUNCTIONS AND CLASSES */
/*                                                                                    */
/*  Helper functions and classes that assist VEX competition controlled
 * functions.    */
/*  Includes: */
/*  - std::string to_string(T value) - To string template function */
/*  - Motor Collection Class - Provides additional functionality for VEX V5
 * motors    */
/*  - GameStates getCompetitionStatus() - gets the current competition state */
/*  - pros::Color getColorFromValue(std::string value, Keywords keyword) -
 * returns    */
/*    a color based on the given value and keyword */
/*  - pros::Color getColorFromValue(bool value, Keywords keyword) - returns */
/*    a color based on the given value and keyword */
/*  - pros::Color getColorFromValue(int value, Keywords keyword) - returns */
/*    a color based on the given value and keyword */
/*  - pros::Color getColorFromValue(GameStates value, Keywords keyword) -
 * returns     */
/*    a color based on the given value and keyword */
/*	- void drawDashedLine(int x0, int y0, int x1, int y1, int dashLength,
 * pros::Color */
/*	  dashColor) - draws a dashed line given the coordinates, length, and
 * color       */
/*  - void drawControlsFrame() - draws the control frame */
/*  - void drawAutonSelectorFrame() - draws the autononomous selection frame */
/*  - void drawModeDisplayFrame() - draws the mode display frame */
/*  - void drawMotorDebugFrame() - draws the motor debug frame */
/*  - void drawBatteryInfoFrame() - draws the battery info frame */
/*  - void drawControllerInfoFrame() - draws the control frame */
/*  - void drawTeamFrame() - draws the team frame */
/*  - void drawGUI() - thread function that calls the seven draw functions above
 */
/*	- void manuallyDrawGUI() - non-threaded version of drawGUI() */
/*  - void autonSelection() - Autonomous program selection function */
/*  - void motorTracking() - thread function that routinely check status of
 * motors    */
/*  - void timeTracking() - thread function that alerts controller w/ time
 * updates    */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

/**
 * @brief Converts a value to a string in a method that allows printing to the
 * VEX V5 Brain and/or Controller
 * @tparam T
 * @param value value to be converted to a std::string
 * @returns value converted to a std::string
 * @author James Pearman
 * @cite
 * https://www.vexforum.com/t/std-tostring-not-working-in-vc-and-vcs/62962/7
 */
template <typename T> std::string to_string(T value) {
  std::ostringstream os;
  os << value;
  return os.str();
}

/**
 * @brief MotorCollection class provides additional functionality to VEX V5
 * motors
 * @details This class provides additional functionality in detecting the
 * connection status of motors in the collection, dynamically updating the drive
 * configuration based on the connection status of the drive motors, and returns
 * attributes of the motors.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
class MotorCollection {
public:
  // Init vector of motor objects
  std::vector<pros::Motor> motorList;
  // Init vector of std::strings representing motor names
  std::vector<std::string> motorNamesList;

  /**
   * @brief adds the passed in motor object and std::string object motorName to
   * respective lists
   * @relates pre_auton()
   * @param newMotor VEX V5 motor
   * @param newMotorName std::string name of newMotor
   * @author Leo Abubucker
   * @date 12/01/2024
   */
  void addMotor(pros::Motor newMotor, std::string newMotorName) {
    motorList.push_back(newMotor);
    motorNamesList.push_back(newMotorName);
  }

  /**
   * @brief returns motor connection values in a vector; prints disconnections
   * to Controller
   * @details creates an std::vector containing booleans representing the
   * connection status of the motors. Prints the names of any disconnected
   * motors to the Controller screen.
   * @relates checkMotors()
   * @returns std::vector containing booleans representing connections statuses
   * of the motors
   * @author Leo Abubucker
   * @date 12/16/2024
   */
  std::vector<bool> isConnected() {

    std::vector<bool> motorConnections;
    bool isConnected;
    for (int i = 0; i < motorList.size(); i++) {
      if (motorList[i].get_gearing() != pros::MotorGears::red &&
          motorList[i].get_gearing() != pros::MotorGears::green &&
          motorList[i].get_gearing() != pros::MotorGears::blue) {
        isConnected = false;
      } else {
        isConnected = true;
      }
      motorConnections.push_back(isConnected);
      // if (!motorList[i].is_installed())
      // {
      // 	controller.clear_line(0);
      // 	controller.set_text(0, 0, motorNamesList[i].c_str());
      // }
    }
    return motorConnections;
  }

  /**
   * @brief determines the optimal drive configuration based on the connection
   * of drive motors
   * @relates usercontrol()
   * @author Leo Abubucker
   * @date 06/14/2024
   */
  void checkMotors() {
    std::vector<bool> motorConnections = isConnected();
    std::vector<std::string> disconnectedMotorNames;
    driveConfig = FOUR_WHEEL;
    for (int i = 0; i < motorConnections.size(); i++) {
      if (motorConnections[i] == false) {
        disconnectedMotorNames.push_back(motorNamesList[i]);
      }
    }
    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "LF") != disconnectedMotorNames.end() ||
        std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "RF") != disconnectedMotorNames.end()) {
      driveConfig = REAR_WHEEL;
    }

    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "LB") != disconnectedMotorNames.end() ||
        std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "RB") != disconnectedMotorNames.end()) {
      driveConfig = FRONT_WHEEL;
    }

    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "LF") != disconnectedMotorNames.end() &&
        std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "RB") != disconnectedMotorNames.end()) {
      driveConfig = RFLB;
    }
    if (std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "LB") != disconnectedMotorNames.end() ||
        std::find(disconnectedMotorNames.begin(), disconnectedMotorNames.end(),
                  "RF") != disconnectedMotorNames.end()) {
      driveConfig = LFRB;
    }
  }

  /**
   * @brief retrieves motor attributes and returns them in a formatted 2D
   * std::string
   * @relates drawMotorDebugFrame()
   * @returns 2D vector where each value is [name: cartridge, type,
   * temperatureC, positiondeg]
   * @author Leo Abubucker
   * @date 12/01/2024
   */
  std::vector<std::vector<std::string>> returnValues() {
    std::vector<std::vector<std::string>> returnedList;
    std::string motorCartridgeType;
    std::string motorType;
    std::string motorTemp;
    std::string motorPos;

    for (int i = 0; i < motorList.size(); i++) {
      if (motorList[i].get_gearing() == pros::MotorGears::red) {
        motorCartridgeType = "Red";
      } else if (motorList[i].get_gearing() == pros::MotorGears::green) {
        motorCartridgeType = "Green";
      } else if (motorList[i].get_gearing() == pros::MotorGears::blue) {
        motorCartridgeType = "Blue";
      } else {
        motorCartridgeType = "ERR";
      }
      if (motorCartridgeType == "ERR") {
        motorType = "ERR";
      } else {
        motorType = "11W";
      }
      if (motorList[i].get_temperature() > 0 &&
          motorList[i].get_temperature() < 200) {
        motorTemp = to_string(motorList[i].get_temperature()) + "C";
      } else {
        motorTemp = "ERR";
      }

      if (std::abs(motorList[i].get_position()) < 999999) {
        motorPos =
            to_string(round(motorList[i].get_position() * 100.00) / 100.00) +
            "deg";
      } else {
        motorPos = "ERR";
      }
      returnedList.push_back({motorNamesList[i] + ": ",
                              motorCartridgeType + ", ", motorType + ", ",
                              motorTemp + ", ", motorPos});
    }
    return returnedList;
  }

  /**
   * @brief returns the list of motors
   * @relates drawMotorDebugFrame(), drawControllerDisplay()
   * @author Leo Abubucker
   * @date 12/01/2024
   */
  std::vector<pros::Motor> returnMotors() { return motorList; }

  /**
   * @brief returns the list of motor names
   * @relates drawControllerDisplay()
   * @author Leo Abubucker
   * @date 12/16/2024
   */
  std::vector<std::string> returnMotorNames() { return motorNamesList; }
};

// Initialization of MotorCollection
MotorCollection myMotorCollection;

/**
 * @brief gets the state of the VEX V5 Competition Control as a String
 * @relates drawModeDisplayFrame()
 * @returns GameStates AUTONOMOUS, USER_CONTROL, PRE_AUTONOMOUS, or DISABLED
 * @author Leo Abubucker
 * @date 12/01/2024
 */
GameStates getCompetitionStatus() {
  if (!pros::competition::is_connected()) {
    return DISABLED;
  } else if (pros::competition::is_disabled()) {
    return PRE_AUTONOMOUS;
  } else if (pros::competition::is_autonomous()) {
    return AUTONOMOUS;
  }
  return USER_CONTROL;
}

/**
 * @brief gets a pros::Color based on an std::string value and Keyword keyword
 * @details This function takes a Keyword keyword and std::string value and
 * returns a pros::Color based on specific condition(s) that the value meets.
 * The conditions that are used are based on the keyword.
 * @relates drawModeDisplayFrame(), drawMotorDebugFrame()
 * @overload getColorFromValue(bool value, Keyword keyword)
 * @overload getColorFromVaue(int value, Keyword keyword)
 * @overload getColorFromValue(GameStates value, Keyword keyword)
 * @param value std::string value that determines color
 * @param keyword Keyword keyword that determines which conditions value is
 * checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no
 * value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(std::string value, Keywords keyword) {
  if (keyword == GAME_STATE) {
    if (value == "AUTON") {
      return pros::Color::orange;
    } else if (value == "USER") {
      return pros::Color::light_green;
    } else if (value == "PRE-AUTON") {
      return pros::Color::yellow;
    } else {
      return pros::Color::red;
    }
  } else if (keyword == MOTOR_CARTRIDGE) {
    if (value == "Green") {
      return pros::Color::light_green;
    } else if (value == "Red") {
      return pros::Color::red;
    } else if (value == "Blue") {
      return pros::Color::blue;
    } else {
      return pros::Color::red;
    }
  } else if (keyword == MOTOR_TYPE) {
    if (value == "11W, ") {
      return pros::Color::white;
    } else {
      return pros::Color::red;
    }
  }
  return pros::Color::white;
}

/**
 * @brief gets a pros::Color based on a bool value and Keyword keyword
 * @details This function takes a Keyword keyword and bool value and returns a
 * pros::Color based on specific condition(s) that the value meets. The
 * conditions that are used are based on the keyword.
 * @relates drawAutonSelectorFrame(), drawMotorDebugFrame(),
 * drawControllerInfoFrame()
 * @overload getColorFromValue(std::string value, Keyword keyword)
 * @overload getColorFromVaue(int value, Keyword keyword)
 * @overload getColorFromValue(GameStates value, Keyword keyword)
 * @param value bool value that determines color
 * @param keyword Keyword keyword that determines which conditions value is
 * checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no
 * value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(bool value, Keywords keyword) {
  if (keyword == USER_INPUT) {
    if (value) {
      return pros::Color::orange;
    } else {
      return pros::Color::light_green;
    }
  } else if (keyword == MOTOR_CONNECTION) {
    if (value) {
      return pros::Color::light_green;
    } else {
      return pros::Color::red;
    }
  } else if (keyword == CONTROLLER_CONNECTION) {
    if (value) {
      return pros::Color::light_green;
    } else {
      return pros::Color::red;
    }
  }
  return pros::Color::white;
}

/**
 * @brief gets a pros::Color based on an int value and Keyword keyword
 * @details This function takes an Keyword keyword and int value and returns a
 * pros::Color based on specific condition(s) that the value meets. The
 * conditions that are used are based on the keyword.
 * @relates drawMotorDebugFrame(), drawBatteryInfoFrame()
 * @overload getColorFromValue(std::string value, Keyword keyword)
 * @overload getColorFromVaue(bool value, Keyword keyword)
 * @overload getColorFromValue(GameStates value, Keyword keyword)
 * @param value int value that determines color
 * @param keyword Keyword keyword that determines which conditions value is
 * checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no
 * value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(int value, Keywords keyword) {
  if (keyword == MOTOR_TEMPERATURE) {
    // Motors will cut their maxium current to 50% at 55 degrees Celsius, to 25%
    // at 60 degrees Celsius, to 12.5% at 65 degrees Celsius, and turn off at 70
    // degrees celsius
    if (value < 55 && value > 0) {
      return pros::Color::light_green;
    } else if (value < 70 && value > 0) {
      return pros::Color::orange;
    } else {
      return pros::Color::red;
    }
  } else if (keyword == BATTERY || keyword == CONTROLLER_BATTERY) {
    if (value > 80) {
      return pros::Color::light_green;
    } else if (value > 50) {
      return pros::Color::orange;
    } else {
      return pros::Color::red;
    }
  }
  return pros::Color::white;
}

/**
 * @brief gets a pros::Color based on an int value and Keyword keyword
 * @details This function takes an Keyword keyword and GameStates value and
 * returns a pros::Color based on specific condition(s) that the value meets.
 * The conditions that are used are based on the keyword.
 * @relates drawMotorDebugFrame(), drawBatteryInfoFrame()
 * @overload getColorFromValue(std::string value, Keyword keyword)
 * @overload getColorFromVaue(bool value, Keyword keyword)
 * @overload getColorFromValue(int value, Keyword keyword)
 * @param value int value that determines color
 * @param keyword Keyword keyword that determines which conditions value is
 * checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no
 * value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(GameStates value, Keywords keyword) {
  if (keyword == GAME_STATE) {
    if (value == AUTONOMOUS) {
      return pros::Color::orange;
    } else if (value == USER_CONTROL) {
      return pros::Color::light_green;
    } else if (value == PRE_AUTONOMOUS) {
      return pros::Color::yellow;
    } else {
      return pros::Color::red;
    }
  }

  return pros::Color::white;
}

/**
 * @brief draws a dashed line of a given color from a given start point to a
 * given end point where each dash is a given length
 * @relates drawMotorDebugFrame()
 * @param x0 int x-coord of starting point
 * @param y0 int y-coord of starting point
 * @param x1 int x-coord of ending point
 * @param y1 int y-coord of ending point
 * @param dashLength int length of individual dashes in pixels
 * @param dashColor pros::Color of dashes
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawDashedLine(int x0, int y0, int x1, int y1, int dashLength,
                    pros::Color dashColor) {
  int distance = std::sqrt(std::pow(std::abs(x1 - x0), 2) +
                           std::pow(std::abs(y1 - y0), 2));

  int numDashes = (distance / 2) / dashLength;
  bool isVertical = true;
  if (std::abs(y1 - y0) == 0) {
    isVertical = false;
  }
  int dashX0 = x0;
  int dashX1;
  int dashY0 = y0;
  int dashY1;
  if (isVertical) {
    dashX1 = dashX0;
    dashY1 = dashY0 + dashLength;
  } else {
    dashY1 = dashY0;
    dashX1 = dashX0 + dashLength;
  }
  pros::screen::set_pen(dashColor);
  for (int i = 0; i < numDashes; i++) {
    pros::screen::draw_line(dashX0, dashY0, dashX1, dashY1);
    if (isVertical) {
      dashY0 = dashY1 + dashLength;
      dashY1 = dashY0 + dashLength;
    } else {
      dashX0 = dashX1 + dashLength;
      dashX1 = dashX0 + dashLength;
    }
  }
}

/**
 * @brief draws the GUI controls frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawControlsFrame() {
  // Coordinates of the control frame
  std::map<std::string, int> controlsFrame = {
      {"x-left", 0}, {"x-right", 182}, {"y-top", 0}, {"y-bottom", 190}};

  // Draws maroon rectangle at above coordinates
  pros::screen::set_pen(pros::Color::maroon);
  pros::screen::fill_rect(controlsFrame["x-left"], controlsFrame["y-top"],
                          controlsFrame["x-right"], controlsFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::draw_rect(controlsFrame["x-left"], controlsFrame["y-top"],
                          controlsFrame["x-right"], controlsFrame["y-bottom"]);

  // Prints Controls in the frame
  std::vector<std::string> controls = {
      "Driving - Tank", "Lift Arm - R1", "Lower Arm - R2", "Intake - L1",
      "Outtake - L2",   "Clamp - X",     "Toggle - Up"};
  pros::screen::set_pen(pros::Color::white);
  pros::screen::print(pros::E_TEXT_MEDIUM, 50, 10, "Controls");
  int y = 30;
  for (int i = 0; i < controls.size(); i++) {
    pros::screen::print(pros::E_TEXT_SMALL, 20, y, controls[i].c_str());
    y += 20;
  }
}

/**
 * @brief draws the GUI autonomous selector frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawAutonSelectorFrame() {
  // Coordinates of the autonomous selector frame
  autonSelectorFrame = {
      {"x-left", 0}, {"x-right", 99}, {"y-top", 189}, {"y-bottom", 240}};

  std::vector<std::string> autonNames= {"RRQ", "BRQ", "RGQ", "BGQ", "RRE", "BRE", "RGE", "BGE", "S", "NA"};
  // Draws maroon rectangle at above coordinates 
  pros::screen::set_pen(pros::Color::maroon);
  pros::screen::fill_rect(
      autonSelectorFrame["x-left"], autonSelectorFrame["y-top"],
      autonSelectorFrame["x-right"], autonSelectorFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::draw_rect(
      autonSelectorFrame["x-left"], autonSelectorFrame["y-top"],
      autonSelectorFrame["x-right"], autonSelectorFrame["y-bottom"]);

  // Gets and prints the color-coordinated current autonomous selection and
  // whether the selection is locked or not in the frame
  pros::screen::set_pen(pros::Color::white);
  pros::screen::print(pros::E_TEXT_MEDIUM, 10, 200, "Auton: ");
  pros::screen::print(pros::E_TEXT_MEDIUM, 68, 200, to_string(autonNames[autonSelector]).c_str());
  pros::screen::set_pen(getColorFromValue(waitingForUserInput, USER_INPUT));

  if (waitingForUserInput) {
    pros::screen::print(pros::E_TEXT_MEDIUM, 10, 220, "UNLOCKED");
  } else {
    pros::screen::print(pros::E_TEXT_MEDIUM, 20, 220, "LOCKED");
  }

  pros::screen::set_pen(pros::Color::white);
}

/**
 * @brief draws the GUI mode display frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawModeDisplayFrame() {
  // Coordinates of the mode display frame
  std::map<std::string, int> modeDisplayFrame = {
      {"x-left", 98}, {"x-right", 182}, {"y-top", 189}, {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coordinates
  pros::screen::set_pen(pros::Color::maroon);
  pros::screen::fill_rect(modeDisplayFrame["x-left"], modeDisplayFrame["y-top"],
                          modeDisplayFrame["x-right"],
                          modeDisplayFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::draw_rect(modeDisplayFrame["x-left"], modeDisplayFrame["y-top"],
                          modeDisplayFrame["x-right"],
                          modeDisplayFrame["y-bottom"]);

  // Gets and prints the color-coordinated current game state in the frame
  pros::screen::set_pen(pros::Color::white);
  pros::screen::print(pros::E_TEXT_MEDIUM, 110, 200, "Mode: ");

  GameStates currentMode = getCompetitionStatus();
  pros::screen::print(pros::E_TEXT_MEDIUM, 160, 200,
                      to_string(currentMode).c_str());
  if (currentMode == 0) {
    pros::screen::set_pen(
        getColorFromValue(to_string("PRE-AUTON"), GAME_STATE));
    pros::screen::print(pros::E_TEXT_MEDIUM, 100, 220, "PREAUTON");
  } else if (currentMode == 1) {
    pros::screen::set_pen(getColorFromValue(to_string("AUTON"), GAME_STATE));
    pros::screen::print(pros::E_TEXT_MEDIUM, 110, 220, "AUTON");
  } else if (currentMode == 2) {
    pros::screen::set_pen(getColorFromValue(to_string("USER"), GAME_STATE));
    pros::screen::print(pros::E_TEXT_MEDIUM, 110, 220, "USER");
  } else if (currentMode == 3) {
    pros::screen::set_pen(getColorFromValue(to_string("DISABLED"), GAME_STATE));
    pros::screen::print(pros::E_TEXT_MEDIUM, 100, 220, "DISABLED");
  }

  pros::screen::set_pen(pros::Color::white);
}

/**
 * @brief draws the GUI motor debug frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/16/2024
 */
void drawMotorDebugFrame() {
  // Coordinates of the motor debug frame
  std::map<std::string, int> motorDebugFrame = {
      {"x-left", 181}, {"x-right", 479}, {"y-top", 0}, {"y-bottom", 190}};

  // Draws a maroon rectangle at the above coordinates
  pros::screen::set_pen(pros::Color::maroon);
  pros::screen::fill_rect(motorDebugFrame["x-left"], motorDebugFrame["y-top"],
                          motorDebugFrame["x-right"],
                          motorDebugFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::draw_rect(motorDebugFrame["x-left"], motorDebugFrame["y-top"],
                          motorDebugFrame["x-right"],
                          motorDebugFrame["y-bottom"]);

  // Gets and prints color-coordinated debug information for all the motors in
  // the frame
  pros::screen::set_pen(pros::Color::white);
  pros::screen::print(pros::E_TEXT_MEDIUM, 250, 10, "Motor Information");
  std::vector<std::vector<std::string>> motorInfo =
      myMotorCollection.returnValues();
  std::vector<pros::Motor> motors = myMotorCollection.returnMotors();
  std::vector<bool> motorConnections = myMotorCollection.isConnected();

  int x;
  int y = 30;

  for (int i = 0; i < motorInfo.size(); i++) {
    x = 200;
    drawDashedLine(0, y - 4, 479, y - 4, 5, pros::Color::white);

    std::string motorCartridgeType;
    bool motorIsInstalled;
    int motorTemp;
    for (int j = 0; j < motorInfo[0].size(); j++) {
      switch (j) {
      case (0):

        if (motorConnections[i]) {
          pros::screen::set_pen(pros::Color::light_green);
        } else {
          pros::screen::set_pen(pros::Color::red);
        }

        // pros::screen::set_pen(getColorFromValue(motorConnections[i],
        // MOTOR_CONNECTION));
        break;
        // pros::screen::set_pen(pros::Color::light_green);

      case (1):

        if (motors[i].get_gearing() == pros::MotorGears::red) {
          motorCartridgeType = "Red";
        } else if (motors[i].get_gearing() == pros::MotorGears::green) {
          motorCartridgeType = "Green";
        } else if (motors[i].get_gearing() == pros::MotorGears::blue) {
          motorCartridgeType = "Blue";
        } else {
          motorCartridgeType = "ERR";
        }
        pros::screen::set_pen(
            getColorFromValue(motorCartridgeType, MOTOR_CARTRIDGE));
        break;

      case (2):
        pros::screen::set_pen(
            getColorFromValue(to_string(motorInfo[i][j].c_str()), MOTOR_TYPE));
        break;
      case (3):
        motorTemp = atoi(to_string(motors[i].get_temperature()).c_str());
        pros::screen::set_pen(getColorFromValue(motorTemp, MOTOR_TEMPERATURE));
        break;

      case (4):
        if (motorInfo[i][j].find("ERR") != -1) {
          pros::screen::set_pen(pros::Color::red);
        }
        break;
      }
      pros::screen::print(pros::E_TEXT_SMALL, x, y, motorInfo[i][j].c_str());
      x += 50;
      pros::screen::set_pen(pros::Color::white);
    }
    y += 20;
  }
}

/**
 * @brief draws the GUI battery info frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawBatteryInfoFrame() {
  // Coordinates of the battery info frame
  std::map<std::string, int> batteryInfoFrame = {
      {"x-left", 181}, {"x-right", 276}, {"y-top", 189}, {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coordinates
  pros::screen::set_pen(pros::Color::maroon);
  pros::screen::fill_rect(batteryInfoFrame["x-left"], batteryInfoFrame["y-top"],
                          batteryInfoFrame["x-right"],
                          batteryInfoFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::draw_rect(batteryInfoFrame["x-left"], batteryInfoFrame["y-top"],
                          batteryInfoFrame["x-right"],
                          batteryInfoFrame["y-bottom"]);

  // Gets and prints the color-coordinated battery percent in the frame
  pros::screen::set_pen(pros::Color::white);
  pros::screen::print(pros::E_TEXT_MEDIUM, 200, 200, "Battery");
  pros::screen::set_pen(
      getColorFromValue((int)pros::battery::get_capacity(), BATTERY));
  pros::screen::print(pros::E_TEXT_MEDIUM, 210, 220, "%.2f%",
                      pros::battery::get_capacity());
  pros::screen::set_pen(pros::Color::white);
}

/**
 * @brief draws the GUI controller info frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawControllerInfoFrame() {
  // Coordinates of the controller info frame
  std::map<std::string, int> controllerInfoFrame = {
      {"x-left", 275}, {"x-right", 410}, {"y-top", 189}, {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coords
  pros::screen::set_pen(pros::Color::maroon);
  pros::screen::fill_rect(
      controllerInfoFrame["x-left"], controllerInfoFrame["y-top"],
      controllerInfoFrame["x-right"], controllerInfoFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::draw_rect(
      controllerInfoFrame["x-left"], controllerInfoFrame["y-top"],
      controllerInfoFrame["x-right"], controllerInfoFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::print(pros::E_TEXT_MEDIUM, 300, 200, "Controller");

  // Gets and prints the color-coordinated controller connection in the frame
  pros::screen::set_pen(getColorFromValue((bool)controller.is_connected(),
                                          CONTROLLER_CONNECTION));
  if (controller.is_connected()) {
    pros::screen::print(pros::E_TEXT_MEDIUM, 300, 220, "CONNECTED");
  } else {
    pros::screen::print(pros::E_TEXT_MEDIUM, 298, 220, "DISCONNECTED");
  }

  pros::screen::set_pen(pros::Color::white);
}

/**
 * @brief draws the GUI team info frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawTeamFrame() {
  // Coordinates of the controller info frame
  std::map<std::string, int> teamFrame = {
      {"x-left", 409}, {"x-right", 479}, {"y-top", 189}, {"y-bottom", 240}};

  // Draws a maroon rectangle at the above coords
  pros::screen::set_pen(pros::Color::maroon);
  pros::screen::fill_rect(teamFrame["x-left"], teamFrame["y-top"],
                          teamFrame["x-right"], teamFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::draw_rect(teamFrame["x-left"], teamFrame["y-top"],
                          teamFrame["x-right"], teamFrame["y-bottom"]);
  pros::screen::set_pen(pros::Color::white);
  pros::screen::print(pros::E_TEXT_MEDIUM, 420, 200, "Team");
  pros::screen::print(pros::E_TEXT_MEDIUM, 420, 220, "934Z");
}

/**
 * @brief threaded function that draws the Controller Display every second
 * @relates initialize()
 * @author Leo Abubucker
 * @date 1/6/2025
 */
void drawControllerDisplay() {
  // controller.clear();
  secondaryController.clear();
  int seconds = 0;
  int minutes = 0;
  float time = 00.00;
  int modeIter = 0;
  int rumbleCount = 0;
  std::vector<pros::Motor> motors = myMotorCollection.returnMotors();
  std::vector<std::string> motorNames = myMotorCollection.returnMotorNames();
  while (true) {
    // Determines and prints disconnected motors

    std::vector<bool> motorConnections = myMotorCollection.isConnected();

    std::string dcMotors = "DC: ";
    for (int i = 0; i < motors.size(); i++) {
      if (!motorConnections[i]) {
        std::string prn = motorNames[i] + " ";
        dcMotors += prn;
      }
    }
    controller.print(1, 0, dcMotors.c_str());
    pros::delay(100);

    // Determines and prints hot motors

    std::string hotMotors = "HOT: ";
    for (int i = 0; i < motors.size(); i++) {
      if (motors[i].get_temperature() > 55 && motorConnections[i]) {
        std::string prn = motorNames[i] + " ";
        hotMotors += prn;
      }
    }
    controller.print(2, 0, hotMotors.c_str());
    pros::delay(100);

    // Sets the timer based on the given game mode

    // Timer starts at 15sec for auton
    if (getCompetitionStatus() == AUTONOMOUS && modeIter != 1 && autonSelector == 8) {
      minutes = 1;
      seconds = 0;
      modeIter = 1;
    }
    else if (getCompetitionStatus() == AUTONOMOUS && modeIter != 1) {
      minutes = 0;
      seconds = 15;
      modeIter = 1;
    }
    // Timer starts at 1m45sec for user control
    else if (getCompetitionStatus() == USER_CONTROL && modeIter != 2 && autonSelector == 8) {
      minutes = 1;
      seconds = 0;
      modeIter = 2;
    }
    else if (getCompetitionStatus() == USER_CONTROL && modeIter != 2) {
      minutes = 1;
      seconds = 45;
      modeIter = 2;
    }
    // Timer starts at 0 for pre-autonomous
    else if (getCompetitionStatus() == DISABLED ||
             getCompetitionStatus() == PRE_AUTONOMOUS && modeIter != 0) {
      minutes = 0;
      seconds = 0;
      modeIter = 0;
    }
    time = minutes + (seconds / 100.00);
    // Counts up for pre-autonomous, converting 60 seconds to 1 minute
    if (getCompetitionStatus() == DISABLED ||
        getCompetitionStatus() == PRE_AUTONOMOUS && modeIter == 0) {
      seconds += 1;
      if (seconds >= 60) {
        seconds = 0;
        minutes += 1;
      }
    }

    // Counts down for autonomous
    else if (getCompetitionStatus() == AUTONOMOUS && modeIter == 1 && autonSelector == 8) {
      if (time >= 0.01) {
        seconds -= 1;
        if (seconds <= 0) {
          seconds = 59;
          minutes -= 1;
        }
      } else {
        seconds = 0;
        minutes = 0;
      }
    }
    else if (getCompetitionStatus() == AUTONOMOUS && modeIter == 1) {
      if (time >= 0.01) {
        seconds -= 1;
      } else {
        seconds = 0;
        minutes = 0;
      }
    }

    else if (getCompetitionStatus() == USER_CONTROL && modeIter == 2 && autonSelector == 8) {
      if (time >= 0.01) {
        seconds -= 1;
        if (seconds <= 0) {
          seconds = 59;
          minutes -= 1;
        }
      } else {
        seconds = 0;
        minutes = 0;
      }

      // Long rumble at 15 seconds
      if (time <= 0.10 && rumbleCount == 0) {
        controller.rumble("---");
        rumbleCount = 1;
      }
    }
    // Counts down for user control, converting 0 seconds to 1 minute
    else if (getCompetitionStatus() == USER_CONTROL && modeIter == 2) {
      if (time >= 0.01) {
        seconds -= 1;
        if (seconds <= 0) {
          seconds = 59;
          minutes -= 1;
        }
      } else {
        seconds = 0;
        minutes = 0;
      }

      // Short rumble at 35 seconds
      if (time <= 0.35 && rumbleCount == 0) {
        controller.rumble("...");
        rumbleCount = 1;
      }

      // Long rumble at 15 seconds
      else if (time <= 0.30 && rumbleCount == 1) {
        controller.rumble("---");
        rumbleCount = 2;
      }
    }
    controller.print(0, 0, "%05.2f", time);

    // Staggered delay to clear the lines and give a function total of 1 second
    // (1000ms) of delay to ensure timer remains accurate
    pros::delay(600);
    controller.clear_line(1);
    pros::delay(100);
    controller.clear_line(2);
    pros::delay(100);
  }
}

/**
 * @brief Threaded function that calls the seven individual drawing functions to
 * draw the entire GUI every 5 seconds
 * @relates initialize()
 * @author Leo Abubucker
 * @date 12/01/2024
 */

void clearControllerScreen() {
  while (getCompetitionStatus() == PRE_AUTONOMOUS) {
    secondaryController.clear();
    pros::delay(5000);
  }
}
void drawControllerScoringDisplay() {
  // clearControllerScreen();
  // pros::delay(200);
  int bluePoints = 0;
  int redPoints = 0;
  bool bonusDecided = false;
  bool h2hMode = false;
  int bonusIter = 0;
  std::vector<std::string> autonBonusOptions = {"Blue", "Red", "Tie"};
  std::string bonusPrn = "Bonus: " + bonusIter + autonBonusOptions[bonusIter];
  std::string bluePointsPrn = "B:" + to_string(bluePoints);
  std::string redPointsPrn = "     R:" + to_string(redPoints);
  while (true) {
    while (!bonusDecided) {
      secondaryController.print(0, 0, bonusPrn.c_str());
      pros::delay(100);
      if (secondaryController.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        bonusDecided = true;
        h2hMode = true;
        if (autonBonusOptions[bonusIter] == "Blue") {
          bluePoints += 6;
        } else if (autonBonusOptions[bonusIter] == "Red") {
          redPoints += 6;
        } else if (autonBonusOptions[bonusIter] == "Tie") {
          bluePoints += 3;
          redPoints += 3;
        }
        bluePointsPrn = "B:" + to_string(bluePoints);
        redPointsPrn = "     R:" + to_string(redPoints);
        secondaryController.clear_line(0);
        pros::delay(200);
        break;
      }
      if (secondaryController.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        if (bonusIter >= autonBonusOptions.size() - 1) {
          bonusIter = 0;
        } else {
          bonusIter++;
        }
        bonusPrn = "Bonus: " + autonBonusOptions[bonusIter];
        secondaryController.clear_line(0);
        pros::delay(100);
      } else if (secondaryController.get_digital(
                     pros::E_CONTROLLER_DIGITAL_DOWN)) {
        if (bonusIter <= 0) {
          bonusIter = autonBonusOptions.size() - 1;
        } else {
          bonusIter--;
        }
        bonusPrn = "Bonus: " + bonusIter + autonBonusOptions[bonusIter];
        secondaryController.clear_line(0);
        pros::delay(100);
      }
    }
    while (h2hMode) {
      pros::delay(200);
      secondaryController.print(2, 14, bluePointsPrn.c_str());
      pros::delay(200);

      secondaryController.print(2, 14, redPointsPrn.c_str());
      pros::delay(200);
    }
    pros::delay(1000);
  }
}

/**
 * @brief Threaded function that calls the seven individual drawing functions to
 * draw the entire GUI every 5 seconds
 * @relates initialize()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawGUI() {
  while (true) {
    pros::screen::set_eraser(pros::Color::maroon);
    pros::screen::erase();
    drawControlsFrame();
    drawAutonSelectorFrame();
    drawModeDisplayFrame();
    drawMotorDebugFrame();
    drawBatteryInfoFrame();
    drawControllerInfoFrame();
    drawTeamFrame();
    pros::delay(5000);
  }
}

/**
 * @brief Non-threaded function that calls the seven individual drawing
 * functions to draw the entire GUI
 * @relates autonSelection()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void manuallyDrawGUI() {
  pros::screen::set_eraser(pros::Color::maroon);
  pros::screen::erase();
  drawControlsFrame();
  drawAutonSelectorFrame();
  drawModeDisplayFrame();
  drawMotorDebugFrame();
  drawBatteryInfoFrame();
  drawControllerInfoFrame();
  drawTeamFrame();
}

/**
 * @brief manages user input for the auton selector
 * @relates initialize()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void autonSelection() {
  waitingForUserInput = true;
  while (waitingForUserInput) {
    if (pros::competition::is_connected() &&
        !pros::competition::is_disabled()) {
      waitingForUserInput = false;
      manuallyDrawGUI();
      break;
    }

    // Checks if the user presses the physical auton selection bumper
    if (autonSelectionBumper.get_value() == 1) {
      if (autonSelector < 9) {
        autonSelector++;
      } else {
        autonSelector = 0;
      }
      manuallyDrawGUI();
      pros::delay(200);
    }

    // Checks if the user presses the physical auton confirmation bumper
    if (autonConfirmationBumper.get_value() == 1) {
      waitingForUserInput = false;
      manuallyDrawGUI();
    }
    pros::delay(200);
  }
}

/**
 * @brief Thread function that checks the motor statuses every 5 seconds
 * @relates intialize()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void motorTracking() {
  while (true) {

    myMotorCollection.checkMotors();
    pros::delay(5000);
  }
}

/**
 * @brief Thread function that provides time updates to the controller at set
 * intervals during user control
 * @relates opcontrol()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void timeTracking() {
  // int timeCheck = 0;
  // int time = 0;
  // bool isH2H = false;

  // while (true)
  // {
  // 	if (pros::competition::is_autonomous && !isH2H)
  // 	{
  // 		isH2H = true;
  // 	}
  // 	else if (pros::competition::is_field_control && isH2H)
  // 	{
  // 		controller.print(0, 0, "User Control - H2H");
  // 		while (pros::competition::is_field_control)
  // 		{
  // 			time++;
  // 			// Time update on controller at 1 minute, 30 seconds,
  // and 10 seconds 			if (time >= 45 && timeCheck == 0)
  // 			{
  // 				controller.clear_line(0);
  // 				controller.rumble(".");
  // 				controller.print(0, 0, "1 Minute Remaining");
  // 				timeCheck++;
  // 			}
  // 			else if (time >= 75 && timeCheck == 1)
  // 			{
  // 				controller.clear_line(0);
  // 				controller.rumble(". .");
  // 				controller.print(0, 0, "30 Seconds Remaining");
  // 				timeCheck++;
  // 			}
  // 			else if (time >= 95 && timeCheck == 2)
  // 			{
  // 				controller.clear_line(0);
  // 				controller.rumble(". . .");
  // 				controller.print(0, 0, "10 Seconds Remaining");
  // 				timeCheck++;
  // 			}
  // 			pros::delay(1000);
  // 		}
  // 	}
  // 	else if (pros::competition::is_field_control && !isH2H)
  // 	{
  // 		controller.print(0, 0, "User Control - Skills");
  // 		while (pros::competition::is_field_control)
  // 		{
  // 			time++;
  // 			// Time update on controller at 45 seconds, 30 seconds,
  // and 15 seconds 			if (time >= 15 && timeCheck == 0)
  // 			{
  // 				controller.clear_line(0);
  // 				controller.rumble(".");
  // 				controller.print(0, 0, "45 Seconds Remaining");
  // 				timeCheck++;
  // 			}
  // 			else if (time >= 30 && timeCheck == 1)
  // 			{
  // 				controller.clear_line(0);
  // 				controller.rumble(". .");
  // 				controller.print(0, 0, "30 Seconds Remaining");
  // 				timeCheck++;
  // 			}
  // 			else if (time >= 45 && timeCheck == 2)
  // 			{
  // 				controller.clear_line(0);
  // 				controller.rumble(". . .");
  // 				controller.print(0, 0, "15 Seconds Remaining");
  // 				timeCheck++;
  // 			}
  // 			pros::delay(1000);
  // 		}
  // 	}
  // 	pros::delay(1000);
  // }
}

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       VEX COMPETITION CONTROLLED FUNCTIONS */
/*                                                                                    */
/*  VEX competition controlled functions are those that are automatically called
 * by   */
/*  VEX tournament management systems and should not be manually called */
/*  Includes: */
/*  - void initialize() - pre-game initializations, thread starting, auton
 * selection  */
/*  - void disabled() - tasks to run when robot is disabled - NOT USED */
/*	- void competition_initialize() - comp specific initializations - NOT
 * USED        */
/*  - void autonomous() - 15 sec or 1min autonomous robot movement */
/*  - void opcontrol() - 1m45s loop of user-controlled robot movement /
/* */
/*------------------------------------------------------------------------------------*/

/**
 * @brief Runs initialization code. This occurs as soon as the program is
 * started.
 * @details All other competition modes are blocked by initialize; it is
 * recommended to keep execution time for this mode under a few seconds.
 * @author Leo Abubucker
 * @date 1/6/2025
 */
void initialize() {
  pros::Task controllerDisplayTask(drawControllerDisplay);

  // MotorCollection Initialization
  myMotorCollection.addMotor(rightArm, "LA");
  myMotorCollection.addMotor(leftArm, "RA");
  myMotorCollection.addMotor(leftBack, "LB");
  myMotorCollection.addMotor(leftFront, "LF");
  myMotorCollection.addMotor(rightBack, "RB");
  myMotorCollection.addMotor(rightFront, "RF");
  myMotorCollection.addMotor(rightIntake, "RI");
  myMotorCollection.addMotor(leftIntake, "LI");

  // Motor Initialization
  leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  intakeMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  armMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  allMotors.tare_position();

  // Sensor Calibration
  chassis.calibrate();

  // Task Initialization
  pros::Task guiUpdatingTask(drawGUI);
  pros::Task motorTrackingTask(motorTracking);
  pros::Task scoringControllerTask(drawControllerScoringDisplay);

  // Auton Selection
  autonSelector = 0;
  autonSelection();
}

/**
 * @brief Runs while the robot is disabled
 * @details Runs while the robot is in the disabled state of Field Management
 * System or the VEX Competition Switch, following either autonomous or
 * opcontrol. When the robot is enabled, this task will exit.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void disabled() {}

/**
 * @brief Runs after initialize() and before autonomous
 * @details This is intended for competition-specific initialization routines,
 * such as an autonomous selector on the LCD. This task will exit when the robot
 * is enabled and autonomous or opcontrol starts.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void competition_initialize() {}

/**
 * @brief Runs the user autonomous code.
 * @details This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field
 * Management System or the VEX Competition Switch in the autonomous mode.
 * Alternatively, this function may be called in initialize or opcontrol for
 * non-competition testing purposes. If the robot is disabled or communications
 * is lost, the autonomous task will be stopped. Re-enabling the robot will
 * restart the task, not re-start it from where it left off.
 * @author Leo Abubucker
 * @date 1/9/2025
 */
void autonomous() {
  // Red Ring Rush Qualifications
  if (autonSelector == 0) {
    // // 1
    chassis.moveToPoint(2, -24, 3000, {.forwards = false}, false);
    clamp.set_value(true);
    chassis.turnToHeading(90, 2000);
    intakeMotors.move_relative(-999999, 150);
    chassis.moveToPoint(32, -30, 2000, {}, false);
    chassis.turnToHeading(180, 1000);
    chassis.moveToPoint(36, -41, 2000, {}, false);
    chassis.moveToPoint(36, -30, 2000, {.forwards=false}, false);
    chassis.turnToHeading(180, 2000);
    chassis.moveToPose(28, -46, 180, 2000, {}, false);
    chassis.moveToPose(28, -30, 180, 2000, {.forwards=false}, false);
    chassis.turnToHeading(224, 1000);
    chassis.moveToPoint(0, -46, 6000, {}, false);
  }

  // Blue Ring Rush Qualifications
  else if (autonSelector == 1) {
    chassis.moveToPoint(-2, -24, 3000, {.forwards = false}, false);
    clamp.set_value(true);
    chassis.turnToHeading(270, 2000, {.direction=AngularDirection::CCW_COUNTERCLOCKWISE});
    intakeMotors.move_relative(-999999, 150);
    chassis.moveToPoint(-28, -30, 2000, {}, false);
    chassis.turnToHeading(180, 1000);
    chassis.moveToPoint(-28, -46, 2000, {}, false);
    chassis.moveToPoint(-24, -35, 2000, {.forwards=false}, false);
    chassis.turnToHeading(180, 2000);
    chassis.moveToPose(-24, -46, 180, 2000, {}, false);
    chassis.moveToPose(-24, -35, 180, 2000, {.forwards=false}, false);
    chassis.turnToHeading(45, 1000);
    chassis.moveToPoint(0, -46, 6000, {.minSpeed=127}, false);

    // New Left
    // armMotors.move_absolute(2370, 100);
    // chassis.moveToPose(-11, 6, 0, 4000, {}, false);
    // intakeMotors.move_relative(1000, 100);
    // pros::delay(200);
    // armMotors.move_absolute(1900, 100);
    // pros::delay(500);
    // armMotors.move_absolute(2000, 100);
    // chassis.moveToPose(10, -38, 0, 4000, {.forwards = false}, false);
    // armMotors.move_absolute(0, 100);
    // clamp.set_value(true);
    // intakeMotors.move_relative(-5500, 200);
    // chassis.moveToPoint(35, -28, 2000, {}, true);
    // chassis.moveToPose(9, -55, 180, 4000, {}, false);
    // intakeMotors.move_relative(-7000, 200);
  }

  // Red Goal Rush Qualifications
  else if (autonSelector == 2) {
    armMotors.move_absolute(2000, 100);
    chassis.moveToPoint(-10, 36, 4000, {.minSpeed=127}, false);
    armMotors.move_absolute(3000, 100);
    chassis.turnToHeading(90, 1000);
    armMotors.move_absolute(1000, 100);
    pros::delay(500);
    chassis.moveToPoint(-15, 34, 4000, {.forwards=false}, false);
    chassis.turnToHeading(280, 1000);
    chassis.moveToPoint(-28, 20, 3000, {.forwards=false}, false);
    armMotors.move_absolute(0, 80);
    clamp.set_value(true);
    chassis.turnToHeading(270, 3000, {}, false);
    intakeMotors.move_relative(-999999, 150);
    chassis.moveToPose(-52, 24, 90, 4000, {}, false);
    // Old Left
    // 1
    // chassis.turnToHeading(180, 2000);
    // chassis.moveToPoint(2, 37, 4000, {.forwards = false}, false);
    // clamp.set_value(true);
    // intakeMotors.move_relative(-10000, 105);
    // 2
    // chassis.moveToPose(-15, 28, 270, 2000, {}, true);
    // chassis.moveToPoint(-24, 28, 2000, {}, true);
    // chassis.moveToPoint(15, 25, 4000, {.forwards = false}, false);
    // Goal
    // chassis.moveToPose(0, 50, 0, 4000, {}, false);
  }
  // Blue Goal Rush Qualifications
  else if (autonSelector == 3) {
    armMotors.move_absolute(2000, 100);
    chassis.moveToPoint(10, 36, 4000, {.minSpeed=127}, false);
    armMotors.move_absolute(3000, 100);
    chassis.turnToHeading(270, 1000);
    armMotors.move_absolute(1000, 100);
    pros::delay(500);
    chassis.moveToPoint(15, 34, 4000, {.forwards=false}, false);
    chassis.turnToHeading(280, 1000);
    chassis.moveToPoint(28, 20, 3000, {.forwards=false}, false);
    armMotors.move_absolute(0, 80);
    clamp.set_value(true);
    chassis.turnToHeading(90, 3000, {}, false);
    intakeMotors.move_relative(-999999, 150);
    chassis.moveToPose(52, 24, 90, 4000, {}, false);
    // chassis.moveToPose(48, 0, 90, 4000, {}, false);
    // New Right
    // armMotors.move_absolute(2370, 100);
    // chassis.moveToPose(16, 7, 0, 3000, {}, false);
    // intakeMotors.move_relative(1000, 100);
    // pros::delay(200);
    // armMotors.move_absolute(2000, 100);
    // pros::delay(500);
    // armMotors.move_absolute(2050, 100);
    // chassis.moveToPose(-10, -42, 0, 4000, {.forwards = false}, false);
    // clamp.set_value(true);
    // armMotors.move_absolute(0, 100);
    // intakeMotors.move_relative(-7000, 200);
    // chassis.moveToPose(-35, -28, 90, 2000, {}, true);
    // chassis.moveToPose(-9, -55, 180, 4000, {}, false);
    // intakeMotors.move_relative(-7000, 200);
  }

  // Red Ring Rush Eliminations
  else if (autonSelector == 4) {
    // Old Skills
    // 1
    // clamp.set_value(true);
    // chassis.moveToPoint(0, 11, 2000, {}, false);
    // chassis.turnToHeading(90, 2000);
    // chassis.moveToPoint(-40, 11, 4000, {false}, false);
    // chassis.turnToHeading(55, 2000, {AngularDirection::CCW_COUNTERCLOCKWISE});
    // chassis.moveToPoint(-60, -10, 4000, {false}, false);
    // 2
    // chassis.moveToPoint(0, 11, 2000, {}, false);
    // chassis.turnToHeading(270, 2000);
    // chassis.moveToPoint(40, 11, 4000, {false}, false);
    // chassis.turnToHeading(305, 2000, {AngularDirection::CW_CLOCKWISE});
    // chassis.moveToPoint(60, -10, 4000, {false}, false);
    // 3
    // chassis.moveToPose(10, 30, 315, 4000, {}, true);

  }

  // Blue Ring Rush Eliminations
  else if (autonSelector == 5) {
    
  }

  // Red Goal Rush Eliminations
  else if (autonSelector == 6){

  }

  // Blue Goal Rush Eliminations
  else if (autonSelector == 7){

  }

  // Skills
  else if (autonSelector == 8) {
    // 1st Goal
    chassis.moveToPoint(0, -20, 5000, {.forwards = false}, false);
    clamp.set_value(true);
    chassis.turnToHeading(180, 3000, {}, false);
    intakeMotors.move_relative(-999999, 150);
    chassis.moveToPoint(5, -35, 5000, {}, false);
    chassis.turnToHeading(90, 2000, {}, false);
    chassis.moveToPoint(30, -35, 5000, {}, false);
    chassis.turnToHeading(0, 2000, {}, false);
    chassis.moveToPoint(30, 0, 5000, {}, false);
    chassis.turnToHeading(240, 2000, {}, false);
    chassis.moveToPoint(25, -5, 3000, {}, false);
    chassis.moveToPoint(50, 0, 3000, {.forwards = false}, false);
    clamp.set_value(false);
    intakeMotors.brake();
    chassis.moveToPoint(25, -5, 3000, {}, false);

    // 2nd Goal
    chassis.turnToHeading(90, 2000, {}, false);
    chassis.moveToPose(-52, -5, 90, 4000, {.forwards = false}, false);
    clamp.set_value(true);
    intakeMotors.move_relative(-999999, 150);
    chassis.turnToHeading(270, 2000, {}, false);
    chassis.moveToPose(-75, -5, 90, 5000, {}, false);
    chassis.turnToHeading(180, 2000, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE}, false);
    clamp.set_value(false);
    chassis.moveToPoint(-79, 12, 3000, {.forwards = false}, false);
    intakeMotors.brake();
    chassis.moveToPoint(-65, -5, 4000, {}, false);
    intakeMotors.move_relative(-999999, 150);
    
    // 3rd Goal
    chassis.turnToHeading(180, 2000, {}, false);
	  intakeMotors.brake();
    chassis.moveToPose(-55, -80, 350, 5000, {}, false);
    chassis.moveToPose(-45, -100, 320, 4000, {.forwards=false}, false);
    chassis.moveToPose(-30, -120, 0, 4000, {.forwards=false}, false);
    clamp.set_value(true);
    chassis.moveToPose(-45, -100, 320, 4000, {}, false);
    chassis.turnToHeading(315, 2000, {}, false);
    chassis.moveToPoint(-60, -110, 4000, {}, false);
    chassis.turnToHeading(45, 2000, {}, false);
    chassis.moveToPoint(-79, -120, 4000, {.forwards=false}, false);
    clamp.set_value(false);
    chassis.moveToPoint(-45, -100, 4000, {}, false);

    // Skills?
    // 1st Goal
    // chassis.moveToPoint(0, -20, 5000, {.forwards = false}, false);
    // clamp.set_value(true);
    // chassis.turnToHeading(180, 3000, {}, false);
    // intakeMotors.move_relative(-999999, 150);
    // chassis.moveToPoint(5, -35, 5000, {}, false);
    // chassis.turnToHeading(90, 2000, {}, false);
    // chassis.moveToPoint(30, -35, 5000, {}, false);
    // chassis.turnToHeading(0, 2000, {}, false);
    // chassis.moveToPoint(30, 0, 5000, {}, false);
    // chassis.turnToHeading(240, 2000, {}, false);
    // chassis.moveToPoint(25, -5, 5000, {}, false);
    // chassis.moveToPoint(50, -5, 5000, {.forwards = false}, false);
    // clamp.set_value(false);
    // intakeMotors.brake();
    // chassis.moveToPoint(25, -5, 3000, {}, false);
    // 2nd Goal
    // chassis.turnToHeading(90, 4000, {}, false);
    // chassis.moveToPose(-52, -5, 90, 5000, {.forwards = false}, false);
    // clamp.set_value(true);
    // chassis.turnToHeading(180, 4000, {}, false);
    // intakeMotors.move_relative(-999999, 150);
    // chassis.moveToPoint(-38, -34, 4000, {}, false);
    // chassis.turnToHeading(270, 2000, {}, false);
    // chassis.moveToPoint(-75, -35, 5000, {}, false);
    // chassis.turnToHeading(0, 2000, {}, false);
    // chassis.moveToPoint(-75, 0, 5000, {}, false);
    // chassis.turnToHeading(160, 2000, {}, false);
    // chassis.moveToPoint(-73, -5, 5000, {}, false);
    // chassis.moveToPoint(-79, 12, 5000, {.forwards = false}, false);
    // clamp.set_value(false);
    // intakeMotors.brake();
    // chassis.moveToPoint(-65, -5, 4000, {}, false);
    // intakeMotors.move_relative(-999999, 150);
    // 3rd Goal
    // chassis.turnToHeading(180, 2000, {}, false);
	  // intakeMotors.brake();
    // chassis.moveToPose(-55, -80, 350, 5000, {}, false);
    // chassis.moveToPose(-45, -100, 320, 4000, {.forwards=false}, false);
    // chassis.moveToPose(-30, -120, 320, 4000, {.forwards=false}, false);
    // clamp.set_value(true);
    // chassis.moveToPose(-45, -100, 320, 4000, {}, false);
    // chassis.turnToHeading(315, 2000, {}, false);
    // chassis.moveToPoint(-60, -110, 5000, {}, false);
    // chassis.turnToHeading(45, 2000, {}, false);
    // chassis.moveToPoint(-79, -120, 5000, {.forwards=false}, false);
    // clamp.set_value(false);
    // chassis.moveToPoint(-45, -100, 4000, {}, false);
  }
}

/**
 * @brief Runs the operator control code.
 * @details This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field
 * Management System or the VEX Competition Switch in the operator control mode.
 * If no competition control is connected, this function will run immediately
 * following initialize(). If the robot is disabled or communications is lost,
 * the operator control task will be stopped. Re-enabling the robot will restart
 * the task, not resume it from where it left off.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void opcontrol() {
  bool clampState = true;
  bool clampLastState = true;

  if (autonSelector == 8) {
    clampState = false;
    clampLastState = false;
  }
  // pros::Task timeTrackingTask(timeTracking);

  while (true) { // Run for 20 ms then update
    leftMotors.move(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
    rightMotors.move(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

    // // Clamp Controls
    // if ((controller.get_digital(pros::E_CONTROLLER_DIGITAL_X) ||
    // (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) &&
    // controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) &&
    // !clampLastState)
    // {
    // 	clampState = !clampState;
    // 	clampLastState = true;
    // }
    // else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_X) ||
    // (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) &&
    // !controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)))
    // {
    // 	clampLastState = false;
    // }

    // if (clampState)
    // {
    // 	clamp.set_value(true);
    // }
    // else
    // {
    // 	clamp.set_value(false);
    // }

    // Clamp Controls
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !clampLastState) {
      clampState = !clampState;
      clampLastState = true;
    } else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
      clampLastState = false;
    }

    if (clampState) {
      clamp.set_value(true);
    } else {
      clamp.set_value(false);
    }
    // Intake Controls

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intakeMotors.move_velocity(-187);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intakeMotors.move_velocity(187);
    } else {
      intakeMotors.brake();
    }

    // Arm Controls

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
      // Position to go to the top of a neutral stake - DOES NOT WORK
      armMotors.move_absolute(720, 100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      // Position to reset the arm
      armMotors.move_absolute(0, 100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      armMotors.move_velocity(100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      armMotors.move_velocity(-100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
      // Position to go to the top of an alliance wall stake
      armMotors.move_absolute(2400, 100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      // Position to go to the top of an alliance wall stake
      armMotors.move_absolute(2400, 100);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) ||
               controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      // Position to go to climb position
      armMotors.move_absolute(4500, 100);
    } else {
      armMotors.brake();
    }

    pros::delay(20);
  }
}