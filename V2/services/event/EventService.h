
#ifndef EVENT_SERVICE_H
#define EVENT_SERVICE_H

#include <runtime/Service.h>



namespace Services {
namespace Event {

class EventService : public GWSP::Service
{

public:
    EventService(GWSP::ServiceContext &context);
    ~EventService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();
};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}



#endif //EVENT_SERVICE_H
