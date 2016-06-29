

#include <runtime/NullService.h>


namespace GWSP {

static std::string nullServiceName("service.null");

Service *nullServiceInstance(ServiceContext &context)
{
    return new NullService(context);
}

std::string &NullService::name()
{
    return nullServiceName;
}


NullService::NullService(ServiceContext &context)
:Service(context)
{
}

NullService::~NullService()
{
}


}

