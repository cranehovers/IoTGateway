/*
* This file defines zigbee coap resource
*/

#ifndef __ZIGBEE_COAP_RESOURCE_H__
#define __ZIGBEE_COAP_RESOURCE_H__

#include <toolkit/CoAP_Resource.h>
#include <services/zigbee/Zigbee_Node.h>    
    
using toolkit::CoAPWrapper;
using toolkit::CoAPCallback;
using toolkit::CoAPResource;
using toolkit::CoAPCallback;

using Services::Zigbee::ZigbeeNode;
using Services::Zigbee::NodeSimpleDesc;

namespace Services {
namespace CoAPServer {


class ZigbeeCoapResource : public CoAPResource
{
public:
    
    ZigbeeCoapResource(CoAPWrapper* ctx);
    virtual ~ZigbeeCoapResource();

    void set_zigbee_ep(unsigned char ep);
    void set_zigbee_node(ZigbeeNode *node);
    void set_zigbee_desc(NodeSimpleDesc *desc);

    virtual void* Create();    
    virtual void handler_get(CoAPCallback &callback);
    virtual void handler_put(CoAPCallback &callback);
    virtual void do_on_off_cmd(unsigned char id);
    virtual void do_identify(unsigned char id, unsigned short time_value);

private:

    unsigned char zigbee_ep_;
    ZigbeeNode *zigbee_node_;
    NodeSimpleDesc *zigbee_desc_;
    
};

}
}


#endif //__ZIGBEE_COAP_RESOURCE_H__

