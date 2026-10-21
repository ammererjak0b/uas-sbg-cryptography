#include "Greeter.hpp"
#include <iostream>

Greeter::Greeter(const std::string& name) : userName(name) {
}

void Greeter::PrintGreeting() {
    std::cout << "Hello World, " << userName << "!" << std::endl;
}
