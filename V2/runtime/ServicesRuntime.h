#ifndef SERVICESRUNTIME_H
#define SERVICESRUNTIME_H

#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>


namespace GWSP{

class ServiceRepository;
class ServiceLoader;
class ServiceContext;

class ServicesRuntime : public toolkit::RefCountedObject
{
public:
    typedef toolkit::AutoPtr<ServiceRepository>  ServiceRepositoryPtr;    
    typedef toolkit::AutoPtr<ServiceLoader>  ServiceLoaderPtr;
    typedef toolkit::AutoPtr<ServiceContext>  ServiceContextPtr;
    typedef toolkit::AutoPtr<ServicesRuntime>  ServicesRuntimePtr;
    
    ServicesRuntime();
    virtual ~ServicesRuntime();

    bool initialize();
    bool unInitialize();
    bool start();
    bool stop();

    ServiceRepositoryPtr &getRepository();
    
private:

    ServiceRepositoryPtr _serviceRepositoryPtr;
    ServiceLoaderPtr     _serviceLoaderPtr;
    ServiceContextPtr    _serviceContextPtr;
    ServicesRuntimePtr   _servicesRuntime;
    
};

} //namespace GWSP

#endif // SERVICESRUNTIME_H
