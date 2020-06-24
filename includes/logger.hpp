#pragma once

#include <sched.h>
#include "process.hpp"


namespace logger{

    class Logger{
    public:
        virtual void ssh_incoming_logger(const pid_t &pid) = 0;
        virtual void ssh_outgoing_logger(const pid_t &pid) = 0;
    };
}