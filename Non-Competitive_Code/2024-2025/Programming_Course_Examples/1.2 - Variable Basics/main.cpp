#include <iostream> // This include statement is necessary to use std::string and to print using std::cout
#include <map> // This include statement is necessary to use std::map
int globalVar;
int main(){ // This is the main method. The main method is where code execution always starts and must be declared exactly like this
    int localVar;
    // Code in here can access both globalVar and localVar
    globalVar = 4; // This works
    localVar = 3; // This also works
    std::string myTextVariable = "hello";
    int myNumberVariable = 1234;
    bool myBoolVariable = true;
    double myDoubleVariable = 1.234567;
    float myFloatVariable = 8.876567;
    std::map<std::string, int> myMap = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
    };

    int declaredInt;
    std::map<std::string, int> declaredMap;

    declaredInt = 5;
    declaredMap = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
    };

    int initializedInt = 5;
    std::map<std::string, int> initializedMap = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
    };

    initializedInt = 24;
    initializedMap = {
        {"four", 4},
        {"five", 5},
        {"six", 6},
    };
}

void otherMethod(){
    // Code in here can access globalVar but cannot access localVar
    globalVar = 4; // This works
    // localVar = 3; // This throws an error so it's commented out
}