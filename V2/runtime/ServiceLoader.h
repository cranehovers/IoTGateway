
#ifndef ServiceLoader_H
#define ServiceLoader_H

#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>


namespace GWSP {

class Service;
class ServiceContext;

class ServiceLoader : public toolkit::RefCountedObject
{

public:
    typedef toolkit::AutoPtr<ServiceLoader> Ptr;
    
    ServiceLoader();
    virtual ~ServiceLoader();

    bool initialize();
    bool unInitialize();

    int count();
    Service *create(int index, ServiceContext &context);

private:

    ServiceLoader(ServiceLoader &other);
    ServiceLoader &operator=(ServiceLoader &other);

};



}



#endif //ServiceLoader_H