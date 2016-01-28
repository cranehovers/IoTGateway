/*
*  This file define Zigbee service
*/

#include "Zigbee_Serialport_Service.h"
#include "CfgService.h"
#include "NET_Service.h"
#include "Zigbee_Service.h"
#include "Zigbee_Serialport_Command.h"
#include "Zigbee_Network.h"
#include "Zigbee_CoAP_Service.h"


ZigbeeService::ZigbeeService(CfgService *conf, 
                             NetService *net)
:conf_(conf),
net_(net)
{
}

ZigbeeService::~ZigbeeService()
{
}

int ZigbeeService::Init()
{
    if (conf_->enable_zigbee_ == true)
    {
        coap_svc_ = new ZigbeeCoAPService(conf_, net_);
        ZigbeeCoAPService::instance_ = coap_svc_;

        if (coap_svc_->Init() == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "failed to init coap in zigbee sevice\n"));

            return -1;
        }

        serialport_ = new ZigbeeSerialportService(conf_, net_);
        ZigbeeSerialportService::instance_ = serialport_;

        if (serialport_->Init() == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "failed to init serialport at zigbee sevice\n"));

            return -1;
        }

        network_ = new ZigbeeNetwork(); 
        
        serialport_->register_notify_node(network_);

    }
    
    return 0;
}

int ZigbeeService::Close()
{
    if(conf_->enable_zigbee_ == true)
    {
        if (serialport_)
        {
            serialport_->Close();

            delete serialport_;
        }

        if (network_)
        {
            delete network_;
        }

        if (coap_svc_)
        {
            delete coap_svc_;
        }
    }
    
    return 0;
}

