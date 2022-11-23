#include "yamlConfigDec.h"

YamlConfigDec::YamlConfigDec() {}

YamlConfigDec::~YamlConfigDec() {}

auto YamlConfigDec::decode(const std::string & filename) -> std::tuple<Node, bool> {
    Node node;
    bool err = false;
    try { node.setNode(YAML::LoadFile(filename)); }catch(std::exception&) { err = true; }
    return { node, err };
}

void YamlConfigDec::Node::setNode(const YAML::Node & node) { _node = node; }

const YamlConfigDec::Node YamlConfigDec::Node::operator [] (int index) const {
    Node new_node;
    if(_node.IsSequence()) {
        new_node._node = _node[index];
    }
    return std::move(new_node);
}

const YamlConfigDec::Node YamlConfigDec::Node::operator [] (const char * name) const {
    return std::move(operator[](std::string(name)));
}

const YamlConfigDec::Node YamlConfigDec::Node::operator [] (const std::string & name) const {
    Node new_node;
    if(_node.IsMap()) {
        new_node._node = _node[name];
    }
    return std::move(new_node);
}

YamlConfigDec::Node::operator int() const {
    auto [value, err] = getValue<int>();
    return value;
}

YamlConfigDec::Node::operator std::string() const {
    auto [value, err] = getValue<std::string>();
    return value;
}

YamlConfigDec::Node::operator bool() const {
    auto [value, err] = getValue<bool>();
    return value;
}