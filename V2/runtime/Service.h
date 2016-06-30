
#ifndef Service_H
#define Service_H


#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>
#include <toolkit/Exception.h>
#include <string>

namespace GWSP {

class ServiceContext;

class Service : public toolkit::RefCountedObject
{

public:
    typedef toolkit::AutoPtr<Service> Ptr;
    
    Service(ServiceContext &context);
    virtual ~Service();

    virtual std::string &name() = 0;
    virtual bool initialize() = 0;
    virtual bool unInitialize() = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;

    ServiceContext &context() const;

private:

    ServiceContext &_serviceContext;
};

inline ServiceContext &Service::context() const
{
    return _serviceContext; 
}

}

#endif //Service_H



