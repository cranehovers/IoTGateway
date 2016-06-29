
#include <runtime/ServiceLoader.h>
#include <runtime/ServiceFactory.h>
#include <runtime/ServiceContext.h>


namespace GWSP {

ServiceLoader::ServiceLoader()
{
}

ServiceLoader::~ServiceLoader()
{
}

bool ServiceLoader::initialize()
{
    return true;
}

bool ServiceLoader::unInitialize()
{
    return true;
}

int ServiceLoader::count()
{
    return serviceFactorySingleton::instance()->count();
}

Service *ServiceLoader::create(int index, ServiceContext &context)
{
    return serviceFactorySingleton::instance()->get(index, context);
}


}
