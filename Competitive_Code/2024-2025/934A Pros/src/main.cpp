#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep


//controller
pros::Controller controller (pros::E_CONTROLLER_MASTER);


//motors
pros::Motor interalIntake(-5, pros::v5::MotorGears::green);
pros::Motor externalIntake(6, pros::v5::MotorGears::green);
pros::MotorGroup rightMotors({3, 11}, pros::v5::MotorGears::green);
pros::MotorGroup leftMotors({-1, -2}, pros::v5::MotorGears::green);
pros::Motor fishMech(20, pros::v5::MotorGears::green);


//inertial sensor (port 10)
pros::Imu inertial(10);


pros::adi::DigitalOut clamp('A');


//drivetrain settings
lemlib::Drivetrain drivetrain{&leftMotors, &rightMotors, 13, lemlib::Omniwheel::OLD_275, 266 , 2};
//sensors for odom
lemlib::OdomSensors sensors(nullptr, nullptr, nullptr, nullptr, &inertial);


// lateral PID controller
lemlib::ControllerSettings lateral_controller(80, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              5, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              127 // maximum acceleration (slew)
);


// angular PID controller
lemlib::ControllerSettings angular_controller(15, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              55, // derivative gain (kD)
                                              0, // anti windup 252.1965
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);


//input curve for throttle input during driver control (deadband, min output required, expo curve gain)
lemlib::ExpoDriveCurve throttle_curve(3, 10, 1.019);


// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, 10, 1.019);


// create the chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller,  sensors, &throttle_curve, &steer_curve);




/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
   
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensor


    //REMOVE, USED FOR AUTON TESTING
    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // delay to save resources
            pros::delay(20);
        }
    });
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

    // DRIVER NOTE REMOVE LATER
    // SETUP AUTON WITH CLAMP BRACKET LEFT SIDE PARRALEL TO 1ST REAL SQUARE OF TILE AND PUSH A LITTLE BACK 
   
    //HEAD TO HEAD BLUE RIGHT SIDE (RING RUSH)
   /*
    chassis.setPose(0,0,0);
    clamp.set_value(true);
    pros::delay(500);
    chassis.moveToPoint(0, -24, 3000, {.forwards = false, .maxSpeed = 60}, false);
    clamp.set_value(false);
    pros::delay(1000);
    interalIntake.move(127);
    externalIntake.move(127);
    pros::delay(700);
    chassis.turnToHeading(290, 1000);
    chassis.moveToPoint(-23, -18, 2000, {.forwards = true}, false);
	pros::delay(500);
	chassis.turnToHeading(185, 1000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
	chassis.moveToPoint(-23, -28, 2000, {.forwards = true}, false);
	chassis.turnToHeading(195, 1000);
	*/

	//HEAD TO HEAD BLUE LEFT SIDE
	/*
	chassis.setPose(0,0,0);
    clamp.set_value(true);
    pros::delay(500);
    chassis.moveToPoint(0, -24, 3000, {.forwards = false, .maxSpeed = 60}, false);
	chassis.moveToPoint(0, -28, 1000, {.forwards = false}, true);
    clamp.set_value(false);
    pros::delay(1000);
    interalIntake.move(127);
    externalIntake.move(127);
    pros::delay(700);
    chassis.turnToHeading(90, 1000);
    chassis.moveToPoint(23, -18, 2000, {.forwards = true}, false);
	*/

    ///CURRENT WORKING AUTON
    
    //chassis.setPose(0, 0, 0);
	
    interalIntake.move(127);
    pros::delay(1500);
    interalIntake.move(0);
	clamp.set_value(true);
    chassis.moveToPoint(0, 8, 3000, {.forwards = true}, false);
    chassis.turnToHeading(270, 3000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE}, false);
	chassis.moveToPoint(15, 8, 1000, {.forwards = false, .maxSpeed = 80}, false);
	chassis.moveToPoint(18, 8, 1000, {.forwards = false}, true);
	clamp.set_value(false);
	externalIntake.move(127);
	interalIntake.move(127);
	pros::delay(1000);
	chassis.moveToPoint(65, 8, 4000, {.forwards = true}, false);
    //chassis.moveToPoint(45, -10, 2000, {.forwards = false}, false);
    //chassis.turnToHeading(320, 3000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE}, false);
    //chassis.moveToPoint(45, -25, 1000,{.forwards = false}, false);
    //chassis.moveToPoint(0, 14, 3000, {.forwards = true}, false);
    //chassis.turnToHeading(50, 3000, {}, false);
    //chassis.moveToPoint(-25, 2, 3000, {.forwards = false}, false);
    //chassis.moveToPoint(-35, -7, 3000, {.forwards = false}, false);
    //chassis.turnToHeading(140, 3000, {}, false);
    //chassis.moveToPoint(-80, 26, 3000, {.forwards = false}, false);
    //chassis.turnToHeading(180, 3000, {}, false);
    //chassis.moveToPoint(-80, 50 , 3000, {.forwards = false}, false);
    
   
   
    //chassis.moveToPoint(0, 0,  3000, {.forwards = false}, false);
   
    //chassis.moveToPoint(40, 5, 3000,{.forwards = true}, false);
    //chassis.turnToHeading(50, 2000);
    //chassis.moveToPoint(-79, -20, 5000, {.forwards = false}, false);
   
   
    //chassis.moveToPose(11,12,-55, 2000);  
    //chassis.turnToHeading(-55, 2000);
    //chassis.moveToPoint(48, 0, 3000, {.forwards = false});
    //chassis.moveToPose(38, -16, -29, 3000);
    //chassis.moveToPoint(0, -5, 1000, {.forwards = false});
    //chassis.turnToHeading(90, 2000);
    //externalIntake.move(110);
    //chassis.moveToPoint(0, 26, 2000, {.forwards = true});
    //pros::delay(2000);
    //chassis.moveToPoint(/*test negative*/-26, -26, 2000, {.forwards = false});
    //interalIntake.move(0);
    //externalIntake.move(0);
   
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
 * If the robot is disabledmunications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
   
    while (true) {


        /*      DRIVE CONTROLS
            Left joystick for forward and backward
            RIght joystick for turning
        */
        int rightJoystickTurn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int leftJoysticForward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);


        chassis.arcade(leftJoysticForward, rightJoystickTurn, false, 0.6);


        /*      INTAKE CONTROLS
            R1 and R2 for forward
            L1 and L2 for reverse
        */
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)||controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            interalIntake.move(127);
            externalIntake.move(127);
        }else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) || controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        {
            interalIntake.move(-127); //-80
            externalIntake.move(-127); //-110
        }else{
            interalIntake.move(0);
            externalIntake.move(0);
        }


        /*      CLAMP CONTROLS
            X to extend (open)
            B to retract (close)
        */
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
            clamp.set_value(true);
        }else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
            clamp.set_value(false);
        }


        /*FISH MECH*/

        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
            fishMech.move(127);
        }else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
            fishMech.move(-127);
        }else{
            fishMech.move(0);


        }


        pros::delay(20);                            
    }
}


