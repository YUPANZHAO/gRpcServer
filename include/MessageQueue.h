#pragma once

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <optional>
#include <map>

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
    void add(const Message & message) {
        {
            unique_lock<mutex> lock(_mutex);
            _que.push(message);
        }
        _cond.notify_all();
    }

    // 获取一条消息
    auto get(MatchFunc is_match) -> optional<Message> {
        optional<Message> ret = nullopt;
        {
            unique_lock<mutex> lock(_mutex);
            while(!is_shutdown && (_que.empty() || !is_match(_que.front()))) {
                _cond.wait(lock);
            }
            if(!is_shutdown) {
                ret = _que.front();
                _que.pop();
            }
        }
        _cond.notify_all();
        return ret;
    }
    
private:
    queue<Message> _que;
    mutex _mutex;
    condition_variable _cond;
    bool is_shutdown;

};