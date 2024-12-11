#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <sstream>

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                      GLOBAL DECLARATIONS AND INITIALIZATIONS                       */
/*  Declarations and initializations of PROS, LEMLIB, and OTHER global variables      */
/*  PROS Initializations:                                                             */
/*  - pros::Controller controller - represents the VEX v5 Controller constructed as   */
/*      the "primary" or the "partner" controller                                     */
/*  - pros::Motor motorName - represents a VEX V5 Motor constructed with a port from  */
/*      1-21 with negatives representing reversed motors and either a red or green    */
/*      gearset.                                                                      */
/*  - pros::MotorGroup motorGroupName - represents a group of motors constructed with */
/*      the motors in the group. primarily used for mass initializations of motor     */
/*      attributes.                                                                   */
/*  - pros::Imu imuName - represents a VEX V5 Inertial Sensor                         */
/*  - pros::adi::DigitalOut digitalOutDevice - represents a 3-wire Digital Out Device */
/*  - pros::adi::DigitalIn digitalInDevice - represents a 3-wire Digital In Device    */
/*  - bumper bumperName - represents a VEX bumper constructed with a specified        */
/*      triport port.                                                                 */
/*  LEMLIB Initializations:                                                           */
/*  - lemlib::ControllerSettings linearController - lateral motion controller         */
/*  - lemlib::ControllerSettings angularController - angular motion controller        */
/*  - lemlib::OdomSensors sensors - odometry sensors                                  */
/*  - lemlib::ExpoDriveCurve throttleCurve - input throttle curve for user control    */
/*  - lemlib::ExpoDriveCurve steerCurve - input steer curve for user control          */
/*  - lemlib::Chassis chassis - chassis                                               */
/*  OTHER Declarations:                                                               */
/*  - int autonSelector - integer representing the autonomous program that the user   */
/*      selects to run.                                                               */
/*  - std::map<std::string, int> autonSelectorFrame - map representing the            */
/*      coordinates of the autonomous selector GUI button on the brain.               */
/*  - enum DriveConfigurations{} - enums representing the 4 drive configs             */
/*  - DriveConfigurations driveConfig - global instance of DriveConfigurations        */
/*  - enum Keywords{} - enums representing the 7 keywords                             */
/*  - enum GameStates{} - enums representing the 5 game states                        */
/*  - enum MovementDirections{} - enums representing the 4 movement directions        */
/*  OTHER Initializations:                                                            */
/*  - bool waitingForUserInput = false - boolean representing whether the user has    */
/*      provided input confirming the autonomous program to be run.                   */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

// Controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// Motors
pros::Motor rightArm(1, pros::MotorGearset::red);
pros::Motor leftArm(-9, pros::MotorGearset::red);
pros::Motor leftBack(-11, pros::MotorGearset::green);
pros::Motor leftFront(-15, pros::MotorGearset::green);
pros::Motor rightBack(13, pros::MotorGearset::green);
pros::Motor rightFront(15, pros::MotorGearset::green);
pros::Motor leftIntake(-10, pros::MotorGearset::green);
pros::Motor rightIntake(2, pros::MotorGearset::green);

// Motor Groups
pros::MotorGroup allMotors({-15, -11, 14, 13, -10, 2, -9, 1});

pros::MotorGroup leftMotors({-15, -11},
							pros::MotorGearset::green);			   // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({14, 13}, pros::MotorGearset::green); // right motor group - ports 6, 7, 9 (reversed)

pros::MotorGroup intakeMotors({-10, 2}, pros::MotorGearset::green);

pros::MotorGroup armMotors({-9, 1}, pros::MotorGearset::red);

// Inertial Sensor
pros::Imu imu(20);

// Piston
pros::adi::DigitalOut clamp(1);

// Auton Selection Bumper
pros::adi::DigitalIn autonSelectionBumper(5);

// Auton Confirmation Bumper
pros::adi::DigitalIn autonConfirmationBumper(3);

// Drivetrain Settings
lemlib::Drivetrain drivetrain(&leftMotors,				  // left motor group
							  &rightMotors,				  // right motor group
							  9.2,						  // 9.2 inch track width
							  lemlib::Omniwheel::OLD_275, // using old 2.75" omnis
							  333.33,					  // drivetrain rpm is 333.33
							  2							  // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// Lateral Motion Controller
lemlib::ControllerSettings linearController(10,	 // proportional gain (kP)
											0,	 // integral gain (kI)
											3,	 // derivative gain (kD)
											3,	 // anti windup
											1,	 // small error range, in inches
											100, // small error range timeout, in milliseconds
											3,	 // large error range, in inches
											500, // large error range timeout, in milliseconds
											20	 // maximum acceleration (slew)
);

// Angular Motion Controller
lemlib::ControllerSettings angularController(2,	  // proportional gain (kP)
											 0,	  // integral gain (kI)
											 10,  // derivative gain (kD)
											 3,	  // anti windup
											 1,	  // small error range, in degrees
											 100, // small error range timeout, in milliseconds
											 3,	  // large error range, in degrees
											 500, // large error range timeout, in milliseconds
											 0	  // maximum acceleration (slew)
);

// Sensors for Odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel,  set to nullptr as we don't have one
							nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
							nullptr, // horizontal tracking wheel,  set to nullptr as we don't have one
							nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
							&imu	 // inertial sensor
);

