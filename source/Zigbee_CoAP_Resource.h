/*
* This file defines zigbee coap resource
*/

#ifndef __ZIGBEE_COAP_RESOURCE_H__
#define __ZIGBEE_COAP_RESOURCE_H__

#include "Config.h"
#include "CoAP_Resource.h"


class CoAPWrapper;
class ZigbeeNode;
class NodeSimpleDesc;

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

private:

    unsigned char zigbee_ep_;
    ZigbeeNode *zigbee_node_;
    NodeSimpleDesc *zigbee_desc_;
    
};



#endif //__ZIGBEE_COAP_RESOURCE_H__

