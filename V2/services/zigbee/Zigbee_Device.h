/*
* This file defines device
*/
#ifndef __ZIGBEE_DEVICE_H__
#define __ZIGBEE_DEVICE_H__

    
#include <services/zigbee/Zigbee_Node.h>


namespace Services {
namespace Zigbee {
    
class ZigbeeDevice :public ZigbeeNode
{
public:

    ZigbeeDevice();
    ~ZigbeeDevice();

    virtual void get_self_basic_info();
    virtual void get_self_ep_count();
    virtual void get_self_ep_desc();
};

}
}

#endif //__ZIGBEE_DEVICE_H__

