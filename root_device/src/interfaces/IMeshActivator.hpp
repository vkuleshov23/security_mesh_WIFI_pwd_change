#ifndef _SmartPublisher_
#define _SmartPublisher_

#include <functional>
#include <string>

using namespace std;

class MeshActivator{
protected:
    function<void(const string&)> processor;
    function<void(const string&, string&, uint32_t)> parser;
    string event_name;
    string param;
public:

    MeshActivator(const string& event_name, function<void(const string&)>& processor, function<void(const string&, string&, uint32_t)> parser){
        this->parser = parser;
        this->processor = processor;
        this->event_name = event_name;
    };

    const string& getName() const{
        return event_name;
    }

    const string& getParam() const{
        return param;
    }

    void setParam(string& param) {
        this->param = param;
        processor(this->param);
    }

    void processEvent(const string& event, uint32_t transmitter){
        parser(event, this->param, transmitter);
        processor(this->param);
    }

};

#endif