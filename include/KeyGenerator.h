#pragma once

#include <string>

using std::string;

class KeyGenerator {
    
    static const int KEY_LEN = 6;    

public:
    KeyGenerator();
    ~KeyGenerator();

public:
    auto generator() -> string;

};