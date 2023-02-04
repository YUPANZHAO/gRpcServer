#include "Recorder.h"

Recorder::Recorder(const std::string & path, int interval_min, int fifo_fd) {
    this->root_path = path;
    this->interval_min = interval_min;
    this->fifo_fd = fifo_fd;
}

Recorder::~Recorder() {
    
}

void Recorder::start() {
    while(true) {
        RecordMessage msg = {0};
        int ret = read(fifo_fd, &msg, sizeof(RecordMessage));
        if(ret == 0) {
            sleep(1);
            continue;
        }else if(ret < 0) {
            break;
        }
        Info(RECORDER_LOG, "获取到消息: key = {}, rtmp_url = {}", msg.key, msg.rtmp_url);
        std::thread thd([msg, this]() {
            this->download_task(msg.key, msg.rtmp_url);
        });
        thd.detach();
    }
}

void Recorder::download_task(const std::string key, const std::string rtmp_url) {
    std::string save_path = fmt::format("{}/{}", root_path, key);
    if(access(root_path.c_str(), F_OK) == -1) {
        if(mkdir(root_path.c_str(), 0777) == -1) {
            Error(RECORDER_LOG, "创建存储目录 {} 失败", root_path);
            return;
        }
    }
    if(access(save_path.c_str(), F_OK) == -1) {
        if(mkdir(save_path.c_str(), 0777) == -1) {
            Error(RECORDER_LOG, "创建存储目录 {} 失败", save_path);
            return;
        }
    }
    int fd = -1;
    time_t pre = -1;
    std::string pre_time = "";
    VideoCapture puller;
    Info(RECORDER_LOG, "拉流线程开启 key = {}", key);
    puller.pull(rtmp_url, [&](char* buf, int len) {
        auto [ now_time, now ] = this->gettimenow("%Y_%m_%d_%H_%M");
        if(fd == -1 || now >= pre + 60 * this->interval_min) {
            pre_time = now_time;
            pre = (now - now % 60);
            if(fd != -1) {
                close(fd);
                fd = -1;
            }
            fd = open(fmt::format("{}/{}_{}_{}.flv", save_path, key, now_time, this->getnexttime(now, "%Y_%m_%d_%H_%M")).c_str(),
                    O_CREAT | O_WRONLY | O_APPEND);
            if(fd != -1 && len >= 3 && !(buf[0] == 'F' && buf[1] == 'L' && buf[2] == 'V')) {
                char flv_header[13] = {0};
                flv_header[0] = 0x46;
                flv_header[1] = 0x4C;
                flv_header[2] = 0x56;
                flv_header[3] = 0x01;
                flv_header[4] = 0x05;
                flv_header[8] = 0x09;
                write(fd, flv_header, 13);
            }
        }
        if(fd != -1) {
            write(fd, buf, len);
        }
    });
    if(fd != -1) {
        close(fd);
    }
    Warn(RECORDER_LOG, "拉流线程结束 key = {}", key);
}

auto Recorder::gettimenow(const std::string & time_fmt) -> std::tuple<std::string,time_t> {
    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);
    char time_now[30];
    strftime(time_now, sizeof(time_now), time_fmt.c_str(), &tm);
    return { time_now, now };
}

auto Recorder::getnexttime(time_t tm, const std::string & fmt) -> std::string {
    tm += 60 * interval_min;
    struct tm time;
    localtime_r(&tm, &time);
    char time_now[30];
    strftime(time_now, sizeof(time_now), fmt.c_str(), &time);
    return time_now;
}