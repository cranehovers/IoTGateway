
#ifndef ServiceContext_H
#define ServiceContext_H

#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>


namespace GWSP {

class ServicesRuntime;
class ServiceRepository;


class ServiceContext : public toolkit::RefCountedObject
{
public:
    typedef toolkit::AutoPtr<ServicesRuntime> ServicesRuntimePtr;
    typedef toolkit::AutoPtr<ServiceRepository> ServiceRepositoryPtr;
    
    ServiceContext(ServicesRuntimePtr &runtime, ServiceRepositoryPtr &repo);
    ~ServiceContext();

private:
    ServicesRuntimePtr &_servicesRumtimePtr;
    ServiceRepositoryPtr &_seviceRepositoryPtr;
    
};


}


#endif //ServiceContext_H










