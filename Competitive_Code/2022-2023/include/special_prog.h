#include <tuple>
// #pragma once
// #ifndef HEADER_H_INCLUDED
// #define HEADER_H_INCLUDED
extern std::tuple<double, double> pos_to_deg(double sensor_1_pos, double sensor_2_pos);
extern std::tuple<double, double> pos_to_360;
extern std::tuple<double, double> deg_to_pos(double sensor_1_deg, double sensor_2_deg);
extern double sensor_1_360;
extern double sensor_2_360;
extern void pos_to_deg_program();
// #endif