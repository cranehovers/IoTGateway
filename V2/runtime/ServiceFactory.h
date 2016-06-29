
#ifndef ServiceFactory_H
#define ServiceFactory_H

#include <toolkit/ACE_Header_File.h>

namespace GWSP {

class Service;
class ServiceContext;

class ServiceFactory
{
public:
    ServiceFactory();
    ~ServiceFactory();
    
    int count();
    Service *get(int index, ServiceContext &context);
};

// Use ACE Singleton wrapper Gateway object.
typedef ACE_Singleton<ServiceFactory, ACE_Mutex> serviceFactorySingleton;

}


#endif //ServiceFactory_H

