
#include <runtime/ServiceLoader.h>
#include <runtime/ServiceContext.h>
#include <runtime/Service.h>
#include <runtime/ServiceRepository.h>
#include <runtime/NullService.h>

namespace GWSP {

ServiceRepository::ServiceRepository()
{
}

ServiceRepository::~ServiceRepository()
{
}

bool ServiceRepository::initialize()
{
    return true;
}

bool ServiceRepository::unInitialize()
{
    return true;
}


bool ServiceRepository::loadAllServices(ServiceLoader &loader, ServiceContext &context)
{
    for (int i =0; i < loader.count(); i++)
    {
        Service *s = loader.create(i, context);

        add(s->name(), s);
    }

    return true;
}


bool ServiceRepository::add(std::string &name, Service *s)
{

    typedef std::pair<std::string, ServicePtr> ServicePair; 

    {
        ACE_Guard<SeviceMutex> guard(_serviceMutex);        
        _servicesMap.insert(ServicePair(name, ServicePtr(s)));
    }
    
    ACE_DEBUG((LM_DEBUG, "add service: %s\n", name.c_str()));

    return true;
}

bool ServiceRepository::startAllServices()
{    
    std::map<std::string, ServicePtr>::iterator e;

    {
        ACE_Guard<SeviceMutex> guard(_serviceMutex);        

        for (e = _servicesMap.begin(); e != _servicesMap.end(); ++e)
        {
            ACE_DEBUG((LM_DEBUG, "start service: %s\n", e->first.c_str()));
            
            if (!e->second->start())
            {
                ACE_DEBUG((LM_DEBUG, "the service %s start failed\n", e->first.c_str()));

                return false;
            }  
        }
    }

    return true;
}

bool ServiceRepository::stopAllServices()
{    
    std::map<std::string, ServicePtr>::iterator e;

    {
        ACE_Guard<SeviceMutex> guard(_serviceMutex);        

        for (e = _servicesMap.begin(); e != _servicesMap.end(); ++e)
        {
            ACE_DEBUG((LM_DEBUG, "stop service: %s\n", e->first.c_str()));
            
            if (!e->second->stop())
            {
                ACE_DEBUG((LM_DEBUG, "the service %s start failed\n", e->first.c_str()));

                return false;
            }  
        }
    }

    return true;
}

bool ServiceRepository::InitializeAllServices()
{
    std::map<std::string, ServicePtr>::iterator e;

    {
        ACE_Guard<SeviceMutex> guard(_serviceMutex);        

        for (e = _servicesMap.begin(); e != _servicesMap.end(); ++e)
        {
            ACE_DEBUG((LM_DEBUG, "initialize service: %s\n", e->first.c_str()));
            
            if (!e->second->initialize())
            {
                ACE_DEBUG((LM_DEBUG, "the service %s initialize failed\n", e->first.c_str()));

                return false;
            }
        }
    }
    
    return true;
}

Service &ServiceRepository::get(std::string &name)
{
    {
        ACE_Guard<SeviceMutex> guard(_serviceMutex);

        std::map<std::string, ServicePtr>::iterator e =
        _servicesMap.find(name);

        if (e != _servicesMap.end())
        {
            return (*(e->second.get()));
        }
        else // NullService, it always exist.
        {
            std::string null("service.null");
            
             e = _servicesMap.find(null);

             return (*(e->second.get()));
        }
    }    
}


}
