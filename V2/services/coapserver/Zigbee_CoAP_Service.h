/*
* This file defines CoAP for zigbee
*/

#ifndef __ZIGBEE_COAP_SERVICE_H__
#define __ZIGBEE_COAP_SERVICE_H__

#include <runtime/Service.h>
#include <toolkit/CoAP_Wrapper.h>
#include <services/conf/ConfService.h>
#include <services/reactor/ReactorService.h>
#include <services/zigbee/Zigbee_Node.h>
#include <list>

using toolkit::CoAPWrapper;
using toolkit::CoAPResource;
using Services::Conf::ConfService;
using Services::Reactor::ReactorService;
using Services::Zigbee::ZigbeeNode;
using Services::Zigbee::NodeSimpleDesc;


namespace Services {
namespace CoAPServer {

class ZigbeeServer : public GWSP::Service, public ACE_Event_Handler
{
public:
    typedef toolkit::AutoPtr<ZigbeeServer> Ptr;
    
    ZigbeeServer(GWSP::ServiceContext &context);
    ~ZigbeeServer();

    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();

    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
    
    virtual ACE_HANDLE get_handle (void) const;

    void create_resource_by_zigbeenode(unsigned char ep,
                                       NodeSimpleDesc *desc,
                                       ZigbeeNode *node);
    void register_resource_to_rd(CoAPResource *r);

    void reset();

private:
    typedef toolkit::AutoPtr<CoAPWrapper> CoAPWrapperPtr;
    typedef toolkit::AutoPtr<ConfService> ConfServicePtr;
    typedef toolkit::AutoPtr<ReactorService> ReactorServicePtr;
    
    
    CoAPWrapperPtr _coapWrapperPtr;
    ConfServicePtr _confPtr;
    ReactorServicePtr _reactorPtr;

    std::list<CoAPResource*> zigbee_coap_resource_list_;
};


GWSP::Service *instance(GWSP::ServiceContext &context);



}
}

#endif //__ZIGBEE_COAP_SERVICE_H__
