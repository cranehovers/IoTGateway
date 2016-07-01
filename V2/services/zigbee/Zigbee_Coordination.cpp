/*
*   This file defines Coordination
*/

#include <toolkit/ACE_Header_File.h>
#include <services/zigbee/ZigbeeCommand.h>
#include <services/zigbee/Zigbee_Request.h>
#include <services/zigbee/Zigbee_Coordination.h>


namespace Services {
namespace Zigbee {



ZigbeeCoordination::ZigbeeCoordination()
{
    unsigned char short_addr[2] = {0x0,0x0};

    set_node_type(Coordination);
    set_short_addr(short_addr);
}

ZigbeeCoordination::~ZigbeeCoordination()
{
}

void ZigbeeCoordination::clear()
{
    std::list<ZigbeeDevice*>::iterator e;

    e = child_list_.begin();
    while(e != child_list_.end())
    {
        ZigbeeDevice *d = (*e);
        delete d;
    }
}

void ZigbeeCoordination::create_device()
{
    unsigned char device_count = get_child_count();

    for (int i = 0; i < device_count; i++)
    {
        unsigned char short_addr[2];

        short_addr[0] = get_child_list()[i];
        short_addr[1] = get_child_list()[(++i)];

        ZigbeeDevice *device = new ZigbeeDevice();

        device->set_short_addr(short_addr);
        device->set_bind_ep(get_bind_ep());

        if ( true == device->set())
        {
            ACE_HEX_DUMP((LM_DEBUG, (const char*)short_addr, 2, "\n\nnew node:"));

            child_list_.push_back(device);

            device->get_self_basic_info();
        }
        else
        {
            delete device;
        }  
    }

}


}
}


