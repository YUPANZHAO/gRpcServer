#include "DeviceManager.h"

DeviceManager::~DeviceManager() {}

auto DeviceManager::getInstance() -> DeviceManager* {
    static DeviceManager _device_manager;
    return &_device_manager;
}

void DeviceManager::init(ConfigNode _cfg) {
    host = string(_cfg["host"]);
    port = int(_cfg["port"]);
    app = string(_cfg["app"]);
}

auto DeviceManager::addDevice(const string & id) -> optional<DeviceInfoPtr> {
    lock_guard<mutex> guard(_mutex);
    optional<DeviceInfoPtr> ret = nullopt;
    string key;
    if(_id_map.find(id) == _id_map.end()) {
        do {
            key = _key_generator.generator();
        }while(_key_map.find(key) != _key_map.end());
        DeviceInfoPtr info = make_shared<DeviceInfo>();
        info->name = "unknow";
        info->device_id = id;
        info->rtmp_url = genRtmpUrl(key);
        info->key = key;
        info->is_talking = false;
        ret = _key_map[key] = info;
        _id_map[id] = key;
    }else {
        ret = _key_map[_id_map[id]];
    }
    return ret;
}

auto DeviceManager::deviceInfo(const string & key) -> optional<DeviceInfoPtr> {
    lock_guard<mutex> guard(_mutex);
    if(_key_map.find(key) == _key_map.end()) return nullopt;
    return _key_map[key];
}

DeviceManager::DeviceManager() {
    // TODO: 从数据库中获取数据初始化_key_map
}

auto DeviceManager::genRtmpUrl(const string & key) -> string {
    return fmt::format("rtmp://{}:{}/{}/{}", host, port, app, key);
}

bool DeviceManager::talkAble(const string & key) {
    if(_key_map.find(key) == _key_map.end()) return false;
    return !_key_map[key]->is_talking;
}

auto DeviceManager::talkStart(const string & key) -> optional<string> {
    if(!talkAble(key)) return nullopt;
    _key_map[key]->is_talking = true;
    string rtmp_push_url = _key_map[key]->rtmp_url + "TALK";
    _msgQue->add(json({
        { "id", key },
        { "operation", "talkStart" },
        { "rtmp_pull_url", rtmp_push_url }
    }).dump());
    return rtmp_push_url; 
}

void DeviceManager::talkStop(const string & key) {
    if(_key_map.find(key) == _key_map.end()) return;
    _msgQue->add(json({
        { "id", key },
        { "operation", "talkStop" }
    }).dump());
    _key_map[key]->is_talking = false;
}

void DeviceManager::initMsgQuePtr(shared_ptr<MessageQueue<string>> p) {
    _msgQue = p;
}

void DeviceManager::setDeviceQuit(const string & key) {
    if(_key_map.find(key) == _key_map.end()) return;
    _msgQue->add(json({
        { "id", key },
        { "operation", "close msg cb" }
    }).dump());
}