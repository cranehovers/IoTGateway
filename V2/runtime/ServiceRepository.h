
#ifndef ServiceRepository_H
#define ServiceRepository_H


#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>
#include <map>

namespace GWSP {

class ServiceLoader;
class Service;

class ServiceRepository : public toolkit::RefCountedObject
{

public:
    ServiceRepository();
    virtual ~ServiceRepository();

    bool initialize();
    bool unInitialize();
    bool startAllServices();
    bool InitializeAllServices();
    bool loadAllServices(ServiceLoader &loader);
    bool add(std::string &name, Service *s);

private:
    ServiceRepository(ServiceRepository &other);
    ServiceRepository &operator=(ServiceRepository &other);

    typedef toolkit::AutoPtr<Service> ServicePtr;
    
    std::map<std::string, ServicePtr> _servicesMap;
    
};



}

#endif //ServiceRepository_H
