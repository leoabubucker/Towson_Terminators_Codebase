#include <iostream> // Used solely in the concatPrint() function

void myFunc(int param1, bool param2){
    // Code to be executed
}

double addNums(double num1, double num2){
    return num1 + num2;
}

bool isGreater(int num1, int num2){
    if(num1 > num2){
        return true;
    }
    return false;
}

void concatPrint(std::string str1, std::string str2){
    std::cout << str1 + str2 << std::endl; //Just prints str1+str2 with a new line at the end
}

void printHello(){
    std::cout << "Hello" << std::endl; // Just prints "Hello" with a new line at the end
}

int main(){ // Remember the main method is called automatically at the start, don't call it manually
    double sum = addNums(1.5, 3.4); // Stores 4.9 in "sum" variable
    bool greater = isGreater(5, 2); // Stores true in "greater" variable
    std::cout << addNums(1.5, 3.4) << std::endl; //Just prints the returned value of addNums(1.5, 3.4) with a new line at the end
    std::cout << isGreater(5, 2) << std::endl; //Just prints the returned value of isGreater(5, 2) with a new line at the end
    concatPrint("Hello", "World"); // Calls the concatPrint() method, passing in the std::string parameters “Hello” and “World"
    printHello(); // Calls the printHello() method, passing in no parameters

}