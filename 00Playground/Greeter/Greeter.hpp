#pragma once
#include <string>

class Greeter {
public:
    Greeter(const std::string& name);      // stores the name to greet
    void PrintGreeting();                  // prints "Hello World, <name>!"

private:
    std::string userName;
};
