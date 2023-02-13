#include "UserManager.h"

UserManager::~UserManager() {

}

auto UserManager::getInstance() -> UserManager* {
    static UserManager _user_manager;
    return &_user_manager;
}

UserManager::UserManager() {
    // TODO: 从数据库中获取用户信息，初始化该单例
}

auto UserManager::signUp(const string & name, const string & password) -> optional<string> {
    if(_user_list.find(name) != _user_list.end()) return "该用户名已被占用";
    UserInfoPtr user_info_ptr = std::make_shared<UserInfo>();
    user_info_ptr->name = name;
    user_info_ptr->password = password;
    user_info_ptr->token = "";
    _user_list[name] = user_info_ptr;
    return std::nullopt;
}

auto UserManager::signIn(const string & name, const string & password) -> optional<string> {
    if(_user_list.find(name) == _user_list.end()) return std::nullopt;
    auto user_info_ptr = _user_list[name];
    if(user_info_ptr->token.length() == 0) {
        do {
            user_info_ptr->token = _key_generator.generator(TOKEN_LEN);
        }while(_token_set.find(user_info_ptr->token) != _token_set.end());
        _token_set.insert(user_info_ptr->token);
    }
    return user_info_ptr->token;
}

void UserManager::logout(const string & token) {
    if(_token_set.find(token) == _token_set.end()) return;
    _msgQue->add(token, json({
        { "id", token },
        { "operation", "close msg cb" }
    }).dump());
}

void UserManager::connect_user_device(const string & token, const string & key) {
    if(_token_set.find(token) == _token_set.end()) return;
    _user_device_list[token].insert(key);
}

void UserManager::unconnect_user_device(const string & token, const string & key) {
    if(_token_set.find(token) == _token_set.end()) return;
    _user_device_list[token].erase(key);
}

auto UserManager::getAllDevice(const string & token) -> set<string> {
    set<string> ret;
    if(_user_device_list.find(token) != _user_device_list.end()) {
        ret = _user_device_list[token];
    }
    return ret;
}

void UserManager::initMsgQuePtr(shared_ptr<MessageQueue<string>> p) {
    _msgQue = p;
}

