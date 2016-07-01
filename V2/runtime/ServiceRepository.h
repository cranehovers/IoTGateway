
#ifndef ServiceRepository_H
#define ServiceRepository_H


#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>
#include <map>


namespace GWSP {


class Service;
class ServiceLoader;
class ServiceContext;
class Service;

class ServiceRepository : public toolkit::RefCountedObject
{

public:
    typedef toolkit::AutoPtr<ServiceRepository> Ptr;
    
    
    ServiceRepository();
    virtual ~ServiceRepository();

    bool initialize();
    bool unInitialize();
    bool startAllServices();
    bool stopAllServices();
    bool InitializeAllServices();
    bool preInitializeAllServices();
    bool loadAllServices(ServiceLoader &loader, ServiceContext &context);
    bool add(std::string &name, Service *s);
    Service &get(std::string &name);

private:
    ServiceRepository(ServiceRepository &other);
    ServiceRepository &operator=(ServiceRepository &other);

    typedef ACE_Recursive_Thread_Mutex SeviceMutex;
    typedef toolkit::AutoPtr<Service> ServicePtr;
    
    std::map<std::string, ServicePtr> _servicesMap;
    SeviceMutex _serviceMutex;
    
};



}

#endif //ServiceRepository_H
