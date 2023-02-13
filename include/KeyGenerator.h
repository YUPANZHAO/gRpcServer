#pragma once

#include <string>

using std::string;

class KeyGenerator {

public:
    KeyGenerator();
    ~KeyGenerator();

public:
    auto generator(int len) -> string;

};