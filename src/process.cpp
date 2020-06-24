#include "process.hpp"


/*
 * Incomming connection detector
 */
bool Process::find_incoming_ssh() const {
    if (_args.find("@pts/") != std::string::npos){
        DEBUG_STDOUT("Incoming ssh connection: PID = " + std::to_string(_pid));
        return true;
    } else return false;
}


/*
 * Outgoing connection detector
 */
bool Process::find_outgoing_ssh() const {
    if (_cmd == "ssh"){
        DEBUG_STDOUT("Outgoing ssh connection: PID = " + std::to_string(_pid));
        return true;
    } else return false;
}

/*
 * Remove process from check list
 */
void HandledProcesses::remove_from_proc_list(uint16_t pid) {
    //TODO for test
    auto it = std::find(procs_in_monitoring.begin(), procs_in_monitoring.end(), pid);
    if(it != procs_in_monitoring.end()) {
        *it = std::move(procs_in_monitoring.back());
        procs_in_monitoring.pop_back();
    }
}
