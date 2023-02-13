#pragma once

#include <string>
#include "commonInterface.hpp"
#include <vector>
#include <map>
#include <set>
#include "KeyGenerator.h"
#include "MessageQueue.h"
#include <memory>
#include "nlohmann/json.hpp"

using std::string;
using std::optional;
using std::vector;
using std::map;
using std::set;
using nlohmann::json;

class UserManager {
public:
    ~UserManager();
    static auto getInstance() -> UserManager*;

    // 返回 nullopt 表示注册成功，否则返回错误信息
    auto signUp(const string & name, const string & password) -> optional<string>;
    // 返回 nullopt 表示登录失败，否则返回用户token
    auto signIn(const string & name, const string & password) -> optional<string>;
    // 注销账号
    void logout(const string & token);
    
    // 关联用户和设备
    void connect_user_device(const string & token, const string & key);
    // 取消关联用户和设备
    void unconnect_user_device(const string & token, const string & key);
    // 获取用户的所有关联设备
    auto getAllDevice(const string & token) -> set<string>;

    // 初始化消息队列
    void initMsgQuePtr(shared_ptr<MessageQueue<string>> p);
    
private:
    UserManager();

    // name -> user info pointer
    map<string, UserInfoPtr> _user_list;
    // token -> [ device_key ... ]
    map<string, set<string>> _user_device_list;
    // token set
    set<string> _token_set;
    // token generator
    KeyGenerator _key_generator;

    shared_ptr<MessageQueue<string>> _msgQue;
};