/*
* This file defines CoAP for zigbee
*/

#ifndef __ZIGBEE_COAP_SERVICE_H__
#define __ZIGBEE_COAP_SERVICE_H__

#include "Config.h"
#include <list>

class CoAPWrapper;
class CfgService;
class NetService;
class ZigbeeNode;
class NodeSimpleDesc;
class CoAPResource;

class ZigbeeCoAPService : public ACE_Event_Handler
{
public:

    ZigbeeCoAPService(CfgService *conf, NetService *net);
    ~ZigbeeCoAPService();

    static ZigbeeCoAPService *instance();
    static ZigbeeCoAPService *instance_;
    
    int Init();
    int Close();

    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
    
    virtual ACE_HANDLE get_handle (void) const;

    void create_resource_by_zigbeenode(unsigned char ep,
                                       NodeSimpleDesc *desc,
                                       ZigbeeNode *node);
    void register_resource_to_rd(CoAPResource *r);

private:
    
    CfgService *conf_;
    NetService *net_;
    CoAPWrapper *coap_wrapper_;

    std::list<CoAPResource*> zigbee_coap_resource_list_;
};

#endif //__ZIGBEE_COAP_SERVICE_H__
