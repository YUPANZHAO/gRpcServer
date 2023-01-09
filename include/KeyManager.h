#pragma once

#include "KeyGenerator.h"
#include <map>
#include <mutex>
#include <set>
#include "spdlog/fmt/fmt.h"
#include "yamlConfigDec.h"

using namespace std;

class KeyManager {
public:
    ~KeyManager();
    static auto getInstance() -> KeyManager*;

public:
    auto generator(const string & id) -> string;

    void initRtmpInfo(ConfigNode _cfg);
    auto genRtmpUrl(const string & key) -> string;

private:
    KeyManager();

    KeyGenerator _key_generator;
    map<string,string> _key_map;
    set<string> _key_set;
    mutex _mutex;
    
    string host;
    int port;
    string app;
};