// Input Curve for Throttle Input During Driver Control
lemlib::ExpoDriveCurve throttleCurve(3,	   // joystick deadband out of 127
									 10,   // minimum output where drivetrain will move out of 127
									 1.019 // expo curve gain
);

// Input Curve for Steer Input During Driver Control
lemlib::ExpoDriveCurve steerCurve(3,	// joystick deadband out of 127
								  10,	// minimum output where drivetrain will move out of 127
								  1.019 // expo curve gain
);

// Create the Chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

// User Input Bool
bool waitingForUserInput = false;

// Auton Selector
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
	CONTROLLER_BATTERY,
	MOTOR_TEMPERATURE,
	BATTERY,
	CONTROLLER_CONNECTION,
	MOTOR_TYPE
};
enum GameStates
{
	PRE_AUTONOMOUS,
	AUTONOMOUS,
	USER_CONTROL,
	DISABLED,
	ENABLED
};

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       HELPER FUNCTIONS AND CLASSES                                 */
/*                                                                                    */
/*  Helper functions and classes that assist VEX competition controlled functions.    */
/*  Includes:                                                                         */
/*  - std::string to_string(T value) - To string template function                    */
/*  - Motor Collection Class - Provides additional functionality for VEX V5 motors    */
/*  - GameStates getCompetitionStatus() - gets the current competition state          */
/*  - pros::Color getColorFromValue(std::string value, Keywords keyword) - returns    */
/*    a color based on the given value and keyword                                    */
/*  - pros::Color getColorFromValue(bool value, Keywords keyword) - returns           */
/*    a color based on the given value and keyword                                    */
/*  - pros::Color getColorFromValue(int value, Keywords keyword) - returns            */
/*    a color based on the given value and keyword                                    */
/*  - pros::Color getColorFromValue(GameStates value, Keywords keyword) - returns     */
/*    a color based on the given value and keyword                                    */
/*	- void drawDashedLine(int x0, int y0, int x1, int y1, int dashLength, pros::Color */
/*	  dashColor) - draws a dashed line given the coordinates, length, and color       */
/*  - void drawControlsFrame() - draws the control frame                              */
/*  - void drawAutonSelectorFrame() - draws the autononomous selection frame          */
/*  - void drawModeDisplayFrame() - draws the mode display frame                      */
/*  - void drawMotorDebugFrame() - draws the motor debug frame                        */
/*  - void drawBatteryInfoFrame() - draws the battery info frame                      */
/*  - void drawControllerInfoFrame() - draws the control frame                        */
/*  - void drawTeamFrame() - draws the team frame                                     */
/*  - void drawGUI() - thread function that calls the seven draw functions above      */
/*	- void manuallyDrawGUI() - non-threaded version of drawGUI()                      */
/*  - void autonSelection() - Autonomous program selection function                   */
/*  - void motorTracking() - thread function that routinely check status of motors    */
/*  - void timeTracking() - thread function that alerts controller w/ time updates    */
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
 * @brief MotorCollection class provides additional functionality to VEX V5 motors
 * @details This class provides additional functionality in detecting the connection
 * status of motors in the collection, dynamically updating the drive configuration
 * based on the connection status of the drive motors, and returns attributes
 * of the motors.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
class MotorCollection
{
public:
	// Init vector of motor objects
	std::vector<pros::Motor> motorList;
	// Init vector of std::strings representing motor names
	std::vector<std::string> motorNamesList;

	/**
	 * @brief adds the passed in motor object and std::string object motorName to respective lists
	 * @relates pre_auton()
	 * @param newMotor VEX V5 motor
	 * @param newMotorName std::string name of newMotor
	 * @author Leo Abubucker
	 * @date 12/01/2024
	 */
	void addMotor(pros::Motor newMotor, std::string newMotorName)
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
	 * @date 12/01/2024
	 */
	std::vector<bool> isConnected()
	{

		std::vector<bool> motorConnections;
		for (int i = 0; i < motorList.size(); i++)
		{
			motorConnections.push_back(motorList[i].is_installed());
			if (!motorList[i].is_installed())
			{
				controller.clear_line(0);
				controller.set_text(0, 0, motorNamesList[i].c_str());
			}
		}
		return motorConnections;
	}

