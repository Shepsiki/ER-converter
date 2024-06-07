#include "cli/CLIApp.h"

namespace erconv
{

CLIApp::CLIApp(IHandler& hnd) : handler(hnd) { }

void CLIApp::Start(std::istream& in, std::ostream& out) {
    std::string line;    
    out << "$ ";
    while (std::getline(in, line)) {
        Command cmd = Parse(line);
        if (cmd.size() == 0) {
            out << "$ ";
            continue;
        }
        if (cmd[0] == "exit") {
            break;
        }
        handler.Execute(cmd, out);
        out << "$ ";
    }
}

} // namespace erconv
