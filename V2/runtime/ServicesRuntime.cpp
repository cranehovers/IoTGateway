

#include <runtime/ServicesRuntime.h>
#include <runtime/ServiceRepository.h>
#include <runtime/ServiceLoader.h>
#include <runtime/ServiceContext.h>


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
    _servicesRuntime = this;
    _serviceContextPtr = new ServiceContext(_servicesRuntime, _serviceRepositoryPtr);

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
    return true;
}

ServicesRuntime::ServiceRepositoryPtr&
ServicesRuntime::getRepository()
{
    return _serviceRepositoryPtr;
}

}

