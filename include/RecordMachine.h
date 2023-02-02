#pragma once

#include "commonInterface.hpp"
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <memory>
#include <queue>
#include "loggerManager.h"

struct RecordMessage {
    char key[10];
    char rtmp_url[50];
};

class RecordMachine {
public:
    RecordMachine();
    ~RecordMachine();

public:
    bool start();
    void stop();

    bool send(const RecordMessage & msg);

private:
    bool is_running;
    int pipefd[2];
    pid_t pid;

    void run();
};
using RecordMachinePtr = std::shared_ptr<RecordMachine>;