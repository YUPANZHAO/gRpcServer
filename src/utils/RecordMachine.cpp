#include "RecordMachine.h"

RecordMachine::RecordMachine()
: is_running(false) {

}

RecordMachine::~RecordMachine() {
    stop();
}

bool RecordMachine::start() {
    if(is_running) return false;
    Info(GLOBAL_LOG, "正在创建管道...");
    int ret = pipe(pipefd);
    if(ret == -1) return false;
    Info(GLOBAL_LOG, "正在创建录像进程...");
    pid = fork();
    if(pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }
    is_running = true;
    if(pid == 0) {
        close(pipefd[1]);
        run();
        exit(0);
    }
    close(pipefd[0]);
    Info(GLOBAL_LOG, "开启录像记录成功");
    return true;
}

void RecordMachine::stop() {
    if(!is_running) return;
    Info(GLOBAL_LOG, "正在关闭录像进程...");
    is_running = false;
    kill(pid, SIGKILL);
    close(pipefd[0]);
    close(pipefd[1]);
    while(true) {
        int ret = waitpid(pid, NULL, WNOHANG);
        if(ret == -1) break;
        usleep(100000);
    }
    Info(GLOBAL_LOG, "录像进程已关闭"); 
}

bool RecordMachine::send(const RecordMessage & msg) {
    if(!is_running) return false;
    write(pipefd[1], &msg, sizeof(RecordMessage));
    return true;
}

void RecordMachine::run() {
    Info(GLOBAL_LOG, "录像进程运行中...");
    while(true) {
        RecordMessage msg;
        int ret = read(pipefd[0], &msg, sizeof(RecordMessage));
        if(ret == -1 || ret == 0) break;
        Info(GLOBAL_LOG, "录像进程接收到消息: key = {}, rtmp_url = {}", msg.key, msg.rtmp_url);
    }
    Info(GLOBAL_LOG, "录像进程结束");
}