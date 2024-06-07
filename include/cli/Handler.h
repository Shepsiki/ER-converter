#pragma once

#include "cli/Command.h"

#include <ostream>

namespace erconv {

class IHandler {
public:
    virtual void Execute(const Command& args, std::ostream& out) = 0;
};

} // namespace erconv
