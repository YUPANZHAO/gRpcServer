#include "KeyGenerator.h"
#include <unistd.h>

KeyGenerator::KeyGenerator() {}

KeyGenerator::~KeyGenerator() {}

auto KeyGenerator::generator(int len) -> string {
    string key = "";
    for(int i=0; i < len; ++i) {
        int flag = rand();
        if(flag & 0x2) key += char('0' + ((flag & 0x3C) >> 2) % 10);
        else key += char(((flag & 0x1) ? 'A' : 'a') + ((flag & 0xFC) >> 2) % 26);
    }
    return key;
}
