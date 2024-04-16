#pragma once

#include "cli/Handler.h"

namespace erconv {
    
class DefaultHandler : public IHandler {
public:
    void Execute(const Command& args, std::ostream& out) override;

private:

};

} // namespace erconv
