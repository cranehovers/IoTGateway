/*
* This file defines zigbee network
*/

#include <services/zigbee/Zigbee_Network.h>
#include <services/zigbee/Zigbee_Node.h>
#include <services/zigbee/Zigbee_Coordination.h>

namespace Services {
namespace Zigbee {


ZigbeeNetwork::ZigbeeNetwork()
{
    root_node_ = 0;
}

ZigbeeNetwork::~ZigbeeNetwork()
{

}

void ZigbeeNetwork::create_topology()
{
    if (root_node_ == 0)
    {
        root_node_ = new ZigbeeCoordination();
        root_node_->set();
    }
    
    root_node_->get_self_basic_info();
            
}

void ZigbeeNetwork::update()
{
    create_topology();
}


void ZigbeeNetwork::clear()
{
    root_node_ = 0;
}

}
}

