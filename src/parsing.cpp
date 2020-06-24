#include "parsing.hpp"

void DEBUG_STDOUT(const std::string& msg) {
    if (DEBUG) std::cout << msg << std::endl;
}
void DEBUG_STDERR(const std::string& msg) {
    if (DEBUG) std::cerr << msg << std::endl;
}

std::vector<std::string> parsing_utils::split_string(const std::string &s, char delim) {       // IS_TESTED
    std::stringstream ss(s);
    std::vector<std::string> elems{};
    std::string item{};
    while (std::getline(ss, item, delim)) {
        if (item.length() > 0) {
            elems.emplace_back(item);
        }
    }
    return elems;
}

std::tuple<std::string, uint16_t, std::string, std::string> parsing_utils::split_proc_info(const std::string &proc_info) {      // IS_TESTED
    auto elems = split_string(proc_info, ' ');
    return std::make_tuple(elems[0], std::stoi(elems[1]), elems[10], elems[11]);
}

std::string parsing_utils::parse_strace_line(const std::string &output, boost::regex &xRegEx) {     // IS_TESTED
    boost::smatch xResults{};
    if (boost::regex_search(output, xResults, xRegEx)) {
        if (xResults["cmd"].matched) {
            if (xResults["cmd"] == "\\r") return "[ENTER]\n";                  // 8-system
            else if (xResults["cmd"] == "\\177") return "[BACKSPACE]";
            else if (xResults["cmd"] == "\\t") return "[TAB]";
            else if (xResults["cmd"] == "\\33") return "[ESC]";
            else if ((xResults["cmd"] == "\\33[A") || (xResults["cmd"] == "\\33OA")) return "[UP]";
            else if ((xResults["cmd"] == "\\33[B") || (xResults["cmd"] == "\\33OB")) return "[DOWN]";
            else if ((xResults["cmd"] == "\\33[C") || (xResults["cmd"] == "\\33OC")) return "[RIGHT]";
            else if ((xResults["cmd"] == "\\33[D") || (xResults["cmd"] == "\\33OD")) return "[LEFT]";
            else if (xResults["cmd"] == "\\33[2~") return "[INSERT]";
            else if (xResults["cmd"] == "\\33[3~") return "[DELETE]";
            else if (xResults["cmd"] == "\\33[5~") return "[PageUP]";
            else if (xResults["cmd"] == "\\33[6~") return "[PageDOWN]";
            else if ((xResults["cmd"] == "\\33[H") || xResults["cmd"] == "\\33OH") return "[HOME]";
            else if ((xResults["cmd"] == "\\33[F") || xResults["cmd"] == "\\33OF") return "[END]";
            else if (xResults["cmd"] == "\\3") return "[Ctrl+C]^C\n";
            else if (xResults["cmd"] == "\\4") return "[Ctrl+D]^D\n";
            else return xResults["cmd"];
        }
    }
    return "[UNDEF]";
}
