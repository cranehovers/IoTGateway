/*
* This file defines zigbee network
*/

#include "Zigbee_Network.h"
#include "Zigbee_Node.h"
#include "Zigbee_Coordination.h"


ZigbeeNetwork::ZigbeeNetwork()
{
    root_node_ = 0;
}

ZigbeeNetwork::~ZigbeeNetwork()
{
    if(root_node_)
    {
        delete root_node_;
    }
}

void ZigbeeNetwork::create_topology()
{
    if (root_node_ == 0)
    {
        root_node_ = new ZigbeeCoordination();
        root_node_->set();

        root_node_->get_self_basic_info();
        
    }
    /* FIXME: todo handle this exception*/
    else
    {
        ACE_DEBUG((LM_DEBUG, "root node have been created\n"));
    }
}

void ZigbeeNetwork::notify(unsigned short n)
{
    if (n == ZIGBEE_NOTIFY_SERIALPORT_CONNECTED)
    {
        create_topology();
    }
}


