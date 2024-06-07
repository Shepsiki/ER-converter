#include "cli/Command.h"

namespace erconv {
    
Command Parse(const std::string& line) {
    Command result;
    std::stringstream ss(line);
    std::string cur;
    while (ss >> cur) {
        result.push_back(cur);
    }
    return result;
}

} // namespace erconv


