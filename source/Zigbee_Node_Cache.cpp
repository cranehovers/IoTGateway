/*
* This file defines Cache
*/

#include "Zigbee_Node_Cache.h"
#include "Zigbee_Node.h"

ZigbeeNodeCache::ZigbeeNodeCache()
{
}

ZigbeeNodeCache::~ZigbeeNodeCache()
{
}

void ZigbeeNodeCache::add(ZigbeeNodeKey *key, ZigbeeNode *node)
{
    node_cache_.insert(std::pair<ZigbeeNodeKey*, ZigbeeNode*>(key, node));
}

ZigbeeNode *ZigbeeNodeCache::find_node_by_shortaddr(unsigned char short_addr[2])
{
    std::map<ZigbeeNodeKey*, ZigbeeNode*>::iterator e;

    e = node_cache_.begin();

    for (; e != node_cache_.end(); ++e)
    {
        ZigbeeNodeKey* tmp_key = e->first;
        if (tmp_key->compare_by_short_addr(short_addr) == true)
        {
            return e->second;
        }
    }

    return 0;
}



