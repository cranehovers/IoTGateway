

#ifndef ZigbeeHelper_H
#define ZigbeeHelper_H

#include <toolkit/ACE_Header_File.h>
#include <services/event/EventNotifyHandler.h>
#include <services/event/EventService.h>
#include <services/coapserver/Zigbee_CoAP_Service.h>
#include <services/zigbee/Zigbee_Network.h>
#include <services/zigbee/Zigbee_Frame_Router.h>
#include <services/zigbee/Zigbee_Node_Cache.h>


using Services::Event::EventService;
using Services::Event::EventNotifyHandler;
using Services::CoAPServer::ZigbeeServer;



namespace Services {
namespace Zigbee {


class ZigbeeHelper
{
public:

    typedef toolkit::AutoPtr<EventService> EventServicePtr;
    typedef toolkit::AutoPtr<ZigbeeNodeCache> ZigbeeNodeCachePtr;
    typedef toolkit::AutoPtr<ZigbeeNetwork> ZigbeeNetworkPtr;
    typedef toolkit::AutoPtr<ZigbeeFrameRouter> ZigbeeFrameRouterPtr;
    typedef toolkit::AutoPtr<ZigbeeServer> ZigbeeServerPtr;
        

    void set (EventServicePtr ptr);
    void set (ZigbeeNodeCachePtr ptr);
    void set (ZigbeeNetworkPtr ptr);
    void set (ZigbeeFrameRouterPtr ptr);
    void set (ZigbeeServerPtr ptr);
    
    
    EventServicePtr         eventQ () const;
    ZigbeeNodeCachePtr      cache () const;
    ZigbeeNetworkPtr        network () const;
    ZigbeeFrameRouterPtr    router () const;
    ZigbeeServerPtr         sever () const;
    
    
private:

    EventServicePtr      _eventQ; 
    ZigbeeNodeCachePtr   _cache;
    ZigbeeNetworkPtr     _network;
    ZigbeeFrameRouterPtr _router;
    ZigbeeServerPtr      _server;
};

typedef ACE_Singleton<ZigbeeHelper, ACE_Mutex> _zigbeeHelper;

}
}


#endif //ZigbeeHelper_H
