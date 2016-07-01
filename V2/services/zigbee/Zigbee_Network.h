/*
* This file defines Zigbee network.
*/

#ifndef __ZIGBEE_NETWORK_H__
#define __ZIGBEE_NETWORK_H__

#include <toolkit/ReferenceCountObject.h>

namespace Services {
namespace Zigbee {

class ZigbeeNode;

class ZigbeeNetwork  :public toolkit::RefCountedObject
{
public:

    ZigbeeNetwork();
    ~ZigbeeNetwork();

    void create_topology();
    void clear();
    void update();
    
 private:

    ZigbeeNode *root_node_;
};


}
}

#endif //__ZIGBEE_NETWORK_H__
