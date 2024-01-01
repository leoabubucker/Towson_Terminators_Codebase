using namespace vex;

// extern bool enableDrivePID;
// extern int drivePID();

#include <string>

extern void turn(int degrees, std::string direction, int velocity = 50);

extern void drive(int inches, std::string direction, int velocity = 50);