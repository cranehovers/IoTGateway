/*
* This file defines zigbee coap resource
*/

#include "Zigbee_CoAP_Resource.h"
#include "CoAP_Wrapper.h"
#include "CoAP_Resource.h"
#include "Zigbee_Node.h"
#include "coap.h"


typedef struct device_id_name_table
{ 
    unsigned char id0;
    unsigned char id1;
    const char *name;
}device_id_name_table;

static device_id_name_table zigbee_device_name_table[0xff] =
{ 
    {0x00,0x00,"OnOff Switch"},
    {0x00,0x01,"Level Control Switch" },
    {0x00,0x03,"OnOff Output" },
    {0x00,0x03,"Level Controllable Output" },
    {0x00,0x04,"Scene Selector"},
    {0x00,0x05,"Configuration Tool"},
    {0x00,0x06,"Remote Control"},
    {0x00,0x07,"Combined Interface" },
    {0x00,0x08,"Range Extender" },
    {0x00,0x09,"Mains Power Outlet" },
    {0x00,0x0A,"Door Lock" },
    {0x00,0x0B,"Door Lock Controller" },
    {0x00,0x0C,"Simple Sensor"},
    {0x00,0x0D,"Consumption Awareness Device" },
    {0x00,0x50,"Home Gateway" },
    {0x00,0x51,"Smart plug" },
    {0x00,0x52,"White Goods"},
    {0x00,0x53,"Meter Interface"},
    {0x01,0x01,"Dimmable Light"},
    {0x01,0x02,"Color Dimmable Light On/Off Light"},
    {0x01,0x03,"On/Off Light Switch"},
    {0x01,0x04,"Dimmer Switch"},
    {0x01,0x05,"Color Dimmer Switch"},
    {0x01,0x06,"Light Sensor"},
    {0x01,0x07,"Occupancy Sensor"}
};

static std::string find_device_type_by_id(unsigned char device[2])
{
    for (int i=0; i < 0xff; i++)
    {
        if(zigbee_device_name_table[i].id0 == device[0] &&
           zigbee_device_name_table[i].id1 == device[1] )

           return std::string(zigbee_device_name_table[i].name);
    }
    
    return "unknow";
}


ZigbeeCoapResource::ZigbeeCoapResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{

}

ZigbeeCoapResource::~ZigbeeCoapResource()
{
}

void ZigbeeCoapResource::set_zigbee_ep(unsigned char ep)
{
    zigbee_ep_ = ep;
}

void ZigbeeCoapResource::set_zigbee_node(ZigbeeNode *node)
{
    zigbee_node_ = node;
}

void ZigbeeCoapResource::set_zigbee_desc(NodeSimpleDesc *desc)
{
    zigbee_desc_ = desc;
}

static void addr_to_string(unsigned char ieee_addr[8], std::string &str_addr)
{
  
    char buf[0xff];
    ACE_OS::sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x",ieee_addr[0],
                                                          ieee_addr[1],
                                                          ieee_addr[2],
                                                          ieee_addr[3],
                                                          ieee_addr[4],
                                                          ieee_addr[5],
                                                          ieee_addr[6],
                                                          ieee_addr[7]
                                                          );
    str_addr = buf;        
}

void* ZigbeeCoapResource::Create()
{
    std::string str_addr;
    addr_to_string(zigbee_node_->get_ieee_addr(), str_addr);

    char buf[0xff];
    
    ACE_OS::sprintf(buf, "%s/%x", str_addr.c_str(), zigbee_ep_);
    std::string resource_uri = buf;
    std::string resource_rt = find_device_type_by_id(zigbee_desc_->deviceid_);
    ACE_OS::sprintf(buf, "%s-%x", str_addr.c_str(), zigbee_ep_);
    std::string resource_ep = buf;    
    ACE_OS::sprintf(buf, "</%s>;ct=41;rt=\"%s\";if=\"sensor\"", resource_uri.c_str(),resource_rt.c_str());
    std::string payload = buf;


    CoAP_Attr attr;

    SetURI(resource_uri);
    attr.insert(std::make_pair("rt",resource_rt));
    attr.insert(std::make_pair("et",resource_rt));
    attr.insert(std::make_pair("ep",resource_ep));

    SetAttr(attr);

    set_payload(payload);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    return rd_resource;
}

void ZigbeeCoapResource::handler_get(CoAPCallback &callback)
{
    ACE_DEBUG((LM_DEBUG, "call %d\n", this->zigbee_ep_));
    CoAPResource::handler_get(callback);
}

void ZigbeeCoapResource::handler_put(CoAPCallback &callback)
{
    ACE_DEBUG((LM_DEBUG, "call %d\n", this->zigbee_ep_));

    CoAPResource::handler_get(callback);
}

