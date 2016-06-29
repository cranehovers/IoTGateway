
#ifndef ServiceRepository_H
#define ServiceRepository_H


#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>
#include <map>

namespace GWSP {

class ServiceLoader;
class Service;
class ServiceContext;

class ServiceRepository : public toolkit::RefCountedObject
{

public:
    typedef toolkit::AutoPtr<Service> ServicePtr;
    
    ServiceRepository();
    virtual ~ServiceRepository();

    bool initialize();
    bool unInitialize();
    bool startAllServices();
    bool InitializeAllServices();
    bool loadAllServices(ServiceLoader &loader, ServiceContext &context);
    bool add(std::string &name, Service *s);
    ServicePtr &get(std::string &name);

private:
    ServiceRepository(ServiceRepository &other);
    ServiceRepository &operator=(ServiceRepository &other);

    typedef ACE_Recursive_Thread_Mutex SeviceMutex;
    
    std::map<std::string, ServicePtr> _servicesMap;
    SeviceMutex _serviceMutex;
    
};



}

#endif //ServiceRepository_H
