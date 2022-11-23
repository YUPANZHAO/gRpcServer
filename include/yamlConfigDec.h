#pragma once

#include "yaml-cpp/yaml.h"

class YamlConfigDec {
public:
    YamlConfigDec();
    ~YamlConfigDec();

public:
    class Node {
    public:
        Node() {}

        const Node operator [] (int) const;
        const Node operator [] (const char *) const;
        const Node operator [] (const std::string &) const;

        template<typename Tp> auto getValue() const -> std::tuple<Tp, bool> {
            Tp value {0};
            bool err = false;
            try { if(!_node.IsNull()) value = _node.as<Tp>(); else throw std::exception(); }
            catch(std::exception& e) { err = true; }
            return { value, err };
        }
        
        void setNode(const YAML::Node &);
        
        operator int() const;
        operator std::string() const;
        operator bool() const;

    private:
        YAML::Node _node;
    };
    
    auto decode(const std::string &) -> std::tuple<Node, bool>;
};

typedef YamlConfigDec::Node ConfigNode;