

#include <runtime/ServiceFactory.h>
#include <runtime/Service.h>
#include <runtime/NullService.h>
#include <runtime/ServiceContext.h>
#include <services/event/EventService.h>
#include <services/console/ConsoleService.h>
#include <services/conf/ConfService.h>



namespace GWSP {


// set a create funtion table to register service's instance funtion

typedef Service *(*instance)(ServiceContext &context);

instance servicesInstanceFunctionTable[] = {
    nullServiceInstance,
    Services::Conf::instance,
    Services::Event::instance,
    Services::Console::instance,
};




ServiceFactory::ServiceFactory()
{
}

ServiceFactory::~ServiceFactory()
{
}

int ServiceFactory::count()
{
    return (sizeof(servicesInstanceFunctionTable) / sizeof(servicesInstanceFunctionTable[0]));
}

Service *ServiceFactory::get(int index, ServiceContext &context)
{
    return servicesInstanceFunctionTable[index](context);
}

}

