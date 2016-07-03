
#include <runtime/ServiceContext.h>
#include <runtime/ServiceRepository.h>
#include <runtime/ServicesRuntime.h>
#include <runtime/ServiceGetter.h>
#include <services/event/EventService.h>
#include <services/event/EventNotifyHandler.h>
#include <services/coapserver/Zigbee_CoAP_Service.h>
#include <services/zigbee/ZigbeeEventHandler.h>
#include <services/zigbee/ZigbeeService.h>
#include <services/zigbee/Zigbee_Frame_Router.h>
#include <services/zigbee/Zigbee_Node_Cache.h>
#include <services/zigbee/ZigbeeHelper.h>


using GWSP::ServiceGetter;

namespace Services {
namespace Zigbee {


GWSP::Service *instance(GWSP::ServiceContext &context)
{
    return new ZigbeeService(context);
}

static std::string serviceName("service.zigbee");


ZigbeeService::ZigbeeService(GWSP::ServiceContext &context)
:Service(context)
{
}

ZigbeeService::~ZigbeeService()
{
}

std::string &ZigbeeService::name()
{
    return serviceName;
}

bool ZigbeeService::initialize()
{
    try
    {
        std::string eventQ("service.event");
        std::string server("service.coapserver");

        _eventQPtr = ServiceGetter::findByName<EventService>(context(), eventQ);
        _coapServerPtr = ServiceGetter::findByName<ZigbeeServer>(context(), server);
        

    }                                                           
    catch(toolkit::NullPointerException &e)
    {
        ACE_DEBUG((LM_DEBUG, "get service failed at ZigbeeService\n"));

        return false;
    }

    _cachePtr = new ZigbeeNodeCache();
    _network = new ZigbeeNetwork();
    _frameRouterPtr = new ZigbeeFrameRouter();
    _eventHandlerPtr = new ZigbeeEventHandler();

    _eventQPtr->set(EventNotifyHandler::EventSerialportRecvData, *_eventHandlerPtr);
    _eventQPtr->set(EventNotifyHandler::EventSerialPortOffline, *_eventHandlerPtr);
    _eventQPtr->set(EventNotifyHandler::EventSerialPortOnline, *_eventHandlerPtr);
    _eventQPtr->set(EventNotifyHandler::EventSerialPortLongOnline, *_eventHandlerPtr);
    

    _zigbeeHelper::instance()->set(_cachePtr);
    _zigbeeHelper::instance()->set(_network);
    _zigbeeHelper::instance()->set(_frameRouterPtr);
    _zigbeeHelper::instance()->set(_eventQPtr);
    _zigbeeHelper::instance()->set(_coapServerPtr);
    
    
    return true;
}

bool ZigbeeService::unInitialize()
{
    return true;
}

bool ZigbeeService::start()
{
    return true;
}

bool ZigbeeService::stop()
{
    return true;
}





}
}










