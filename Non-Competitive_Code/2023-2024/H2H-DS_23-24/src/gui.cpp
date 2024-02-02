/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*                       Global Initializations & References                          */
/*                                                                                    */
/*  Necessary #include and using calls, and initializations of global variables       */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

#include "vex.h" //VEX functions and subsequent call to robot-config.h
#include "gui.h" //GUI functions
using namespace vex; //VEX References
#include <map> //std::map
#include <string> //std::string

// /*------------------------------------------------------------------------------------*/
// /*                                                                                    */
// /*                              Auton Selector Function                               */
// /*                                                                                    */
// /*  Creates a GUI to allow the user to choose between two possible auton programs     */
// /*  by clikcing on the correctly-labeled button. Returns str with choice to main      */
// /*  autonomous function for use. Returns "" str correlating to skipping auton if      */
// /*  no input is provided.                                                             */
// /*                                                                                    */
// /*------------------------------------------------------------------------------------*/

// std::string autonSelector(){
//     /*------------------------------------------------------------------------------------*/
//     /*                                                                                    */
//     /*                              Function Initializations                              */
//     /*                                                                                    */
//     /*  Display of title and declartion of variables for future use.                      */
//     /*                                                                                    */
//     /*------------------------------------------------------------------------------------*/

//     Brain.Screen.printAt(150, 20, "VRC GUI vDEV - Auton Selector"); //Prints title at (150, 20) to brain
//     bool waitingForUserInput = true; //Init bool to run while loop while user input hasn't been received
//     int localLastTouchX = -1; //Init int to log the x-coordinate where the user last touched the Brain's screen
//     int localLastTouchY = -1; //Init int to log the y-coordinate where the user last touched the Brain's screen
//     std::string response = ""; //Init str to hold the auton selection that is returned to main.cpp's autonomous function  
    
//     /*------------------------------------------------------------------------------------*/
//     /*                                                                                    */
//     /*                              R2B1 Auton Btn                                        */
//     /*                                                                                    */
//     /*  Creates a labeled button at predefined coordinates to allow the user to           */
//     /*  select the R2B1 Auton program.                                                    */
//     /*                                                                                    */
//     /*------------------------------------------------------------------------------------*/

//     //Coordinates of the R2B1 Auton Btn
//     std::map<std::string, int> r2b1AutonBtnCoords = {
//         {"x-left", 0},
//         {"x-right", 225},
//         {"y-top", 25},
//         {"y-bottom", 240}
//     };
//     //Creates red Btn at previously declared coords
//     Brain.Screen.setFillColor(color::red);
//     Brain.Screen.drawRectangle(r2b1AutonBtnCoords["x-left"], r2b1AutonBtnCoords["y-top"], r2b1AutonBtnCoords["x-right"] - r2b1AutonBtnCoords["x-left"], r2b1AutonBtnCoords["y-bottom"] - r2b1AutonBtnCoords["y-top"], color::red);
    
//     //Prints R2B1 Auton in the middle of created btn
//     Brain.Screen.setCursor(6, 4);
//     Brain.Screen.print("Our Offensive Zone (r2b1) Auton");

//     /*------------------------------------------------------------------------------------*/
//     /*                                                                                    */
//     /*                              R1B2 Auton Btn                                        */
//     /*                                                                                    */
//     /*  Creates a labeled button at predefined coordinates to allow the user to           */
//     /*  select the R1B2 Auton program.                                                    */
//     /*                                                                                    */
//     /*------------------------------------------------------------------------------------*/

//     //Coordinates of the R1B2 Auton Btn
//     std::map<std::string, int> r1b2AutonBtnCoords = {
//         {"x-left", 225},
//         {"x-right", 450},
//         {"y-top", 25},
//         {"y-bottom", 240}
//     };

//    //Creates green Btn at previously declared coords
//     Brain.Screen.setFillColor(color::green);
//     Brain.Screen.drawRectangle(r1b2AutonBtnCoords["x-left"], r1b2AutonBtnCoords["y-top"], r1b2AutonBtnCoords["x-right"] - r1b2AutonBtnCoords["x-left"], r1b2AutonBtnCoords["y-bottom"] - r1b2AutonBtnCoords["y-top"], color::green);
   
//     //Prints R1B2 Auton in the middle of created btn
//     Brain.Screen.setCursor(6, 26);
//     Brain.Screen.print("Opposing Offensive Zone (r1b2) Auton");


