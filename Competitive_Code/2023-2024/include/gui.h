#ifndef GUI_H //Header File Guard
#define GUI_H //Header File Guard

using namespace vex; //VEX References
#include <string> //std::string

//Main GUI Drawing Function Declaration - See gui.cpp for full explanation and implementation
extern void drawGUI(MotorCollection myMotorConnection, std::string matchState);

//Auton Selection GUI Drawing Function Declaration - See gui.cpp for full explanation and implementation
extern std::string autonSelector();

#endif //Header File Guard