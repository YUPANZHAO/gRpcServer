#pragma once

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <optional>
#include <map>
#include <list>

using namespace std;

template<typename Message>
class MessageQueue {

    using MatchFunc = function<bool(const Message&)>;

public:
    MessageQueue() : is_shutdown(false) {};
    ~MessageQueue() {
        is_shutdown = true;
        _cond.notify_all();
    }

    // 添加一条消息
    void add(const string & key, const Message & message) {
        {
            unique_lock<mutex> lock(_mutex);
            _lists[key].push_back(message);
        }
        _cond.notify_all();
    }

    // 获取一条消息
    auto get(const string & key) -> optional<Message> {
        optional<Message> ret = nullopt;
        {
            unique_lock<mutex> lock(_mutex);
            while(!is_shutdown && _lists[key].empty()) {
                _cond.wait(lock);
            }
            if(!is_shutdown) {
                ret = _lists[key].front();
                _lists[key].pop_front();
            }
        }
        _cond.notify_all();
        return ret;
    }
    
private:
    map<string, list<Message>> _lists;
    mutex _mutex;
    condition_variable _cond;
    bool is_shutdown;

};