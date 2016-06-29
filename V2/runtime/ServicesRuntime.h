#ifndef SERVICESRUNTIME_H
#define SERVICESRUNTIME_H

#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>


namespace GWSP{

class ServiceRepository;
class ServiceLoader;

class ServicesRuntime : public toolkit::RefCountedObject
{
public:
    ServicesRuntime();
    virtual ~ServicesRuntime();

    bool initialize();
    bool unInitialize();
    bool start();
    bool stop();
    

private:

    typedef toolkit::AutoPtr<ServiceRepository>  ServiceRepositoryPtr;    
    typedef toolkit::AutoPtr<ServiceLoader>  ServiceLoaderPtr;

    ServiceRepositoryPtr _serviceRepositoryPtr;
    ServiceLoaderPtr     _serviceLoaderPtr;
    
};

} //namespace GWSP

#endif // SERVICESRUNTIME_H
