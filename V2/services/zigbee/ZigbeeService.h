
#ifndef ZigbeeService_H
#define ZigbeeService_H

#include <runtime/Service.h>

using Services::Event::EventService;
using Services::Event::EventNotifyHandler;
using Services::CoAPServer::ZigbeeServer;



namespace Services {
namespace Zigbee {

class ZigbeeEventHandler;
class ZigbeeFrameRouter;
class ZigbeeNodeCache;
class ZigbeeNetwork;

class ZigbeeService : public GWSP::Service
{

public:
    typedef toolkit::AutoPtr<ZigbeeService> Ptr;

    ZigbeeService(GWSP::ServiceContext &context);
    ~ZigbeeService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();

private:
    typedef toolkit::AutoPtr<EventService> EventServicePtr;
    typedef toolkit::AutoPtr<ZigbeeEventHandler> ZigbeeEventHandlerPtr;
    typedef toolkit::AutoPtr<ZigbeeFrameRouter> ZigbeeFrameRouterPtr;
    typedef toolkit::AutoPtr<ZigbeeNodeCache> ZigbeeNodeCachePtr;
    typedef toolkit::AutoPtr<ZigbeeNetwork> ZigbeeNetworkPtr;
    typedef toolkit::AutoPtr<ZigbeeServer> ZigbeeServerPtr;
    

    
    EventServicePtr _eventQPtr;
    ZigbeeEventHandlerPtr _eventHandlerPtr;
    ZigbeeFrameRouterPtr _frameRouterPtr;
    ZigbeeNodeCachePtr _cachePtr;
    ZigbeeNetworkPtr _network;
    ZigbeeServerPtr _coapServerPtr;

};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}


#endif //ZigbeeService_H





























