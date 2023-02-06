#include "RecordMachine.h"

RecordMachine::RecordMachine()
: is_running(false)
, fifo_fd(-1) {

}

RecordMachine::~RecordMachine() {
    stop();
}

void RecordMachine::init(const std::string & root_path, const std::string & exec, const std::string & fifo) {
    this->root_path = root_path;
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

void RecordMachine::download(const std::string & key,
const std::string & begin_time, const std::string & end_time,
BufferCallBack buf_cb) {
    std::string save_dir = fmt::format("{}/{}", root_path, key);
    DIR* dir = opendir(save_dir.c_str());
    if(dir == nullptr) return;
    dirent* file;
    const int time_length = 16;
    while((file = readdir(dir)) != nullptr) {
        std::string name = file->d_name;
        if(name.length() > key.length() + 2 + (time_length << 1)) {
            std::string begin = name.substr(key.length() + 1, time_length);
            std::string end = name.substr(key.length() + 2 + time_length, time_length);
            if(check_time_is_between(begin, begin_time, end_time)
            || check_time_is_between(end, begin_time, end_time)) {
                writeFile(save_dir + "/" + name, buf_cb);
            }
        }
    }
    closedir(dir);
}

time_t RecordMachine::mkgmtime(tm * pTm) {
    unsigned int year = pTm->tm_year + 1900;
    unsigned int mon = pTm->tm_mon + 1;
    unsigned int day = pTm->tm_mday;
    unsigned int hour = pTm->tm_hour;
    unsigned int min = pTm->tm_min;
    unsigned int sec = pTm->tm_sec;
    if (0 >= (int)(mon -= 2)) {
        mon += 12;
        year -= 1;
    }
    return ((((unsigned long)(year / 4 - year / 100 + year / 400 + 367 * mon / 12 + day) +
        year * 365 - 719499) * 24 + hour) * 60 + min) * 60 + sec;
}

bool RecordMachine::check_time_is_between(const std::string & time, const std::string & begin, const std::string & end) {
    tm time_tm = {0}, begin_tm = {0}, end_tm = {0};
    try {
        strptime(time.c_str(), "%Y_%m_%d_%H_%M", &time_tm);
        strptime(begin.c_str(), "%Y-%m-%d %H:%M:%S", &begin_tm);
        strptime(end.c_str(), "%Y-%m-%d %H:%M:%S", &end_tm);
    }catch(std::exception) {
        return false;
    }
    time_t time_time_t = mkgmtime(&time_tm);
    time_t begin_time_t = mkgmtime(&begin_tm);
    time_t end_time_t = mkgmtime(&end_tm);
    return time_time_t >= begin_time_t && time_time_t <= end_time_t;
}

void RecordMachine::writeFile(const std::string & file_path, BufferCallBack buf_cb) {
    char* buf = new char [RECORD_BUFF_SIZE];
    std::ifstream infile;
    infile.open(file_path, std::ifstream::in | std::ifstream::binary);
    while(!infile.eof()) {
        infile.read(buf, RECORD_BUFF_SIZE);
        buf_cb(buf, infile.gcount());
    }
    delete [] buf;
}