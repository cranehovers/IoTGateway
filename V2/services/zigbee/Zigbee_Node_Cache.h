/*
* This file defines Cache for Node
*/

#ifndef __ZIGBEE_NODE_CACHE_H__
#define __ZIGBEE_NODE_CACHE_H__

#include <toolkit/ReferenceCountObject.h>
#include <map>

namespace Services {
namespace Zigbee {


class ZigbeeNode;
class ZigbeeNodeKey;

class ZigbeeNodeCache :public toolkit::RefCountedObject
{
public:

    ZigbeeNodeCache();
    ~ZigbeeNodeCache();

    void add(ZigbeeNodeKey *key, ZigbeeNode *node);
    ZigbeeNode *find_node_by_shortaddr(unsigned char short_addr[2]);
    void clear();


private:

    std::map<ZigbeeNodeKey*, ZigbeeNode*> node_cache_;
};

}
}

#endif //__ZIGBEE_NODE_CACHE_H__
