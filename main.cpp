#include "cli/DefaultHandler.h"
#include "cli/CLIApp.h"

#include <iostream>

using namespace erconv;

int main() {
    DefaultHandler handler;
    CLIApp app(handler);
    
    app.Start(std::cin, std::cout);

    return 0;
}
