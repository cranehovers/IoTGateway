/*
*   This file defines device
*/

#include "Zigbee_Device.h"
#include "Zigbee_Request.h"
#include "Zigbee_Serialport_Command.h"


ZigbeeDevice::ZigbeeDevice()
{
    set_node_type(Device);
}

ZigbeeDevice::~ZigbeeDevice()
{
}

void ZigbeeDevice::get_self_basic_info()
{
    unsigned char req_type = 0x01;
    unsigned char start_index = 0x0;
    
    ZigbeeSerialportCommand *cmd = 
    ZigbeeSerialportCommand::create_IEEE_ADDR_cmd(get_short_addr(),req_type, start_index);

    ZigbeeRequest *req = new ZigbeeRequest();
    req->set_cmd(cmd);

    req->get();

    ACE_DEBUG((LM_DEBUG,"get_self_basic_info(%x%x):\n",get_short_addr()[0],get_short_addr()[1]));
}

void ZigbeeDevice::get_self_ep_count()
{
    ZigbeeSerialportCommand *cmd = 
    ZigbeeSerialportCommand::create_ACTIVE_EP_cmd(get_short_addr(),get_short_addr());

    ZigbeeRequest *req = new ZigbeeRequest();
    req->set_cmd(cmd);

    req->get();

    ACE_DEBUG((LM_DEBUG,"get_self_ep_count(%x%x):\n",get_short_addr()[0],get_short_addr()[1]));
}

void ZigbeeDevice::get_self_ep_desc()
{
    unsigned char *ep_list = get_ep_list();

    if (ep_list == 0)
    {
        ACE_DEBUG((LM_DEBUG,"get_self_ep_desc(not find ep list):\n"));
    }
    
    for (int i=0; i < get_ep_count(); i++)
    {        
        ZigbeeSerialportCommand *cmd = 
        ZigbeeSerialportCommand::create_EP_SIMPLE_DESC_cmd(get_short_addr(),get_short_addr(),ep_list[i]);

        ZigbeeRequest *req = new ZigbeeRequest();
        req->set_cmd(cmd);

        req->get();

        ACE_DEBUG((LM_DEBUG,"get_self_ep_desc(ep=%x):\n",ep_list[i]));
    }
}

