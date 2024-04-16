#pragma once

#include <sstream>
#include <vector>
#include <string>

namespace erconv {

using Command = std::vector<std::string>;

Command Parse(const std::string& line);

} // namespace erconv
