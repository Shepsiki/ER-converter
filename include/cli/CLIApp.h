#pragma once

#include "cli/Handler.h"

namespace erconv {
    
class CLIApp {
public:
    CLIApp(IHandler& hnd);
    void Start(std::istream& in, std::ostream& out);

private:
    IHandler& handler;
};

} // namespace erconv
