#include <iostream>
#include <tuple>
#include "special_prog.h"
#include "vex.h"
using namespace vex;

double sensor_1_360 = 50.0;
double sensor_2_360 = 82.0;
double sensor_1_conv = 360 / sensor_1_360;
double sensor_2_conv = 360 / sensor_2_360;

std::tuple<double, double> pos_to_360 = std::make_tuple(sensor_1_360, sensor_2_360);

std::tuple<double, double> pos_to_deg(double sensor_1_pos, double sensor_2_pos) {
    double sensor_1 = sensor_1_pos * sensor_1_conv;
    double sensor_2 = sensor_2_pos * sensor_2_conv;
    return std::make_tuple(sensor_1, sensor_2);
}

std::tuple<double, double> deg_to_pos(double sensor_1_deg, double sensor_2_deg) {
    double sensor_1 = sensor_1_deg / sensor_1_conv;
    double sensor_2 = sensor_2_deg / sensor_2_conv;
    return std::make_tuple(sensor_1, sensor_2);
}
void pos_to_deg_program() {
    Brain.Screen.clearScreen();
    Brain.Screen.print("Positions equal to 360 Degrees:");
    int sensor1_print = std::get<0>(pos_to_360);
    Brain.Screen.print("Sensor 1: ");
    Brain.Screen.print(sensor1_print);
    int sensor2_print = std::get<1>(pos_to_360);
    Brain.Screen.print("Sensor 2: ");
    Brain.Screen.print(sensor2_print);

    double sensor_1_pos = sensor1.position(vex::rotationUnits::deg);
    double sensor_2_pos = sensor2.position(vex::rotationUnits::deg);
    std::tuple<double, double> conv_pos = pos_to_deg(sensor_1_pos, sensor_2_pos);
    Brain.Screen.clearScreen();
    Brain.Screen.print("Degree value of Sensor 1 with a position of ");
    Brain.Screen.print(sensor_1_pos);
    Brain.Screen.print(" is ");
    int sensor1_print_2 = std::get<0>(conv_pos);
    Brain.Screen.print(sensor1_print_2);
    Brain.Screen.print("Degree value of Sensor 2 with a position of ");
    Brain.Screen.print(sensor_2_pos);
    Brain.Screen.print(" is ");
    int sensor2_print_2 = std::get<1>(conv_pos);
    Brain.Screen.print(sensor2_print_2);
}
