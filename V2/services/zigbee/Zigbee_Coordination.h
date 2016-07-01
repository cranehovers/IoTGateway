/*
* This file defines Coordination
*/

#ifndef __ZIGBEE_COORDINATION_H__
#define __ZIGBEE_COORDINATION_H__


#include <services/zigbee/Zigbee_Device.h>
#include <list>


namespace Services {
namespace Zigbee {


class ZigbeeCoordination :public ZigbeeDevice
{
public:

    ZigbeeCoordination();
    ~ZigbeeCoordination();

    virtual void create_device();

private:

    void clear();

    std::list<ZigbeeDevice*> child_list_;
};

}
}

#endif //__ZIGBEE_COORDINATION_H__
