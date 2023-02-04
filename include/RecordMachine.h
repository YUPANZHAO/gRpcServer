#pragma once

#include "commonInterface.hpp"
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <memory>
#include <queue>
#include "loggerManager.h"
#include <thread>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

class RecordMachine {
public:
    RecordMachine();
    ~RecordMachine();

public:
    void init(const std::string & root_path, const std::string & fifo);

    bool start();
    void stop();

    bool send(const RecordMessage & msg);

private:
    std::string exec;
    std::string fifo;

    bool is_running;
    int fifo_fd;
    pid_t pid;
};
using RecordMachinePtr = std::shared_ptr<RecordMachine>;