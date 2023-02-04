#pragma once

#include "librtmp/rtmp.h"
#include <functional>
#include <iostream>
#include <string>
#include <unistd.h>

class VideoCapture {
    using VideoCallBack = std::function<void(char*,int)>;
    const int BUFFSIZE = 1024 * 1024;
public:
    VideoCapture();
    ~VideoCapture();

public:
    bool pull(const std::string & url, VideoCallBack cb);
    void clear();

private:
    RTMP* rtmp;
    char* buf;
};