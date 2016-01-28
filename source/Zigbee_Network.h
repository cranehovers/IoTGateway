/*
* This file defines Zigbee network.
*/

#ifndef __ZIGBEE_NETWORK_H__
#define __ZIGBEE_NETWORK_H__

#include "Config.h"
#include "Zigbee_Notify.h"

class ZigbeeNode;

class ZigbeeNetwork : public ZigbeeNotify
{
public:

    ZigbeeNetwork();
    ~ZigbeeNetwork();

    void create_topology();
    virtual void notify(unsigned short n);
    
 private:

    ZigbeeNode *root_node_;
};




#endif //__ZIGBEE_NETWORK_H__