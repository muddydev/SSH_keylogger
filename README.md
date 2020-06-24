# SSH_keylogger for GNU/Linux


### Summary
This is featurable SSH and keyboard keylogger for Linux-systems. **Needs root-privileges**.

### Motivation
My main idea is to create programm, which will be able to detect and interact with SSH-connections to/from your Linux PC/server. This interaction is to detect new ssh-connections in runtime or scan already opened connections at the program start moment and then hidely and safely attach to their processes and log all commands, sending in these connections. It can be used by administrators of Linux systems, who need to detect illegal activity by ssh to/from their systems and others.

I will be glad to discuss new ideas, suggestions, criticism, best practices and other help in the development of architecture, the choice of libraries and new features. I would like to combine in one project all means of keylogging.

### Already implemented features:
 - Automatically scan (every 3 seconds) all SSH-connections (incoming and outgoing) and logging their entire commands into ```/var/log/logd/*```.
 - Detect new connections (incoming or outgoing) by SSH and start logging them using **```strace```** for both.
 - Detect new connections (incoming or outgoing) by SSH and start logging them using **```ptrace```** for both.
 - Detect closing the connections and stop logging them.
 - As additional feature is implemented **```keyboard logging```** by scanning system files: ```/dev/input/event*```.
 - Run logger in daemon mode by using flag ```-d```.
 - Opportunity to choose directory to save log files by using flag ```-o```. If this directory doesn't exist, it will be created.

I use C++14/17 for coding but I am still studying. I do it in out of work time and the speed is not such rapid as I dream. You are welcome for contributing together.

### How to compile and run

```bash
cd SSH_keylloger/
mkdir build && cd build
cmake .. && cmake --build .
sudo ./keylogger --help
```

You should start keylloger with ```--help``` flag at the first time to know about available options:

```
Options:
  --help                               produce help message
  -d [ --daemon ]                      daemonize mode
  -k [ --kboard ]                      run logging all detected keyboard 
                                       devices
  -m [ --mode ] arg (=ptrace)          [strace/ptrace] run ssh keylogger via 
                                       STRACE or PTRACE mechanism (ptrace by 
                                       default)
  -o [ --output ] arg (=/var/log/logd) path to directory(if it doesn't exist, 
                                       it will be created) for saving logfiles
```


#### Requirements:
 - CMake
 - GCC
 - Boost (regex, filesystem)
 - Administrative privilege (Ha-Ha)
 
 
**NOTE:** This code uses ```strace``` and ```ptrace``` for logging depends on arguments you use to start program. If you start with ```strace```-argument, it is not enough hide. You can see ```strace``` command in ```ps```. But when you start with ```ptrace```, it is actually hide. Choose it by youself ;)
