
#include <runtime/ServiceRepository.h>
#include <runtime/ServiceLoader.h>
#include <runtime/ServiceContext.h>
#include <runtime/ServicesRuntime.h>


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
    _servicesRuntimePtr = this;
    _serviceContextPtr = new ServiceContext(*_servicesRuntimePtr, 
                                            *_serviceRepositoryPtr);

    if (!_serviceRepositoryPtr->loadAllServices(*_serviceLoaderPtr, *_serviceContextPtr))
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
    _serviceRepositoryPtr->stopAllServices();
    
    return true;
}

ServiceRepository&
ServicesRuntime::repo()
{
    return *_serviceRepositoryPtr;
}

ServiceContext&
ServicesRuntime::context()
{
    return *_serviceContextPtr;
}

}

