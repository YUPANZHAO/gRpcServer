#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <map>
#include "spdlog/spdlog.h"
#include "VideoCapture.h"
#include "commonInterface.hpp"
#include "loggerManager.h"
#include <dirent.h>
#include <sys/types.h>

class Recorder {
public:
    Recorder(const std::string & path, int interval_min, int fifo_fd, int days);
    ~Recorder();

public:
    void start();

private:
    void download_task(const std::string key, const std::string rtmp_url);
    auto get_time_now(const std::string & time_fmt) -> std::tuple<std::string,time_t>;
    auto get_offset_time(time_t tm, int offset_min, const std::string & fmt) -> std::string;
    void clear_expired_video(const std::string key);

private:
    std::string root_path;
    int interval_min;
    int fifo_fd;
    int reserved_days;

    std::map<std::string,bool> is_exist;
};