#ifndef AUTON_H //Header File Guard
#define AUTON_H //Header File Guard

using namespace vex; //VEX References
#include <string> //std::string

//Autonomous Turn Function Declaration - See auton.cpp for full explanation and implementation
extern void turn(int degrees, std::string direction, int velocity = 50);

//Autonomous Drive Function Declaration - See auton.cpp for full explanation and implementation
extern void drive(int inches, std::string direction, int velocity = 50);

#endif //Header File Guard