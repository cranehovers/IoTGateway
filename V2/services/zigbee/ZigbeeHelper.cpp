
#include <services/zigbee/ZigbeeHelper.h>


namespace Services {
namespace Zigbee {


void ZigbeeHelper::set(EventServicePtr ptr)
{
    _eventQ = ptr;
}

void ZigbeeHelper::set(ZigbeeNodeCachePtr ptr)
{
    _cache = ptr;
}

void ZigbeeHelper::set(ZigbeeNetworkPtr ptr)
{
    _network = ptr;
}

void ZigbeeHelper::set(ZigbeeFrameRouterPtr ptr)
{
    _router = ptr;
}

ZigbeeHelper::EventServicePtr ZigbeeHelper::eventQ () const
{
    return _eventQ;
}

ZigbeeHelper::ZigbeeNodeCachePtr ZigbeeHelper::cache () const
{
    return _cache;    
}

ZigbeeHelper::ZigbeeNetworkPtr ZigbeeHelper::network () const
{
    return _network;
}

ZigbeeHelper::ZigbeeFrameRouterPtr ZigbeeHelper::router () const
{
    return _router;
}

}
}
