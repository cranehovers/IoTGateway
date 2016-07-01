

#ifndef ZigbeeEventHandler_H
#define ZigbeeEventHandler_H

#include <toolkit/ReferenceCountObject.h>
#include <services/event/EventNotifyHandler.h>
#include <services/zigbee/ZigbeeEventHandler.h>
#include <string>


using Services::Event::EventNotifyHandler;

namespace Services {
namespace Zigbee {

class ZigbeeCommandParser;

class ZigbeeEventHandler :public toolkit::RefCountedObject, 
                              public EventNotifyHandler
{
public:
    
    ZigbeeEventHandler();
    ~ZigbeeEventHandler();

    virtual int handleEvent(int id, const ACE_Message_Block &b);
    

private:
    ZigbeeCommandParser *_parser;    
};


}
}

#endif //ZigbeeEventHandler_H
