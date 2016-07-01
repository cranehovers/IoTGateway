
#ifndef EventNotifyHandler_H
#define EventNotifyHandler_H

#include <toolkit/ACE_Header_File.h>

namespace Services {
namespace Event {

class EventNotifyHandler
{
public:

    enum {
    EventSerialPortOffline = 0,
    EventSerialPortOnline,
    EventSerialPortLongOnline,
    EventConsoleQuit,
    EventSerialportRecvData,
    EventSerialportSendData,
    
    };
    

    EventNotifyHandler(){};
    virtual ~EventNotifyHandler(){};
    
    virtual int handleEvent(int id, const ACE_Message_Block &b) = 0;

};

}
}

#endif //EventNotifyHandler_H

