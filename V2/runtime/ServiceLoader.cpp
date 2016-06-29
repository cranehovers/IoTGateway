
#include <runtime/ServiceLoader.h>

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
    return 0;
}

Service *ServiceLoader::get(int index)
{

    return 0;
}


}
