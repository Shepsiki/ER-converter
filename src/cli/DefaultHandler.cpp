#include "cli/DefaultHandler.h"

namespace erconv {

void DefaultHandler::Execute(const Command& args, std::ostream& out) {
    // Здесь вся обработка запросов.
    // Dummy code:
    for (const auto& arg : args) {
        out << arg << " ";
    }
    out << std::endl;
}

} // namespace erconv
