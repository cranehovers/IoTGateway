
#include <runtime/ServiceContext.h>
#include <runtime/ServiceRepository.h>
#include <runtime/ServicesRuntime.h>


namespace GWSP {

ServiceContext::ServiceContext(ServicesRuntimePtr &runtime, ServiceRepositoryPtr &repo)
:_servicesRumtimePtr(runtime),
 _seviceRepositoryPtr(repo)
{
}

ServiceContext::~ServiceContext()
{
}


}





