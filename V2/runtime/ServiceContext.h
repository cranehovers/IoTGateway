
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
    typedef toolkit::AutoPtr<ServiceContext> Ptr;
    
    
    ServiceContext(ServicesRuntime &runtime, ServiceRepository &repo);
    ~ServiceContext();

    ServicesRuntime &runtime() const;
    ServiceRepository &repo() const;

private:
    ServicesRuntime &_servicesRumtime;
    ServiceRepository &_seviceRepository;
};

inline ServicesRuntime &ServiceContext::runtime() const
{
    return _servicesRumtime;
}

inline ServiceRepository &ServiceContext::repo() const
{
    return _seviceRepository;
}



}


#endif //ServiceContext_H










