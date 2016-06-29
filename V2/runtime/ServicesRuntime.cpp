

#include <runtime/ServicesRuntime.h>
#include <runtime/ServiceRepository.h>
#include <runtime/ServiceLoader.h>



namespace GWSP {

ServicesRuntime::ServicesRuntime()
{
}

ServicesRuntime::~ServicesRuntime()
{
}


bool ServicesRuntime::initialize()
{
    _serviceRepositoryPtr = new ServiceRepository();
    _serviceLoaderPtr = new ServiceLoader();

    if (!_serviceRepositoryPtr->loadAllServices(*_serviceLoaderPtr))
    {
        ACE_DEBUG((LM_DEBUG, "service repository load all services failed\n"));

        return false;
    }

    if (!_serviceRepositoryPtr->InitializeAllServices())
    {
        ACE_DEBUG((LM_DEBUG, "service repository initialize all services failed\n"));

        return false;
    }
    
    return true;
}

bool ServicesRuntime::unInitialize()
{
    return true;
}

bool ServicesRuntime::start()
{
    if (!_serviceRepositoryPtr->startAllServices())
    {
        ACE_DEBUG((LM_DEBUG, "service repository start all services failed\n"));

        return false;
    }
    
    return true;
}


bool ServicesRuntime::stop()
{
    return true;
}

}

