
#include <runtime/ServiceGetter.h>
#include <services/event/EventService.h>
#include <services/event/EventNotifyHandler.h>
#include <services/console/ConsoleService.h>


using Services::Event::EventService;
using Services::Event::EventNotifyHandler;
using namespace GWSP;


namespace Services {
namespace Console {

GWSP::Service *instance(GWSP::ServiceContext &context){return new ConsoleService(context);}

static std::string serviceName("service.console");


ConsoleService::ConsoleService(GWSP::ServiceContext &context)
:_stopped(false),
Service(context)
{
}

ConsoleService::~ConsoleService()
{
}

std::string &ConsoleService::name()
{
    return serviceName;
}

bool ConsoleService::initialize()
{
    return true;
}

bool ConsoleService::unInitialize()
{
    return true;
}

bool ConsoleService::start()
{
    if (activate() == -1)
    {
        ACE_DEBUG((LM_DEBUG, "console service start failed\n"));
        
        return false;
    }
    
    return true;
}

bool ConsoleService::stop()
{
    _stopped = true;
    
    return true;
}

int ConsoleService::svc()
{
    while(!_stopped)
    {
        char c = ::getchar();

        switch (c)
        {
            case 'q':
            case 'Q':
            {
                try
                {
                    std::string name("service.event");
                    EventService::Ptr ePtr = 
                    ServiceGetter::findByName<EventService>(context(), name);

                    ACE_Message_Block *b = new ACE_Message_Block();
                    
                    b->msg_type(EventNotifyHandler::EventConsoleQuit);

                    ePtr->putQ(b);
                }
                catch(...)
                {
                    ACE_DEBUG((LM_DEBUG, "get event service failed at application\n"));
                    
                    return -1;
                }
                
            }
            break;

            default:
            {
                //ACE_DEBUG((LM_DEBUG, "unknown command\n"));
            }
        }
    }

    return 0;
}

}
}



