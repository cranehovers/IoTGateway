
#include <toolkit/ACE_Header_File.h>
#include <server/Application.h>
#include <runtime/ServicesRuntime.h>
#include <runtime/Service.h>
#include <runtime/ServiceRepository.h>
#include <services/console/ConsoleService.h>


namespace GWServer {


GWApplication::GWApplication()
{
}

GWApplication::~GWApplication()
{
}


void GWApplication::init(int argc, char* argv[])
{
    _servicesRuntimePtr = new GWSP::ServicesRuntime();

    if (!_servicesRuntimePtr->initialize())
    {
        ACE_DEBUG((LM_DEBUG, "initialize the service runtime failed\n"));

        return;
    }    
}

int GWApplication::run()
{
    if (!_servicesRuntimePtr->start())
    {
        ACE_DEBUG((LM_DEBUG, "start runtime failed\n"));

        return -1;
    }


    std::string consoleServiceName("service.console");
    
    GWSP::ServiceRepository::ServicePtr consoleServicePtr =
    _servicesRuntimePtr->getRepository()->get(consoleServiceName);

    if (consoleServicePtr->name().compare(consoleServiceName) == 0)
    {
        Services::Console::ConsoleService &consoleServiceRef =
        *dynamic_cast<Services::Console::ConsoleService*> (consoleServicePtr.get());

        consoleServiceRef.wait();
    }

    return 0;
}

}


int main(int argc, char** argv)
{
    toolkit::AutoPtr<GWServer::GWApplication> pApp = new GWServer::GWApplication();

    try
    {
        pApp->init(argc, argv);
    }
    catch (toolkit::Exception& exc)
    {
        ACE_DEBUG((LM_DEBUG, exc.displayText().c_str()));

        return 0;
    }

    return pApp->run();
}

