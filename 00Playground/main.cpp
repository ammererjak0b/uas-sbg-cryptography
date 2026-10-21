#include "Greeter/Greeter.hpp"

int main(int argc, char* argv[]) {
    std::string inputName = "World";      // fallback if args.txt is empty
    if (argc > 1) {
        inputName = argv[1];              // first arg comes from args.txt
    }

    Greeter greeter(inputName);
    greeter.PrintGreeting();

    return 0;
}
