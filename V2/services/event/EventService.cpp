
#include <runtime/ServiceContext.h>
#include <services/event/EventService.h>
#include <services/event/EventTable.h>
#include <services/event/EventNotifyHandler.h>



namespace Services {
namespace Event {


GWSP::Service *instance(GWSP::ServiceContext &context)
{
    return new EventService(context);
}

static std::string serviceName("service.event");


EventService::EventService(GWSP::ServiceContext &context)
:Service(context),
_stopped(false)
{    
    _eventTablePrt = new EventTable();
}

EventService::~EventService()
{
}

std::string &EventService::name()
{
    return serviceName;
}

bool EventService::initialize()
{
    // 50K for Queue
    _eventQ.high_water_mark(50*1024);
    
    return true;
}

bool EventService::unInitialize()
{
    return true;
}

bool EventService::start()
{
    if (activate() == -1)
    {
        ACE_DEBUG((LM_DEBUG, "EventService service thread start failed\n"));
        
        return false;
    } 
    
    return true;
}

bool EventService::stop()
{
    _eventQ.deactivate();
    
    _stopped = true;

    //wait();
    clear();

    return true;
}

void EventService::clear()
{
    while (!_eventQ.is_empty())
    {
        ACE_Message_Block *b = 0;
        _eventQ.dequeue(b);

        if (!b)
            b->release();
    }
}

void EventService::set(int id, EventNotifyHandler &handler)
{
    _eventTablePrt->set(id, handler);
}

int EventService::putQ(ACE_Message_Block *b)
{
    if (_eventQ.enqueue_tail(b) == -1 )
    {
        b->release();
        
        ACE_DEBUG((LM_DEBUG, "EventService faild to put b into queue\n"));

        return -1;
    }

    return 0;
}

int EventService::svc()
{
    ACE_DEBUG((LM_DEBUG,
                "Create EventService  thread\n"));

    ACE_Time_Value last_time = ACE_OS::gettimeofday();
    ACE_Time_Value current_time = ACE_OS::gettimeofday();

    _stopped = false;

    while( !_stopped)
    {
        ACE_Message_Block *b = 0;

        if (_eventQ.dequeue_head(b) == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "faile get block from queue\n"));
            continue;
        }

        //ACE_DEBUG((LM_DEBUG, "*** get type[%x] size[%d] ****\n", b->msg_type(),
        //_eventQ.message_count()));

        _eventTablePrt->handleEvent(b->msg_type(), *b);

        if (b)
        {
            b->release();
        }
    }

    ACE_DEBUG((LM_DEBUG,
                "Return from EventService thread\n"));

    return 0;

}



}
}








