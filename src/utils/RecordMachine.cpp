#include "RecordMachine.h"

RecordMachine::RecordMachine()
: is_running(false)
, fifo_fd(-1) {

}

RecordMachine::~RecordMachine() {
    stop();
}

void RecordMachine::init(const std::string & exec, const std::string & fifo) {
    this->exec = exec;
    this->fifo = fifo;
}

bool RecordMachine::start() {
    if(is_running) return false;
    
    Info(GLOBAL_LOG, "正在创建管道...");
    int ret = access(fifo.c_str(), F_OK);
    if(ret == -1) {
        ret = mkfifo(fifo.c_str(), 0664);
        if(ret == -1) {
            Error(GLOBAL_LOG, "管道创建失败");
            return false;
        }
    }
    
    Info(GLOBAL_LOG, "正在创建录像进程...");
    pid = fork();
    if(pid == -1) {
        close(fifo_fd);
        fifo_fd = -1;
        Error(GLOBAL_LOG, "创建录像进程失败");
        return false;
    }
    
    if(pid == 0) {
        execl(exec.c_str(), "./bin/recorder", NULL);
        Error(GLOBAL_LOG, "录像进程开启失败");
        return false;
    }

    int count = 5;
    do {
        fifo_fd = open(fifo.c_str(), O_WRONLY | O_NONBLOCK);
        if(fifo_fd != -1) break;
        count--;
        sleep(1);
    }while(fifo_fd == -1 && count > 0);

    if(fifo_fd == -1) {
        Error(GLOBAL_LOG, "开启管道失败");
        stop();
        return false;
    }
    
    is_running = true;
    Info(GLOBAL_LOG, "开启录像进程成功");

    return true;
}

void RecordMachine::stop() {
    if(!is_running) return;
    Info(GLOBAL_LOG, "正在关闭录像进程...");
    is_running = false;
    if(fifo_fd != -1) {
        close(fifo_fd);
        fifo_fd = -1;
    }
    if(pid != -1) {
        kill(pid, SIGKILL);
        pid = -1;
        while(true) {
            int ret = waitpid(pid, NULL, WNOHANG);
            if(ret == -1) break;
            usleep(100000);
        }
    }
    Warn(GLOBAL_LOG, "录像进程已关闭"); 
}

bool RecordMachine::send(const RecordMessage & msg) {
    if(!is_running) return false;
    write(fifo_fd, &msg, sizeof(RecordMessage));
    return true;
}