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
#include <dirent.h>
#include <fstream>

class RecordMachine {
public:
    RecordMachine();
    ~RecordMachine();

public:
    void init(const std::string & root_path, const std::string & exec, const std::string & fifo);

    bool start();
    void stop();

    bool send(const RecordMessage & msg);

    void download(const std::string & key, const std::string & begin_time, 
        const std::string & end_time, BufferCallBack buf_cb);

private:
    std::string root_path;
    std::string exec;
    std::string fifo;

    bool is_running;
    int fifo_fd;
    pid_t pid;

    const int RECORD_BUFF_SIZE = 1024 * 1024;
    
    time_t mkgmtime(tm * pTm);
    bool check_time_is_between(const std::string & time, const std::string & begin, const std::string & end);
    void writeFile(const std::string & file_path, BufferCallBack buf_cb);
};
using RecordMachinePtr = std::shared_ptr<RecordMachine>;