//     /*------------------------------------------------------------------------------------*/
//     /*                                                                                    */
//     /*                         User Input Detection Logic                                 */
//     /*                                                                                    */
//     /*  Detects and interprets user input in deciding appropriate auton selection.        */
//     /*                                                                                    */
//     /*------------------------------------------------------------------------------------*/

//     while(waitingForUserInput){
//         //Logs the last x and y position of the user's touch to the localLastTouchX, localLastTouchY vars respectively
//         if(Brain.Screen.pressing()){
//             localLastTouchX = Brain.Screen.xPosition();
//             localLastTouchY = Brain.Screen.yPosition();
//         }
//         //Checks if the user touches the R2B1 Auton Btn
//         if(localLastTouchX >= r2b1AutonBtnCoords["x-left"] && localLastTouchX <= r2b1AutonBtnCoords["x-right"] && localLastTouchY >= r2b1AutonBtnCoords["y-top"] && localLastTouchY <= r2b1AutonBtnCoords["y-bottom"]){
//             waitingForUserInput = false;
//             response =  "r2b1Auton";
//             Brain.Screen.clearScreen();
//             Brain.Screen.printAt(1,1, "Our Offensive Zone (r2b1) Autonomous Programm Selected.");
//         }
//         //Checks if user touches the R1B2 Auton Btn
//         else if(localLastTouchX >= r1b2AutonBtnCoords["x-left"] && localLastTouchX <= r1b2AutonBtnCoords["x-right"] && localLastTouchY >= r1b2AutonBtnCoords["y-top"] && localLastTouchY <= r1b2AutonBtnCoords["y-bottom"]){
//             waitingForUserInput = false; 
//             response = "r1b2Auton";
//             Brain.Screen.clearScreen();
//             Brain.Screen.printAt(1,1, "Opposing Offensive Zone (r1b2) Autonomous Programm Selected.");
//         }
//         wait(20, msec); // Cooldown to prevent excess CPU usage and subsequent brain crashing
//         }
//     return response; //Returns auton selection
//     }

void drawGUI(MotorCollection myMotorCollection, std::string matchState){
    Brain.Screen.setCursor(1,1);
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.clearScreen();
    std::vector<bool> motorConnections = myMotorCollection.isConnected();
    int xLeft = 0;
    int yTop = 20;
    int width = 20;
    int height = 53;
    int row = 3;
    int col = 4;
    for(int i=0; i<motorConnections.size(); i++){
        if(motorConnections[i] == true){
            Brain.Screen.drawRectangle(xLeft, yTop, width, height, color::green);
        }
        else{
            Brain.Screen.drawRectangle(xLeft, yTop, width, height, color::red);
        }
        Brain.Screen.printAt(col, row, myMotorCollection.motorNamesList[i].c_str());
        
    //     yTop += height;
    //     if(i%4 == 0){
    //         xLeft = 40;
    //         col *=3;
    //         yTop = 20;
    //     }
    // }

    // int col2 = 15;
    // for (std::string x : myMotorCollection.returnPositions()){
    //     Brain.Screen.printAt(col2, 3, (x + " Deg").c_str());
    //     col2+=3;
    // }
    // col2 = 15;
    // for (std::string x : myMotorCollection.returnVelocities()){
    //     Brain.Screen.printAt(col2, 5, (x + " %").c_str());
    //     col2+=3;
    // }
    // col2 = 15;
    // for (std::string x : myMotorCollection.returnTorque()){
    //     Brain.Screen.printAt(col2, 7, (x + " Nm").c_str());
    //     col2+=3;
    // }
    // col2 = 15;
    // for (std::string x : myMotorCollection.returnTemperatures()){
    //     Brain.Screen.printAt(col2, 9, (x + " %").c_str());
    //     col2+=3;
    }   
    
    Brain.Screen.printAt(20, 11, ("Current Drive Mode: "));
    std::string currentDriveMode = myMotorCollection.checkDriveMotors();
    if(currentDriveMode != "fourWheel"){
        Brain.Screen.setPenColor(color::green);
    }
    else{
        Brain.Screen.setPenColor(color::red);
    }
    Brain.Screen.printAt(30, 11, (currentDriveMode).c_str());
    Brain.Screen.setPenColor(color::white);
    if(matchState == "NO AUTON"){
        Brain.Screen.setPenColor(color::red);
    }
    Brain.Screen.printAt(38, 11, matchState.c_str());
}