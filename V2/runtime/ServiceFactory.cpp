

#include <runtime/ServiceFactory.h>
#include <runtime/Service.h>
#include <runtime/NullService.h>
#include <runtime/ServiceContext.h>
#include <services/event/EventService.h>
#include <services/console/ConsoleService.h>
#include <services/conf/ConfService.h>
#include <services/reactor/ReactorService.h>
#include <services/serialport/SerialportService.h>
#include <services/coapserver/Zigbee_CoAP_Service.h>
#include <services/zigbee/ZigbeeService.h>
#include <services/coaprd/CoAPRdService.h>
#include <services/coapproxy/CoAPProxyService.h>





namespace GWSP {


// set a create funtion table to register service's instance funtion

typedef Service *(*instance)(ServiceContext &context);

instance servicesInstanceFunctionTable[] = {
    nullServiceInstance,
    Services::Conf::instance,
    Services::Zigbee::instance,
    Services::CoAPRD::instance,
    Services::CoAPProxy::instance,
    Services::CoAPServer::instance,
    Services::SerialPort::instance,
    Services::Reactor::instance,
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

