#include "KeyManager.h"

KeyManager::~KeyManager() {}

auto KeyManager::getInstance() -> KeyManager* {
    static KeyManager _key_manager;
    return &_key_manager;
}

auto KeyManager::generator(const string & id) -> string {
    lock_guard<mutex> guard(_mutex);
    string key;
    if(_key_map.find(id) == _key_map.end()) {
        do {
            key = _key_generator.generator();
        }while(_key_set.find(key) != _key_set.end());
        _key_map[id] = key;
        _key_set.insert(key);
    }else {
        key = _key_map[id];
    }
    return key;
}

void KeyManager::initRtmpInfo(ConfigNode _cfg) {
    host = string(_cfg["host"]);
    port = int(_cfg["port"]);
    app = string(_cfg["app"]);
}

auto KeyManager::genRtmpUrl(const string & key) -> string {
    string rtmp = fmt::format("rtmp://{}:{}/{}/{}", host, port, app, key);
    return rtmp;
}

KeyManager::KeyManager() {
    // TODO: 从数据库中获取数据初始化_key_map、_key_set
}
