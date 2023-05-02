#pragma once
#include <string>
#include <functional>

using namespace std;

class IMeshEvent {
protected:
    string name;
    string data;
    uint32_t transmitter;
    uint32_t target;
public:
    IMeshEvent(string name, string data, uint32_t transmitter, uint32_t target){
        this->name = name;
        this->transmitter = transmitter;
        this->target = target;
    }

    void set_name(string name) {this->name = name;}
    string get_name() {return this->name;}

    void set_data(string data) {this->data = data;}
    string get_data() {return this->data;}

    void set_transmitter(uint32_t transmitter) {this->transmitter = transmitter;}
    uint32_t get_transmitter() {return this->transmitter;}

    void set_target(uint32_t target) {this->target = target;}
    uint32_t get_target() {return this->target;}

};