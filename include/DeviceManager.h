#pragma once

#include "KeyGenerator.h"
#include <map>
#include <mutex>
#include <memory>
#include <set>
#include "spdlog/fmt/fmt.h"
#include "yamlConfigDec.h"
#include "commonInterface.hpp"
#include "MessageQueue.h"
#include "nlohmann/json.hpp"
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include "loggerManager.h"
#include "RecordMachine.h"

using namespace std;
using nlohmann::json;

class DeviceManager {
public:
    ~DeviceManager();
    static auto getInstance() -> DeviceManager*;

public:
    void init(ConfigNode _cfg);

    auto addDevice(const string & id) -> optional<DeviceInfoPtr>;
    auto deviceInfo(const string & user, const string & key) -> optional<DeviceInfoPtr>;

    bool talkAble(const string & key);
    auto talkStart(const string & key) -> optional<string>;
    void talkStop(const string & key);

    void initMsgQuePtr(shared_ptr<MessageQueue<string>> p);
    void initRecorder(RecordMachinePtr p);

    void setDeviceQuit(const string & key);

    void heartBeat(const string & key);
    void startHeartBeatHandleTimer();
    void heartBeatHandlerImpl();

private:
    DeviceManager();

    auto genRtmpUrl(const string & key) -> string;

    KeyGenerator _key_generator;
    // id to key
    map<string,string> _id_map;
    // key to device_ifno
    map<string,DeviceInfoPtr> _key_map;
    mutex _mutex;
    
    string host;
    int port;
    string app;

    shared_ptr<MessageQueue<string>> _msgQue;
    RecordMachinePtr _recorder;

    map<string,set<string>> _device_user_list;
};