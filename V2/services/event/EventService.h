
#ifndef EVENT_SERVICE_H
#define EVENT_SERVICE_H

#include <runtime/Service.h>



namespace Services {
namespace Event {

class EventTable;
class EventNotifyHandler;

class EventService : public GWSP::Service, public ACE_Task_Base
{

public:
    typedef toolkit::AutoPtr<EventService> Ptr;

    EventService(GWSP::ServiceContext &context);
    ~EventService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();
    virtual int svc (void);
    void set(int id, EventNotifyHandler &handler);
    int putQ(ACE_Message_Block *b);
    void clear();


private:

    typedef ACE_Message_Queue<ACE_MT_SYNCH> MsgQueue;
    typedef toolkit::AutoPtr<EventTable> EventTablePtr;

    EventTablePtr _eventTablePrt;
    MsgQueue _eventQ;
    bool _stopped;
    
};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}



#endif //EVENT_SERVICE_H
