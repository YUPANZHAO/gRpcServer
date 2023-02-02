#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

using namespace std;

struct Data {
    int arg;
    char key[100];
    char rtmp[100];
    double height;
};


int main() {

    int ret = access("fifo_rtmp_stream_download", F_OK);
    if(ret == -1) {
        ret = mkfifo("fifo_rtmp_stream_download", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    int fd = open("fifo_rtmp_stream_download", O_WRONLY);
    if(fd == -1) {
        perror("open");
        exit(0);
    }
    
    Data data;
    data.arg = 2;
    std::string key = "hello";
    strcpy(data.key, key.data());
    std::string rtmp = "rtmp://192.168.0.130/live/key";
    strcpy(data.rtmp, rtmp.data());
    data.height = 34.56;
    
    for(int i=0; i < 100; ++i) {
        write(fd, &data, sizeof(Data));
        sleep(1);
        data.arg++;
        data.height += 0.3;
    }

    close(fd);

    return 0;
}