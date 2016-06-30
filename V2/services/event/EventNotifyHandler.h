
#ifndef EventNotifyHandler_H
#define EventNotifyHandler_H

#include <toolkit/ACE_Header_File.h>

namespace Services {
namespace Event {

class EventNotifyHandler
{
public:

    enum {
    EventNotifySerialPortOffline = 0,
    EventNotifySerialPortOnline,
    EventConsoleQuit,
    
    };
    

    EventNotifyHandler(){};
    virtual ~EventNotifyHandler(){};
    
    virtual int handleEvent(int id, const ACE_Message_Block &b) = 0;

};

}
}

#endif //EventNotifyHandler_H

