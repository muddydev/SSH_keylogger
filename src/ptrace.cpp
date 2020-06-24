#include "ptrace.hpp"


void logger::Ptrace::ssh_incoming_logger(const pid_t &pid) {
    char *write_string = nullptr;
    int status = 0;
    int syscall = 0;
    long length = 0;
    struct user_regs_struct regs{};
    memset(&regs, 0, sizeof(regs));

    std::string log_filename = path_to_log + "/" + std::to_string(pid) + "_ssh_in.log";
    FILE *fd = std::fopen(log_filename.c_str(), "a+");
    if(!fd){
        auto log_string = std::string{"Log-file error: "} + log_filename;
        DEBUG_STDERR(log_string);
        return;
    }
    DEBUG_STDOUT("[PTRACE] Starting attach pid: " + std::to_string(pid));
    if(ptrace(PTRACE_ATTACH, pid, NULL, &regs) == -1) return;
    if(waitpid(pid, &status, 0) == -1) return;
    if (!WIFSTOPPED(status)) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return;
    }
    ptrace(PTRACE_SETOPTIONS, pid, 0, 0x00000001);

    while(true) {
        if (wait_for_syscall(pid) != 0)
            break;
        syscall = get_syscall(pid);
        if (wait_for_syscall(pid) != 0)
            break;
        if (syscall == SYSCALL_write) {
            length = get_reg(pid, eax);
            assert(errno == 0);
            write_string = extract_write_string(pid, length);
            find_data_to_log(write_string, length, fd);
            free(write_string);
        }
    }
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    DEBUG_STDOUT("[PTRACE] Connection is closed from PID = " + std::to_string(pid));
    HandledProcesses& current_proc_list = HandledProcesses::getInstance();
    current_proc_list.remove_from_proc_list(pid);
    std::fclose(fd);
}

void logger::Ptrace::ssh_outgoing_logger(const pid_t &pid) {
    char *write_string = nullptr;
    int status = 0;
    int syscall{};
    long length{};
    struct user_regs_struct regs{};
    memset(&regs, 0, sizeof(regs));

    std::string log_filename = path_to_log + "/" + std::to_string(pid) + "_ssh_out.log";
    FILE *fd = std::fopen(log_filename.c_str(), "a+");
    if(!fd){
        auto log_string = std::string{"Log-file error: "} + log_filename;
        DEBUG_STDERR(log_string);
        return;
    }
    DEBUG_STDOUT("[PTRACE] Starting attach pid: " + std::to_string(pid));
    if(ptrace(PTRACE_ATTACH, pid, NULL, &regs) == -1) return;
    if(waitpid(pid, &status, 0) == -1) return;

    if (!WIFSTOPPED(status)) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return;
    }
    ptrace(PTRACE_SETOPTIONS, pid, 0, 0x00000001);

    while(true) {
        if (wait_for_syscall(pid) != 0)
            break;
        syscall = get_syscall(pid);
        if (wait_for_syscall(pid) != 0)
            break;
        if (syscall == SYSCALL_read) {
            length = get_reg(pid, eax);
            assert(errno == 0);
            write_string = extract_write_string(pid, length);
            find_data_to_log(write_string, length, fd);
            free(write_string);
        }
    }
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    DEBUG_STDOUT("[PTRACE] Connection is closed from PID = " + std::to_string(pid));
    HandledProcesses& current_proc_list = HandledProcesses::getInstance();
    current_proc_list.remove_from_proc_list(pid);
    std::fclose(fd);
}

