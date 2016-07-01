
#include <runtime/ServiceContext.h>
#include <services/reactor/ReactorService.h>

namespace Services {
namespace Reactor {


GWSP::Service *instance(GWSP::ServiceContext &context)
{
    return new ReactorService(context);
}

static std::string serviceName("service.reactor");


ReactorService::ReactorService(GWSP::ServiceContext &context)
:Service(context),
 _reactor(new ACE_Select_Reactor(), true),
 _stopped(false)
{
}

ReactorService::~ReactorService()
{
}

std::string &ReactorService::name()
{
    return serviceName;
}

bool ReactorService::initialize()
{
    return true;
}

bool ReactorService::unInitialize()
{
    return true;
}

bool ReactorService::start()
{
    if (activate() == -1 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "failed to allocate reactor service thread\n"));
        return false;
    }

    return true;
}

bool ReactorService::stop()
{
    _stopped = true;

    _reactor.end_reactor_event_loop();
    _reactor.close();

    wait();

    return true;
}


int ReactorService::svc (void)
{
    int code = 0;
    _stopped = false;
    
    _reactor.owner (ACE_OS::thr_self ());

    ACE_Time_Value timeout;
    timeout.sec(3);

    ACE_DEBUG((LM_DEBUG,"Running reactor service thread\n"));

    for (;;)
    {
        code = _reactor.handle_events(timeout);

        if (_stopped)
        {
            break;
        }
    }

    ACE_DEBUG((LM_DEBUG,
                "return from  reactor service thread[%d]\n",code));

    return code;

}
    
int ReactorService::register_handler (ACE_Event_Handler *eh,
                                      ACE_Reactor_Mask mask)
{
    return _reactor.register_handler(eh, mask);
}

long ReactorService::schedule_timer(ACE_Event_Handler *eh,
                                    const void *arg,
                                    const ACE_Time_Value &delay)
{
    return _reactor.schedule_timer(eh, arg, delay);
}


void ReactorService::remove_handler(ACE_Event_Handler *eh)
{
    _reactor.remove_handler(eh, ACE_Event_Handler::READ_MASK );
}

void ReactorService::cancel_timer(ACE_Event_Handler *eh)
{
    _reactor.cancel_timer(eh );
}



}
}









