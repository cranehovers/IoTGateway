
#include <runtime/ServiceContext.h>
#include <services/event/EventService.h>

namespace Services {
namespace Event {


GWSP::Service *instance(GWSP::ServiceContext &context)
{
    return new EventService(context);
}

static std::string serviceName("service.event");


EventService::EventService(GWSP::ServiceContext &context)
:Service(context)
{
}

EventService::~EventService()
{
}

std::string &EventService::name()
{
    return serviceName;
}

bool EventService::initialize()
{
    return true;
}

bool EventService::unInitialize()
{
    return true;
}

bool EventService::start()
{
    return true;
}

bool EventService::stop()
{
    return true;
}




}
}








