#include "yaml-cpp/yaml.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "Recorder.h"
#include "loggerManager.h"
#include "commonInterface.hpp"
#include <memory>

int main() {

    YAML::Node config = YAML::LoadFile("config.yaml");
    if(config.IsNull()) return 0;

    YAML::Node record = config["config"]["record"];
    if(record.IsNull()) return 0;

    std::string path = record["path"].as<std::string>();
    int interval_min = record["interval"].as<int>();
    std::string fifo = record["fifo"].as<std::string>();
    
    config["config"]["log"]["file"]["name"] = record["log_path"];

    ConfigNode log;
    log.setNode(config["config"]["log"]);

    LoggerManager::getInstance()->addLogger(
        RECORDER_LOG,
        std::make_shared<SpdLogger>(log)
    );

    int count = 5;
    int fifo_fd = -1;
    do {
        fifo_fd = open(fifo.c_str(), O_RDONLY);
        if(fifo_fd != -1) break;
        count--;
        sleep(1);
    }while(fifo_fd == -1 && count > 0);
    if(fifo_fd == -1) return 0;

    Info(RECORDER_LOG, "录像进程开启");

    Recorder recorder(path, interval_min, fifo_fd);
    recorder.start();

    close(fifo_fd);

    Warn(RECORDER_LOG, "录像进程关闭");

    return 0;
}