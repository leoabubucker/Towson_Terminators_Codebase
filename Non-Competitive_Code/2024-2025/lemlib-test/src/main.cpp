#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup allMotors({-15, -11, 14, 13, -10, 2, -9, 1});

pros::MotorGroup leftMotors({-15, -11},
                            pros::MotorGearset::green); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({14, 13}, pros::MotorGearset::green); // right motor group - ports 6, 7, 9 (reversed)

pros::MotorGroup intakeMotors({-10, 2}, pros::MotorGearset::green);

pros::MotorGroup armMotors({-9, 1}, pros::MotorGearset::red);

// Inertial Sensor on port 20
pros::Imu imu(20);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	// pros::lcd::initialize();
	// pros::lcd::set_text(1, "Hello PROS User!");
	//   pros::Motor drive_left_initializer (PORT1, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
	// pros::lcd::register_btn1_cb(on_center_button);
	leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	intakeMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	armMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	allMotors.tare_position();

	chassis.calibrate(); // calibrate sensors
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	    // Move to x: 20 and y: 15, and face heading 90. Timeout set to 4000 ms
    chassis.moveToPose(20, 15, 90, 4000);
    // Move to x: 0 and y: 0 and face heading 270, going backwards. Timeout set to 4000ms
    chassis.moveToPose(0, 0, 270, 4000, {.forwards = false});
    // cancel the movement after it has traveled 10 inches
    chassis.waitUntil(10);
    chassis.cancelMotion();
    // Turn to face the point x:45, y:-45. Timeout set to 1000
    // dont turn faster than 60 (out of a maximum of 127)
    chassis.turnToPoint(45, -45, 1000, {.maxSpeed = 60});
    // Turn to face a direction of 90º. Timeout set to 1000
    // will always be faster than 100 (out of a maximum of 127)
    // also force it to turn clockwise, the long way around
    chassis.turnToHeading(90, 1000, {.direction = AngularDirection::CW_CLOCKWISE, .minSpeed = 100});
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	while (true) {                           // Run for 20 ms then update
	}

	leftMotors.move(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X));
	rightMotors.move(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

	// Intake Controls

	if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
		intakeMotors.move_velocity(-200);	
	}
	else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
		intakeMotors.move_velocity(200);	
	}
	else{
		intakeMotors.brake();
	}

	// Arm Controls

	if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
		armMotors.move_absolute(720, 100);
	}
	else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
		armMotors.move_absolute(0, 100);
	}
	else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
		armMotors.move_velocity(100);
	}
	else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
		armMotors.move_velocity(-100);
	}
	else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
		armMotors.move_absolute(600, 100);
	}
	else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
		armMotors.move_absolute(320, 100);
	}
	else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) || controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
		armMotors.move_absolute(970, 100);
	}
	else{
		armMotors.brake();
	}
}