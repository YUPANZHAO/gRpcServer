#include <iostream>
#include "librtmp/rtmp.h"
#include <unistd.h>
#include <fcntl.h>

using namespace std;

//g++ read.cpp -o read -I../../include -L../../libs -lrtmp -lssl -lcrypto -ldl -lz

int main() {

    RTMP* rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);

    int ret = RTMP_SetupURL(rtmp, "rtmp://192.168.0.130:1935/live/1A2U52");
    if(!ret) {
        cout << "RTMP_SetupURL error" << endl;
        return 0;
    }
    
    rtmp->Link.timeout = 100;
    rtmp->Link.lFlags |= RTMP_LF_LIVE;
    ret = RTMP_Connect(rtmp, NULL);
    if(!ret) {
        cout << "RTMP_Connect error" << endl;
        return 0;
    }

    ret = RTMP_ConnectStream(rtmp, 0);
    if(!ret) {
        cout << "RTMP_ConnectStream" << endl;
        return 0;
    }

    int bufsize = 1024 * 512;
    char* buf = new char [bufsize];
    
    int fd = open("video.flv", O_CREAT | O_WRONLY);
    
    int sum = 0;
    while(true) {
        int len = RTMP_Read(rtmp, buf, bufsize);
        if(len <= 0) {
            cout << "读取发生错误 " << len << endl;
            break;
        }
        sum += len;
        cout << "读取到 " << len << " bytes，total: " << sum / 1024.0 / 1024.0 << " MB\n";
        write(fd, buf, len);
        if(sum >= 1024 * 1024 * 10) break;
    }

    delete [] buf;

    close(fd);

    RTMP_Close(rtmp);
    RTMP_Free(rtmp);

    cout << "获取视频流完毕" << endl;

    return 0;
}