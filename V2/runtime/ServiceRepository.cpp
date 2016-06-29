

#include <runtime/ServiceRepository.h>
#include <runtime/ServiceLoader.h>
#include <runtime/Service.h>


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


bool ServiceRepository::loadAllServices(ServiceLoader &loader)
{
    for (int i =0; i < loader.count(); i++)
    {
        Service *s = loader.get(i);

        add(s->name(), s);
    }

    return true;
}


bool ServiceRepository::add(std::string &name, Service *s)
{

    typedef std::pair<std::string, ServicePtr> ServicePair; 

    _servicesMap.insert(ServicePair(name, ServicePtr(s)));

    return true;
}

bool ServiceRepository::startAllServices()
{    
    std::map<std::string, ServicePtr>::iterator e;

    for (e = _servicesMap.begin(); e != _servicesMap.end(); ++e)
    {
        if (!e->second->start())
        {
            ACE_DEBUG((LM_DEBUG, "the service %s start failed\n", e->first.c_str()));

            return false;
        }
    }

    return true;
}

bool ServiceRepository::InitializeAllServices()
{
    std::map<std::string, ServicePtr>::iterator e;

    for (e = _servicesMap.begin(); e != _servicesMap.end(); ++e)
    {
        if (!e->second->initialize())
        {
            ACE_DEBUG((LM_DEBUG, "the service %s initialize failed\n", e->first.c_str()));

            return false;
        }
    }

    return true;
}

}
