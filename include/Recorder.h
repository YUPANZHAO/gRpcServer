#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include "spdlog/spdlog.h"
#include "VideoCapture.h"
#include "commonInterface.hpp"
#include "loggerManager.h"

class Recorder {
public:
    Recorder(const std::string & path, int interval_min, int fifo_fd);
    ~Recorder();

public:
    void start();

private:
    void download_task(const std::string key, const std::string rtmp_url);
    auto gettimenow(const std::string & time_fmt) -> std::tuple<std::string,time_t>;
    auto getnexttime(time_t tm, const std::string & fmt) -> std::string;

private:
    std::string root_path;
    int interval_min;
    int fifo_fd;
};