#pragma once
#include <string>

using namespace std;

class IMeshCommand {
protected:
    string name;
    string data;
    uint32_t transmitter;
    uint32_t target;
public:

    IMeshCommand(string name, uint32_t target, string data) 
    : name(name), target(target), data(data) {}

    IMeshCommand(string name, uint32_t target, String data) 
    : name(name), target(target), data(string(data.c_str())) {}

    IMeshCommand(string name, uint32_t transmitter, uint32_t target, string data) 
    : name(name), transmitter(transmitter), target(target), data(data) {}

    void set_name(string name) {this->name = name;}
    
    string get_name() {return this->name;}
    
    void set_data(string data) {this->data = data;}

    string get_data() {return this->data;}

    void set_transmitter(uint32_t transmitter) {this->transmitter = transmitter;}

    uint32_t get_transmitter() {return this->transmitter;}

    void set_target(uint32_t target) {this->target = target;}

    uint32_t get_target() {return this->target;}

     ~IMeshCommand() {}
};