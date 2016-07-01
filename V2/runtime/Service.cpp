
#include <runtime/Service.h>
#include <runtime/ServiceContext.h>


namespace GWSP {

Service::Service(ServiceContext &context)
:_serviceContext(context)
{
}

Service::~Service()
{
}

bool Service::preInitialize()
{
    return true;
}
    
}










