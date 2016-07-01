/*
* This file defines Coordination
*/

#ifndef __ZIGBEE_COORDINATION_H__
#define __ZIGBEE_COORDINATION_H__

#include "Config.h"
#include "Zigbee_Node.h"
#include "Zigbee_Device.h"
#include <list>

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


#endif //__ZIGBEE_COORDINATION_H__