	/**
	 * @brief determines the optimal drive configuration based on the connection of drive motors
	 * @relates usercontrol()
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
	 * @returns 2D vector where each value is [name: cartridge, type, temperatureC, positiondeg]
	 * @author Leo Abubucker
	 * @date 12/01/2024
	 */
	std::vector<std::vector<std::string>> returnValues()
	{
		std::vector<std::vector<std::string>> returnedList;
		std::string motorCartridgeType;
		std::string motorType;
		std::string motorTemp;
		std::string motorPos;

		for (int i = 0; i < motorList.size(); i++)
		{
			if (motorList[i].get_gearing() == pros::MotorGears::red)
			{
				motorCartridgeType = "Red";
			}
			else if (motorList[i].get_gearing() == pros::MotorGears::green)
			{
				motorCartridgeType = "Green";
			}
			else if (motorList[i].get_gearing() == pros::MotorGears::blue)
			{
				motorCartridgeType = "Blue";
			}
			else
			{
				motorCartridgeType = "ERR";
			}
			if (motorCartridgeType == "ERR")
			{
				motorType = "ERR";
			}
			else
			{
				motorType = "11W";
			}
			if (motorList[i].get_temperature() > 0 && motorList[i].get_temperature() < 200)
			{
				motorTemp = to_string(motorList[i].get_temperature()) + "C";
			}
			else
			{
				motorTemp = "ERR";
			}

			if (std::abs(motorList[i].get_position()) < 999999)
			{
				motorPos = to_string(round(motorList[i].get_position() * 100.00) / 100.00) + "deg";
			}
			else
			{
				motorPos = "ERR";
			}
			returnedList.push_back({motorNamesList[i] + ": ", motorCartridgeType + ", ", motorType + ", ", motorTemp + ", ", motorPos});
		}
		return returnedList;
	}

