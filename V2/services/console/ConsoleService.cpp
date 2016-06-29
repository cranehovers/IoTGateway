#include <services/console/ConsoleService.h>

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
                _stopped = true;
            }
            break;

            default:
            {
                ACE_DEBUG((LM_DEBUG, "unknown command\n"));
            }
        }
    }

    return 0;
}

}
}



