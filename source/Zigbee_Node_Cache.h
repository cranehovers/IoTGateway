/*
* This file defines Cache for Node
*/

#ifndef __ZIGBEE_NODE_CACHE_H__
#define __ZIGBEE_NODE_CACHE_H__

#include "Config.h"
#include <map>

class ZigbeeNode;
class ZigbeeNodeKey;

class ZigbeeNodeCache
{
public:

    ZigbeeNodeCache();
    ~ZigbeeNodeCache();

    void add(ZigbeeNodeKey *key, ZigbeeNode *node);
    ZigbeeNode *find_node_by_shortaddr(unsigned char short_addr[2]);


private:

    std::map<ZigbeeNodeKey*, ZigbeeNode*> node_cache_;    
};

typedef ACE_Singleton<ZigbeeNodeCache, ACE_Mutex> gZigbeeNodeCache;

#endif //__ZIGBEE_NODE_CACHE_H__