	/**
	 * @brief returns the list of motors
	 * @relates drawMotorDebugFrame()
	 * @author Leo Abubucker
	 * @date 12/01/2024
	 */
	std::vector<pros::Motor> returnMotors()
	{
		return motorList;
	}
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
GameStates getCompetitionStatus()
{
	if (!pros::competition::is_connected())
	{
		return DISABLED;
	}
	else if (pros::competition::is_disabled())
	{
		return PRE_AUTONOMOUS;
	}
	else if (pros::competition::is_autonomous())
	{
		return AUTONOMOUS;
	}
	return USER_CONTROL;
}

/**
 * @brief gets a pros::Color based on an std::string value and Keyword keyword
 * @details This function takes a Keyword keyword and std::string value and returns a pros::Color based
 * on specific condition(s) that the value meets. The conditions that are used are based on the keyword.
 * @relates drawModeDisplayFrame(), drawMotorDebugFrame()
 * @overload getColorFromValue(bool value, Keyword keyword)
 * @overload getColorFromVaue(int value, Keyword keyword)
 * @overload getColorFromValue(GameStates value, Keyword keyword)
 * @param value std::string value that determines color
 * @param keyword Keyword keyword that determines which conditions value is checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(std::string value, Keywords keyword)
{
	if (keyword == GAME_STATE)
	{
		if (value == "AUTON")
		{
			return pros::Color::orange;
		}
		else if (value == "USER")
		{
			return pros::Color::light_green;
		}
		else if (value == "PRE-AUTON")
		{
			return pros::Color::yellow;
		}
		else
		{
			return pros::Color::red;
		}
	}
	else if (keyword == MOTOR_CARTRIDGE)
	{
		if (value == "Green")
		{
			return pros::Color::light_green;
		}
		else if (value == "Red")
		{
			return pros::Color::red;
		}
		else if (value == "Blue")
		{
			return pros::Color::blue;
		}
		else
		{
			return pros::Color::red;
		}
	}
	else if (keyword == MOTOR_TYPE)
	{
		if (value == "11W, ")
		{
			return pros::Color::white;
		}
		else
		{
			return pros::Color::red;
		}
	}
	return pros::Color::white;
}

/**
 * @brief gets a pros::Color based on a bool value and Keyword keyword
 * @details This function takes a Keyword keyword and bool value and returns a pros::Color based
 * on specific condition(s) that the value meets. The conditions that are used are based on the keyword.
 * @relates drawAutonSelectorFrame(), drawMotorDebugFrame(), drawControllerInfoFrame()
 * @overload getColorFromValue(std::string value, Keyword keyword)
 * @overload getColorFromVaue(int value, Keyword keyword)
 * @overload getColorFromValue(GameStates value, Keyword keyword)
 * @param value bool value that determines color
 * @param keyword Keyword keyword that determines which conditions value is checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(bool value, Keywords keyword)
{
	if (keyword == USER_INPUT)
	{
		if (value)
		{
			return pros::Color::orange;
		}
		else
		{
			return pros::Color::light_green;
		}
	}
	else if (keyword == MOTOR_CONNECTION)
	{
		if (value)
		{
			return pros::Color::light_green;
		}
		else
		{
			return pros::Color::red;
		}
	}
	else if (keyword == CONTROLLER_CONNECTION)
	{
		if (value)
		{
			return pros::Color::light_green;
		}
		else
		{
			return pros::Color::red;
		}
	}
	return pros::Color::white;
}

/**
 * @brief gets a pros::Color based on an int value and Keyword keyword
 * @details This function takes an Keyword keyword and int value and returns a pros::Color based
 * on specific condition(s) that the value meets. The conditions that are used are based on the keyword.
 * @relates drawMotorDebugFrame(), drawBatteryInfoFrame()
 * @overload getColorFromValue(std::string value, Keyword keyword)
 * @overload getColorFromVaue(bool value, Keyword keyword)
 * @overload getColorFromValue(GameStates value, Keyword keyword)
 * @param value int value that determines color
 * @param keyword Keyword keyword that determines which conditions value is checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(int value, Keywords keyword)
{
	if (keyword == MOTOR_TEMPERATURE)
	{
		// Motors will cut their maxium current to 50% at 55 degrees Celsius, to 25% at 60 degrees Celsius,
		// to 12.5% at 65 degrees Celsius, and turn off at 70 degrees celsius
		if (value < 55 && value > 0)
		{
			return pros::Color::light_green;
		}
		else if (value < 70 && value > 0)
		{
			return pros::Color::orange;
		}
		else
		{
			return pros::Color::red;
		}
	}
	else if (keyword == BATTERY || keyword == CONTROLLER_BATTERY)
	{
		if (value > 80)
		{
			return pros::Color::light_green;
		}
		else if (value > 50)
		{
			return pros::Color::orange;
		}
		else
		{
			return pros::Color::red;
		}
	}
	return pros::Color::white;
}

/**
 * @brief gets a pros::Color based on an int value and Keyword keyword
 * @details This function takes an Keyword keyword and GameStates value and returns a pros::Color based
 * on specific condition(s) that the value meets. The conditions that are used are based on the keyword.
 * @relates drawMotorDebugFrame(), drawBatteryInfoFrame()
 * @overload getColorFromValue(std::string value, Keyword keyword)
 * @overload getColorFromVaue(bool value, Keyword keyword)
 * @overload getColorFromValue(int value, Keyword keyword)
 * @param value int value that determines color
 * @param keyword Keyword keyword that determines which conditions value is checked.
 * @returns pros::Color determined by value. Returns pros::Color::white if no value, keyword pair matches.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
pros::Color getColorFromValue(GameStates value, Keywords keyword)
{
	if (keyword == GAME_STATE)
	{
		if (value == AUTONOMOUS)
		{
			return pros::Color::orange;
		}
		else if (value == USER_CONTROL)
		{
			return pros::Color::light_green;
		}
		else if (value == PRE_AUTONOMOUS)
		{
			return pros::Color::yellow;
		}
		else
		{
			return pros::Color::red;
		}
	}

	return pros::Color::white;
}

/**
 * @brief draws a dashed line of a given color from a given start point to a given end point where each dash is a given length
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
void drawDashedLine(int x0, int y0, int x1, int y1, int dashLength, pros::Color dashColor)
{
	int distance = std::sqrt(std::pow(std::abs(x1 - x0), 2) + std::pow(std::abs(y1 - y0), 2));

	int numDashes = (distance / 2) / dashLength;
	bool isVertical = true;
	if (std::abs(y1 - y0) == 0)
	{
		isVertical = false;
	}
	int dashX0 = x0;
	int dashX1;
	int dashY0 = y0;
	int dashY1;
	if (isVertical)
	{
		dashX1 = dashX0;
		dashY1 = dashY0 + dashLength;
	}
	else
	{
		dashY1 = dashY0;
		dashX1 = dashX0 + dashLength;
	}
	pros::screen::set_pen(dashColor);
	for (int i = 0; i < numDashes; i++)
	{
		pros::screen::draw_line(dashX0, dashY0, dashX1, dashY1);
		if (isVertical)
		{
			dashY0 = dashY1 + dashLength;
			dashY1 = dashY0 + dashLength;
		}
		else
		{
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
void drawControlsFrame()
{
	// Coordinates of the control frame
	std::map<std::string, int> controlsFrame = {
		{"x-left", 0},
		{"x-right", 182},
		{"y-top", 0},
		{"y-bottom", 190}};

	// Draws maroon rectangle at above coordinates
	pros::screen::set_pen(pros::Color::maroon);
	pros::screen::fill_rect(controlsFrame["x-left"], controlsFrame["y-top"], controlsFrame["x-right"], controlsFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::draw_rect(controlsFrame["x-left"], controlsFrame["y-top"], controlsFrame["x-right"], controlsFrame["y-bottom"]);

	// Prints Controls in the frame
	std::vector<std::string> controls = {"Driving - Tank", "Lift Arm - R1", "Lower Arm - R2", "Intake - L1", "Outtake - L2", "Clamp - X", "Toggle - Up"};
	pros::screen::set_pen(pros::Color::white);
	pros::screen::print(pros::E_TEXT_MEDIUM, 50, 10, "Controls");
	int y = 30;
	for (int i = 0; i < controls.size(); i++)
	{
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
void drawAutonSelectorFrame()
{
	// Coordinates of the autonomous selector frame
	autonSelectorFrame = {
		{"x-left", 0},
		{"x-right", 99},
		{"y-top", 189},
		{"y-bottom", 240}};

	// Draws maroon rectangle at above coordinates
	pros::screen::set_pen(pros::Color::maroon);
	pros::screen::fill_rect(autonSelectorFrame["x-left"], autonSelectorFrame["y-top"], autonSelectorFrame["x-right"], autonSelectorFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::draw_rect(autonSelectorFrame["x-left"], autonSelectorFrame["y-top"], autonSelectorFrame["x-right"], autonSelectorFrame["y-bottom"]);

	// Gets and prints the color-coordinated current autonomous selection and whether the selection is locked or not in the frame
	pros::screen::set_pen(pros::Color::white);
	pros::screen::print(pros::E_TEXT_MEDIUM, 10, 200, "Auton: ");
	pros::screen::print(pros::E_TEXT_MEDIUM, 75, 200, to_string(autonSelector).c_str());
	pros::screen::set_pen(getColorFromValue(waitingForUserInput, USER_INPUT));

	if (waitingForUserInput)
	{
		pros::screen::print(pros::E_TEXT_MEDIUM, 10, 220, "UNLOCKED");
	}
	else
	{
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
void drawModeDisplayFrame()
{
	// Coordinates of the mode display frame
	std::map<std::string, int> modeDisplayFrame = {
		{"x-left", 98},
		{"x-right", 182},
		{"y-top", 189},
		{"y-bottom", 240}};

	// Draws a maroon rectangle at the above coordinates
	pros::screen::set_pen(pros::Color::maroon);
	pros::screen::fill_rect(modeDisplayFrame["x-left"], modeDisplayFrame["y-top"], modeDisplayFrame["x-right"], modeDisplayFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::draw_rect(modeDisplayFrame["x-left"], modeDisplayFrame["y-top"], modeDisplayFrame["x-right"], modeDisplayFrame["y-bottom"]);

	// Gets and prints the color-coordinated current game state in the frame
	pros::screen::set_pen(pros::Color::white);
	pros::screen::print(pros::E_TEXT_MEDIUM, 110, 200, "Mode: ");

	GameStates currentMode = getCompetitionStatus();
	pros::screen::print(pros::E_TEXT_MEDIUM, 160, 200, to_string(currentMode).c_str());
	if (currentMode == 0)
	{
		pros::screen::set_pen(getColorFromValue(to_string("PRE-AUTON"), GAME_STATE));
		pros::screen::print(pros::E_TEXT_MEDIUM, 100, 220, "PREAUTON");
	}
	else if (currentMode == 1)
	{
		pros::screen::set_pen(getColorFromValue(to_string("AUTON"), GAME_STATE));
		pros::screen::print(pros::E_TEXT_MEDIUM, 110, 220, "AUTON");
	}
	else if (currentMode == 2)
	{
		pros::screen::set_pen(getColorFromValue(to_string("USER"), GAME_STATE));
		pros::screen::print(pros::E_TEXT_MEDIUM, 110, 220, "USER");
	}
	else if (currentMode == 3)
	{
		pros::screen::set_pen(getColorFromValue(to_string("DISABLED"), GAME_STATE));
		pros::screen::print(pros::E_TEXT_MEDIUM, 100, 220, "DISABLED");
	}

	pros::screen::set_pen(pros::Color::white);
}

/**
 * @brief draws the GUI motor debug frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawMotorDebugFrame()
{
	// Coordinates of the motor debug frame
	std::map<std::string, int> motorDebugFrame = {
		{"x-left", 181},
		{"x-right", 479},
		{"y-top", 0},
		{"y-bottom", 190}};

	// Draws a maroon rectangle at the above coordinates
	pros::screen::set_pen(pros::Color::maroon);
	pros::screen::fill_rect(motorDebugFrame["x-left"], motorDebugFrame["y-top"], motorDebugFrame["x-right"], motorDebugFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::draw_rect(motorDebugFrame["x-left"], motorDebugFrame["y-top"], motorDebugFrame["x-right"], motorDebugFrame["y-bottom"]);

	// Gets and prints color-coordinated debug information for all the motors in the frame
	pros::screen::set_pen(pros::Color::white);
	pros::screen::print(pros::E_TEXT_MEDIUM, 250, 10, "Motor Information");
	std::vector<std::vector<std::string>> motorInfo = myMotorCollection.returnValues();
	std::vector<pros::Motor> motors = myMotorCollection.returnMotors();
	std::vector<bool> motorConnections = myMotorCollection.isConnected();

	int x;
	int y = 30;

	for (int i = 0; i < motorInfo.size(); i++)
	{
		x = 200;
		drawDashedLine(0, y - 4, 479, y - 4, 5, pros::Color::white);

		std::string motorCartridgeType;
		bool motorIsInstalled;
		int motorTemp;
		for (int j = 0; j < motorInfo[0].size(); j++)
		{
			switch (j)
			{
			case (0):

				if (motors[i].get_gearing() == pros::MotorGears::red || motors[i].get_gearing() == pros::MotorGears::green || motors[i].get_gearing() == pros::MotorGears::blue)
				{
					pros::screen::set_pen(pros::Color::light_green);
				}
				// else if(motors[i].get_gearing() == pros::MotorGears::green){
				// 	pros::screen::set_pen(pros::Color::gold);
				// }
				// else if(motors[i].get_gearing() == pros::MotorGears::blue){
				// 	pros::screen::set_pen(pros::Color::cyan);
				// }
				else
				{
					pros::screen::set_pen(pros::Color::red);
				}

				// pros::screen::set_pen(getColorFromValue(motorConnections[i], MOTOR_CONNECTION));
				break;
				// pros::screen::set_pen(pros::Color::light_green);

			case (1):

				if (motors[i].get_gearing() == pros::MotorGears::red)
				{
					motorCartridgeType = "Red";
				}
				else if (motors[i].get_gearing() == pros::MotorGears::green)
				{
					motorCartridgeType = "Green";
				}
				else if (motors[i].get_gearing() == pros::MotorGears::blue)
				{
					motorCartridgeType = "Blue";
				}
				else
				{
					motorCartridgeType = "ERR";
				}
				pros::screen::set_pen(getColorFromValue(motorCartridgeType, MOTOR_CARTRIDGE));
				break;

			case (2):
				pros::screen::set_pen(getColorFromValue(to_string(motorInfo[i][j].c_str()), MOTOR_TYPE));
				break;
			case (3):
				motorTemp = atoi(to_string(motors[i].get_temperature()).c_str());
				pros::screen::set_pen(getColorFromValue(motorTemp, MOTOR_TEMPERATURE));
				break;

			case (4):
				if (motorInfo[i][j].find("ERR") != -1)
				{
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
void drawBatteryInfoFrame()
{
	// Coordinates of the battery info frame
	std::map<std::string, int> batteryInfoFrame = {
		{"x-left", 181},
		{"x-right", 276},
		{"y-top", 189},
		{"y-bottom", 240}};

	// Draws a maroon rectangle at the above coordinates
	pros::screen::set_pen(pros::Color::maroon);
	pros::screen::fill_rect(batteryInfoFrame["x-left"], batteryInfoFrame["y-top"], batteryInfoFrame["x-right"], batteryInfoFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::draw_rect(batteryInfoFrame["x-left"], batteryInfoFrame["y-top"], batteryInfoFrame["x-right"], batteryInfoFrame["y-bottom"]);

	// Gets and prints the color-coordinated battery percent in the frame
	pros::screen::set_pen(pros::Color::white);
	pros::screen::print(pros::E_TEXT_MEDIUM, 200, 200, "Battery");
	pros::screen::set_pen(getColorFromValue((int)pros::battery::get_capacity(), BATTERY));
	pros::screen::print(pros::E_TEXT_MEDIUM, 210, 220, "%.2f%", pros::battery::get_capacity());
	pros::screen::set_pen(pros::Color::white);
}

/**
 * @brief draws the GUI controller info frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawControllerInfoFrame()
{
	// Coordinates of the controller info frame
	std::map<std::string, int> controllerInfoFrame = {
		{"x-left", 275},
		{"x-right", 410},
		{"y-top", 189},
		{"y-bottom", 240}};

	// Draws a maroon rectangle at the above coords
	pros::screen::set_pen(pros::Color::maroon);
	pros::screen::fill_rect(controllerInfoFrame["x-left"], controllerInfoFrame["y-top"], controllerInfoFrame["x-right"], controllerInfoFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::draw_rect(controllerInfoFrame["x-left"], controllerInfoFrame["y-top"], controllerInfoFrame["x-right"], controllerInfoFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::print(pros::E_TEXT_MEDIUM, 300, 200, "Controller");

	// Gets and prints the color-coordinated controller connection in the frame
	pros::screen::set_pen(getColorFromValue((bool)controller.is_connected(), CONTROLLER_CONNECTION));
	if (controller.is_connected())
	{
		pros::screen::print(pros::E_TEXT_MEDIUM, 300, 220, "CONNECTED");
	}
	else
	{
		pros::screen::print(pros::E_TEXT_MEDIUM, 300, 220, "DISCONNECTED");
	}

	pros::screen::set_pen(pros::Color::white);
}

/**
 * @brief draws the GUI team info frame
 * @relates drawGUI(), manuallyDrawGUI()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawTeamFrame()
{
	// Coordinates of the controller info frame
	std::map<std::string, int> teamFrame = {
		{"x-left", 409},
		{"x-right", 479},
		{"y-top", 189},
		{"y-bottom", 240}};

	// Draws a maroon rectangle at the above coords
	pros::screen::set_pen(pros::Color::maroon);
	pros::screen::fill_rect(teamFrame["x-left"], teamFrame["y-top"], teamFrame["x-right"], teamFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::draw_rect(teamFrame["x-left"], teamFrame["y-top"], teamFrame["x-right"], teamFrame["y-bottom"]);
	pros::screen::set_pen(pros::Color::white);
	pros::screen::print(pros::E_TEXT_MEDIUM, 420, 200, "Team");
	pros::screen::print(pros::E_TEXT_MEDIUM, 420, 220, "934Z");
}

/**
 * @brief Threaded function that calls the seven individual drawing functions to draw the entire GUI every 5 seconds
 * @relates initialize()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void drawGUI()
{
	while (true)
	{
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
 * @brief Non-threaded function that calls the seven individual drawing functions to draw the entire GUI
 * @relates autonSelection()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void manuallyDrawGUI()
{
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
void autonSelection()
{
	waitingForUserInput = true;
	while (waitingForUserInput)
	{
		if (pros::competition::is_connected() && !pros::competition::is_disabled())
		{
			waitingForUserInput = false;
			manuallyDrawGUI();
			break;
		}

		// Checks if the user presses the physical auton selection bumper
		if (autonSelectionBumper.get_value() == 1)
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
			pros::delay(200);
		}

		// Checks if the user presses the physical auton confirmation bumper
		if (autonConfirmationBumper.get_value() == 1)
		{
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
void motorTracking()
{
	while (true)
	{

		myMotorCollection.checkMotors();
		pros::delay(5000);
	}
}

/**
 * @brief Thread function that provides time updates to the controller at set intervals during user control
 * @relates opcontrol()
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void timeTracking()
{
	int timeCheck = 0;
	int time = 0;
	bool isH2H = false;

	while (true)
	{
		if (pros::competition::is_autonomous && !isH2H)
		{
			isH2H = true;
		}
		else if (pros::competition::is_field_control && isH2H)
		{
			controller.print(0, 0, "User Control - H2H");
			while (pros::competition::is_field_control)
			{
				time++;
				// Time update on controller at 1 minute, 30 seconds, and 10 seconds
				if (time >= 45 && timeCheck == 0)
				{
					controller.clear_line(0);
					controller.rumble(".");
					controller.print(0, 0, "1 Minute Remaining");
					timeCheck++;
				}
				else if (time >= 75 && timeCheck == 1)
				{
					controller.clear_line(0);
					controller.rumble(". .");
					controller.print(0, 0, "30 Seconds Remaining");
					timeCheck++;
				}
				else if (time >= 95 && timeCheck == 2)
				{
					controller.clear_line(0);
					controller.rumble(". . .");
					controller.print(0, 0, "10 Seconds Remaining");
					timeCheck++;
				}
				pros::delay(1000);
			}
		}
		else if (pros::competition::is_field_control && !isH2H)
		{
			controller.print(0, 0, "User Control - Skills");
			while (pros::competition::is_field_control)
			{
				time++;
				// Time update on controller at 45 seconds, 30 seconds, and 15 seconds
				if (time >= 15 && timeCheck == 0)
				{
					controller.clear_line(0);
					controller.rumble(".");
					controller.print(0, 0, "45 Seconds Remaining");
					timeCheck++;
				}
				else if (time >= 30 && timeCheck == 1)
				{
					controller.clear_line(0);
					controller.rumble(". .");
					controller.print(0, 0, "30 Seconds Remaining");
					timeCheck++;
				}
				else if (time >= 45 && timeCheck == 2)
				{
					controller.clear_line(0);
					controller.rumble(". . .");
					controller.print(0, 0, "15 Seconds Remaining");
					timeCheck++;
				}
				pros::delay(1000);
			}
		}
		pros::delay(1000);
	}
}

/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       VEX COMPETITION CONTROLLED FUNCTIONS                         */
/*                                                                                    */
/*  VEX competition controlled functions are those that are automatically called by   */
/*  VEX tournament management systems and should not be manually called               */
/*  Includes:                                                                         */
/*  - void initialize() - pre-game initializations, thread starting, auton selection  */
/*  - void disabled() - tasks to run when robot is disabled - NOT USED                */
/*	- void competition_initialize() - comp specific initializations - NOT USED        */
/*  - void autonomous() - 15 sec or 1min autonomous robot movement                    */
/*  - void opcontrol() - 1m45s loop of user-controlled robot movement                  /
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

/**
 * @brief Runs initialization code. This occurs as soon as the program is started.
 * @details All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void initialize()
{
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

	// Auton Selection
	autonSelector = 0;
	autonSelection();
}

/**
 * @brief Runs while the robot is disabled
 * @details Runs while the robot is in the disabled state of Field Management System
 * or the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
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
 * @details This function will be started in its own task with the default priority
 * and stack size whenever the robot is enabled via the Field Management System or
 * the VEX Competition Switch in the autonomous mode. Alternatively, this function
 * may be called in initialize or opcontrol for non-competition testing purposes.
 * If the robot is disabled or communications is lost, the autonomous task will be
 * stopped. Re-enabling the robot will restart the task, not re-start it from where
 * it left off.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void autonomous()
{
	if (autonSelector == 0)
	{
		// chassis.turnToHeading(180, 2000);

		chassis.moveToPoint(0, 25, 4000, {});
		armMotors.move_absolute(2400, 100);
		chassis.moveToPoint(0, 35, 4000, {}, false);
		armMotors.move_absolute(1400, 100);
		intakeMotors.move_relative(1500, 80);
		pros::delay(1000);
		chassis.moveToPoint(0, 19, 4000, {false}, false);
		// chassis.turnToHeading(205, 2000);
		// armMotors.move_absolute(0, 100);
		// chassis.moveToPoint(-15, 30, 4000, {});
		
		// chassis.moveToPoint(-25, 40, 4000, {});
		// intakeMotors.move_relative(-1500, 100);
		// chassis.moveToPoint(0, 0, 4000, {false}, false);
		// chassis.turnToHeading(250, 3000, {AngularDirection::CCW_COUNTERCLOCKWISE});
		// armMotors.move_absolute(2600, 100);
		// chassis.moveToPoint(-13, 0, 4000, {}, false);
		// chassis.turnToHeading(210, 3000, {AngularDirection::CCW_COUNTERCLOCKWISE});
		// chassis.moveToPoint(-20, -10, 2000, {}, false);
		// intakeMotors.move_relative(3000, 100);
		// pros::delay(300);
		// armMotors.move_absolute(1700, 100);
		// pros::delay(1000);
		// chassis.moveToPoint(-10, 0, 2000, {false}, false);
		// chassis.moveToPoint(-20, 5, 2000, {});
		// armMotors.move_absolute(0, 100);

		// clamp.set_value(true);
		// chassis.moveToPoint(5, 30, 4000);
		// chassis.turnToHeading(90, 2000, {AngularDirection::CCW_COUNTERCLOCKWISE});
	}
	else if (autonSelector == 1)
	{
		clamp.set_value(true);
		chassis.moveToPoint(0, 11, 2000, {}, false);
		chassis.turnToHeading(90, 2000);
		chassis.moveToPoint(-40, 11, 4000, {false}, false);
		chassis.turnToHeading(55, 2000, {AngularDirection::CCW_COUNTERCLOCKWISE});
		chassis.moveToPoint(-60, -10, 4000, {false}, false);
		chassis.moveToPoint(10, 22, 4000);
		chassis.turnToHeading(290, 2000);
		chassis.moveToPoint(70, -15, 4000, {false});
		chassis.moveToPoint(50, 10, 2000);
		chassis.turnToHeading(45, 2000);
		// chassis.moveToPoint(30, 30, 3000);
		// armMotors.move_absolute(4500, 100);
		// chassis.moveToPoint(20, 40, 2000);

		chassis.moveToPoint(50, 110, 2000);
		chassis.turnToHeading(300, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
		chassis.turnToHeading(110, 2000, {.direction = AngularDirection::CW_CLOCKWISE});
		chassis.moveToPoint(0, 130, 2000, {false});
		chassis.turnToHeading(52, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
		chassis.moveToPoint(-5, 110, 2000, {false}, false);
		chassis.moveToPoint(-40, 110, 2000, {false}, false);
		// chassis.turnToHeading(135, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
		chassis.moveToPoint(-70, 130, 4000, {false}, false);
		chassis.turnToHeading(270, 2000, {.direction = AngularDirection::CW_CLOCKWISE});
		chassis.moveToPoint(-40, 130, 4000, {false}, false);
		chassis.turnToHeading(285, 2000, {.direction = AngularDirection::CW_CLOCKWISE});
		chassis.moveToPoint(10, 110, 4000, {false}, false);
		chassis.turnToHeading(265, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
		chassis.moveToPoint(60, 140, 2000, {false}, false);
		armMotors.move_absolute(4000, 100);
		chassis.moveToPoint(-5, 75, 2000, {}, false);
		chassis.moveToPoint(-10, 70, 2000, {});
		armMotors.move_absolute(0, 100);

		// chassis.turnToHeading(90, 2000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
		// chassis.moveToPoint(-60, 120, 4000, {false}, false);
		// // clamp.set_value(false);
		// chassis.moveToPoint(-15, 104, 2000, {false}, false);
		// chassis.moveToPoint(-25, 90, 2000, {false});
		// clamp.set_value(true);

	}
}

/**
 * @brief Runs the operator control code.
 * @details This function will be started in its own task with the default priority
 * and stack size whenever the robot is enabled via the Field Management System or
 * the VEX Competition Switch in the operator control mode. If no competition control
 * is connected, this function will run immediately following initialize().
 * If the robot is disabled or communications is lost, the operator control task will
 * be stopped. Re-enabling the robot will restart the task, not resume it from where
 * it left off.
 * @author Leo Abubucker
 * @date 12/01/2024
 */
void opcontrol()
{
	bool clampState = false;
	bool clampLastState = false;
	pros::Task timeTrackingTask(timeTracking);

	while (true)
	{ // Run for 20 ms then update
		leftMotors.move(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
		rightMotors.move(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

		// Intake Controls

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			intakeMotors.move_velocity(-200);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			intakeMotors.move_velocity(200);
		}
		else
		{
			intakeMotors.brake();
		}

		// Arm Controls

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
		{
			// Position to go to the top of a neutral stake - DOES NOT WORK
			armMotors.move_absolute(720, 100);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
		{
			// Position to reset the arm
			armMotors.move_absolute(0, 100);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
		{
			armMotors.move_velocity(100);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			armMotors.move_velocity(-100);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
		{
			// Position to go to the top of an alliance wall stake
			armMotors.move_absolute(2400, 100);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
		{
			// Position to go to the top of an alliance wall stake
			armMotors.move_absolute(2400, 100);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) || controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
		{
			// Position to go to climb position
			armMotors.move_absolute(4500, 100);
		}
		else
		{
			armMotors.brake();
		}

		// Clamp Controls
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !clampLastState)
		{
			clampState = !clampState;
			clampLastState = true;
		}
		else if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))
		{
			clampLastState = false;
		}

		if (clampState)
		{
			clamp.set_value(true);
		}
		else
		{
			clamp.set_value(false);
		}
		pros::delay(20);
	}
}