#include "VideoCapture.h"

VideoCapture::VideoCapture()
: rtmp(nullptr)
, buf(nullptr) {
    buf = new char [BUFFSIZE];
}

VideoCapture::~VideoCapture() {
    if(buf) {
        delete [] buf;
        buf = nullptr;
    }
}

bool VideoCapture::pull(const std::string & url, VideoCallBack cb) {
    if(rtmp) return false;
    
    rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    
    int ret = RTMP_SetupURL(rtmp, const_cast<char*>(url.c_str()));
    if(!ret) {
        clear();
        return false;
    }

    ret = RTMP_Connect(rtmp, NULL);
    if(!ret) {
        clear();
        return false;
    }

    ret = RTMP_ConnectStream(rtmp, 0);
    if(!ret) {
        clear();
        return false;
    }

    int len = 0;
    while(true) {
        len = RTMP_Read(rtmp, buf, BUFFSIZE);
        if(len == 0) {
            sleep(1);
            continue;
        }else if(len < 0) {
            break;
        }
        cb(buf, len);
    }

    clear();
    
    return true;
}

void VideoCapture::clear() {
    if(rtmp) {
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
        rtmp = nullptr;
    }
}