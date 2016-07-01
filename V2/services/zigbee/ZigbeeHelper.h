

#ifndef ZigbeeHelper_H
#define ZigbeeHelper_H

#include <toolkit/ACE_Header_File.h>
#include <services/event/EventNotifyHandler.h>
#include <services/event/EventService.h>
#include <services/zigbee/Zigbee_Network.h>
#include <services/zigbee/Zigbee_Frame_Router.h>
#include <services/zigbee/Zigbee_Node_Cache.h>


using Services::Event::EventService;
using Services::Event::EventNotifyHandler;


namespace Services {
namespace Zigbee {


class ZigbeeHelper
{
public:

    typedef toolkit::AutoPtr<EventService> EventServicePtr;
    typedef toolkit::AutoPtr<ZigbeeNodeCache> ZigbeeNodeCachePtr;
    typedef toolkit::AutoPtr<ZigbeeNetwork> ZigbeeNetworkPtr;
    typedef toolkit::AutoPtr<ZigbeeFrameRouter> ZigbeeFrameRouterPtr;

    void set (EventServicePtr ptr);
    void set (ZigbeeNodeCachePtr ptr);
    void set (ZigbeeNetworkPtr ptr);
    void set (ZigbeeFrameRouterPtr ptr);
    
    EventServicePtr         eventQ () const;
    ZigbeeNodeCachePtr      cache () const;
    ZigbeeNetworkPtr        network () const;
    ZigbeeFrameRouterPtr    router () const;
    
private:

    EventServicePtr      _eventQ; 
    ZigbeeNodeCachePtr   _cache;
    ZigbeeNetworkPtr     _network;
    ZigbeeFrameRouterPtr _router; 
};

typedef ACE_Singleton<ZigbeeHelper, ACE_Mutex> _zigbeeHelper;

}
}


#endif //ZigbeeHelper_H
