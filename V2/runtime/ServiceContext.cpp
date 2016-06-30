
#include <runtime/ServiceContext.h>
#include <runtime/ServiceRepository.h>
#include <runtime/ServicesRuntime.h>


namespace GWSP {

ServiceContext::ServiceContext(ServicesRuntime &runtime, ServiceRepository &repo)
:_servicesRumtime(runtime),
 _seviceRepository(repo)
{
}

ServiceContext::~ServiceContext()
{
}


}





