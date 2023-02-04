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
        info->is_active = false;
        time(&info->last_active_time);
        ret = _key_map[key] = info;
        _id_map[id] = key;
    }else {
        ret = _key_map[_id_map[id]];
    }
    // 发送录像消息，交由录像进程开启记录线程
    if(ret != nullopt) {
        RecordMessage msg;
        strcpy(msg.key, (*ret)->key.data());
        strcpy(msg.rtmp_url, (*ret)->rtmp_url.data());
        _recorder->send(msg);
    }
    return ret;
}

auto DeviceManager::deviceInfo(const string & user, const string & key) -> optional<DeviceInfoPtr> {
    lock_guard<mutex> guard(_mutex);
    if(_key_map.find(key) == _key_map.end()) return nullopt;
    _device_user_list[key].insert(user);
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
    _msgQue->add(key, json({
        { "id", key },
        { "operation", "talkStart" },
        { "rtmp_pull_url", rtmp_push_url }
    }).dump());
    return rtmp_push_url; 
}

void DeviceManager::talkStop(const string & key) {
    if(_key_map.find(key) == _key_map.end()) return;
    _msgQue->add(key, json({
        { "id", key },
        { "operation", "talkStop" }
    }).dump());
    _key_map[key]->is_talking = false;
}

void DeviceManager::initMsgQuePtr(shared_ptr<MessageQueue<string>> p) {
    _msgQue = p;
}

void DeviceManager::initRecorder(RecordMachinePtr p) {
    _recorder = p;
}

void DeviceManager::setDeviceQuit(const string & key) {
    if(_key_map.find(key) == _key_map.end()) return;
    _msgQue->add(key, json({
        { "id", key },
        { "operation", "close msg cb" }
    }).dump());
}

void DeviceManager::heartBeat(const string & key) {
    if(_key_map.find(key) == _key_map.end()) return;
    auto device = _key_map[key];
    time(&device->last_active_time);
}

void DeviceManager::startHeartBeatHandleTimer() {
    // 设置开启定时器
    struct itimerval time_value;
    time_value.it_interval.tv_sec = 4;
    time_value.it_interval.tv_usec = 0;
    time_value.it_value.tv_sec = 2;
    time_value.it_value.tv_usec = 0;
    int ret = setitimer(ITIMER_REAL, &time_value, NULL);
    if(ret == -1) {
        Error(GLOBAL_LOG, "开启心跳处理定时器失败!");
        return;
    }
    // 设置定时器任务处理逻辑
    struct sigaction action;
    action.sa_handler = [](int sig) {
        DeviceManager::getInstance()->heartBeatHandlerImpl();  
    };
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    action.sa_restorer = NULL;
    // 启动定时器信号捕捉
    ret = sigaction(SIGALRM, &action, NULL);
    if(ret == -1) {
        Error(GLOBAL_LOG, "开启心跳处理定时器失败!");
        return;
    }
    Info(GLOBAL_LOG, "开启心跳定时器成功! 定时器将于2秒后开始运行...");
}

void DeviceManager::heartBeatHandlerImpl() {
    time_t now = time(NULL);
    lock_guard<mutex> guard(_mutex);
    for(auto & pair : _device_user_list) {
        auto device = _key_map[pair.first];
        bool is_active = (now - device->last_active_time) > 3 ? false : true;
        if(!(is_active ^ device->is_active)) continue;
        device->is_active = is_active;
        Info("设备({})状态变化 is_active: {} ---> {}", device->key, device->is_active, is_active);
        for(auto & user : pair.second) {
            _msgQue->add(user, json({
                { "id", user },
                { "operation", "device status change" },
                { "device_id", device->key },
                { "device_status", is_active ? "active" : "negative" }
            }).dump());
        }
    }
}