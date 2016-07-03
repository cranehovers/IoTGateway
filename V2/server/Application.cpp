
#include <toolkit/ACE_Header_File.h>
#include <server/Application.h>
#include <runtime/ServicesRuntime.h>
#include <runtime/Service.h>
#include <runtime/ServiceGetter.h>
#include <runtime/ServiceRepository.h>
#include <services/console/ConsoleService.h>


using namespace Services::Console;
using namespace GWSP;

namespace GWServer {

GWApplication::GWApplication()
{
}

GWApplication::~GWApplication()
{
}


bool GWApplication::init(int argc, char* argv[])
{
    _servicesRuntimePtr = new GWSP::ServicesRuntime();

    if (!_servicesRuntimePtr->initialize())
    {
        ACE_DEBUG((LM_DEBUG, "initialize the service runtime failed\n"));

        return false;
    }

    try
    {
        std::string name("service.event");
        _eventServicePtr = 
        ServiceGetter::findByName<EventService>(_servicesRuntimePtr->context(), name);

        _eventServicePtr->set(EventConsoleQuit, *this);
    }
    catch(...)
    {
        ACE_DEBUG((LM_DEBUG, "get event service failed at application\n"));
        
        return false;
    }

    return true;
}

int GWApplication::handleEvent(int id, const ACE_Message_Block &b)
{
    if (id == EventConsoleQuit)
    {
        _servicesRuntimePtr->stop();
    }

    return 0;
}

int GWApplication::run()
{
    if (!_servicesRuntimePtr->start())
    {
        ACE_DEBUG((LM_DEBUG, "start runtime failed\n"));

        return -1;
    }

    try
    {
        std::string name("service.console");

        ConsoleService::Ptr consoleServicePtr =
        ServiceGetter::findByName<ConsoleService>(_servicesRuntimePtr->context(), name);

        consoleServicePtr->wait();

        //_servicesRuntimePtr->stop();
    }
    catch(toolkit::NullPointerException &e)
    {
        ACE_DEBUG((LM_DEBUG, "get ConsoleService failed at app run method\n"));
    }

    return 0;
}

}


int main(int argc, char** argv)
{
    toolkit::AutoPtr<GWServer::GWApplication> pApp = new GWServer::GWApplication();

    try
    {
        if (!pApp->init(argc, argv))
        {
            return -1;
        }
    }
    catch (toolkit::Exception& exc)
    {
        ACE_DEBUG((LM_DEBUG, exc.displayText().c_str()));

        return 0;
    }

    return pApp->run();
}

