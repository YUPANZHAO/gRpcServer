#pragma once

#include "KeyGenerator.h"
#include <map>
#include <mutex>
#include <set>
#include "spdlog/fmt/fmt.h"
#include "yamlConfigDec.h"
#include "commonInterface.hpp"

using namespace std;

class DeviceManager {
public:
    ~DeviceManager();
    static auto getInstance() -> DeviceManager*;

public:
    void init(ConfigNode _cfg);

    auto addDevice(const string & id) -> optional<DeviceInfoPtr>;
    auto deviceInfo(const string & key) -> optional<DeviceInfoPtr>;

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
};