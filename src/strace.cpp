#include "strace.hpp"


std::string logger::Strace::find_key_in_strace_line(const std::string &data_from_strace, boost::regex &xRegEx, const std::string &sys_call)
{
    std::string symbol{};
    if ((data_from_strace.find(sys_call + "(") != std::string::npos &&
        ((data_from_strace.find(", 1)") != std::string::npos &&
            data_from_strace.find("= 1\n") != std::string::npos) ||
        (data_from_strace.find(", 3)") != std::string::npos &&
            data_from_strace.find("= 3\n") != std::string::npos)||
        (data_from_strace.find(", 4)") != std::string::npos &&
         data_from_strace.find("= 4\n") != std::string::npos))) ||

        (data_from_strace.find(sys_call + "(") != std::string::npos &&
         ((data_from_strace.find(", 16384)") != std::string::npos &&
           data_from_strace.find("= 1\n") != std::string::npos) ||
          (data_from_strace.find(", 16384)") != std::string::npos &&
           data_from_strace.find("= 3\n") != std::string::npos)||
          (data_from_strace.find(", 16384)") != std::string::npos &&
           data_from_strace.find("= 4\n") != std::string::npos))))
    {
        DEBUG_STDOUT(data_from_strace);
        symbol = parsing_utils::parse_strace_line(data_from_strace, xRegEx);
    }
    return symbol;
}

void logger::Strace::ssh_outgoing_logger(const pid_t& pid) {
    DEBUG_STDOUT("Handling process(outgoing SSH) " + std::to_string(pid));
    std::string strace_cmd= "strace -s 16384 -p " + std::to_string(pid) + " -e read 2>&1";
    FILE* pipe = popen(strace_cmd.c_str(), "r");
    if (!pipe) {
        DEBUG_STDERR("Couldn't 'Strace' command");
        return;
    }

    std::string log_filename = path_to_log + "/" + std::to_string(pid) + "_ssh_out.log";
    auto fd = std::fopen(log_filename.c_str(), "a+");
    if(!fd){
        DEBUG_STDERR("Log-file error: " + log_filename);
        return;
    }
    std::array<char, 256> buffer{};
    while(fgets(buffer.data(), 256, pipe) != nullptr){
        std::string data_from_strace{buffer.data()};
        boost::regex xRegEx("read\\(\\d+, \"(?<cmd>.*)\", 16384\\)\\s+= (1|3|4)");
        std::string symbol = find_key_in_strace_line(data_from_strace, xRegEx, "read");
        if(!symbol.empty())
            std::fwrite(symbol.data(), 1, symbol.size(), fd);
    }
    DEBUG_STDOUT("Connection is closed from PID = " +  std::to_string(pid));
    HandledProcesses& current_proc_list = HandledProcesses::getInstance();
    current_proc_list.remove_from_proc_list(pid);
    pclose(pipe);
    std::fclose(fd);
}

void logger::Strace::ssh_incoming_logger(const pid_t& pid) {
    DEBUG_STDOUT("Handling process(incoming SSH) " + std::to_string(pid));
    std::string strace_cmd= "strace -s 16384 -p " + std::to_string(pid) + " -e write 2>&1";
    FILE* pipe = popen(strace_cmd.c_str(), "r");
    if (!pipe)
    {
        DEBUG_STDERR("Couldn't 'Strace' command");
        return;
    }
    std::string log_filename = path_to_log + "/" + std::to_string(pid) + "_ssh_in.log";
    FILE *fd = std::fopen(log_filename.c_str(), "a+");
    if(!fd){
        DEBUG_STDERR("Log-file error: " + log_filename);
        return;
    }
    std::array<char, 256> buffer{};
    while(fgets(buffer.data(), 256, pipe) != nullptr){
        std::string data_from_strace{buffer.data()};
        boost::regex xRegEx("write\\(\\d+, \"(?<cmd>.*)\", (1|3|4)\\)\\s+= (1|3|4)");
        std::string symbol = find_key_in_strace_line(data_from_strace, xRegEx, "write");
        if(!symbol.empty())
            std::fwrite(symbol.data(), 1, symbol.size(), fd);
    }
    DEBUG_STDOUT("Connection is closed from PID = " +  std::to_string(pid));
    HandledProcesses& current_proc_list = HandledProcesses::getInstance();
    current_proc_list.remove_from_proc_list(pid);
    pclose(pipe);
    std::fclose(fd);
}
