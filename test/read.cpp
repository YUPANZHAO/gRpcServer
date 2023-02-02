#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct Data {
    int arg;
    char key[100];
    char rtmp[100];
    double height;
};

int main() {

    // 打开管道文件
    int fd = open("fifo_rtmp_stream_download", O_RDONLY);
    if(fd == -1) {
        perror("open");
        exit(0);
    }

    // 读数据
    while(true) {
        Data data;
        int len = read(fd, &data, sizeof(Data));
        if(len == 0) {
            cout << "写端断开了连接..." << endl;
            exit(0);
        }
        std::cout << "receive: arg = " << data.arg
                  << " key = " << data.key
                  << " rtmp = " << data.rtmp
                  << " height = " << data.height
                  << std::endl;
    }

    // 关闭文件
    close(fd);

    return 0;
}