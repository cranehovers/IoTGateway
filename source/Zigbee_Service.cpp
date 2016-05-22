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
#include "Zigbee_Serialport_Monitor.h"


ZigbeeService::ZigbeeService(CfgService *conf,
                             NetService *net)
:conf_(conf),
net_(net)
{
    coap_svc_ = 0;
    serialport_ = 0;
    network_ = 0;
    serial_port_monitor_ = 0;

}

ZigbeeService::~ZigbeeService()
{
}

int ZigbeeService::Init()
{
    if (conf_->enable_zigbee_ == true)
    {
        serial_port_monitor_ =  new SerialPortMonitor();

        if (serial_port_monitor_ == 0 )
        {

            return -1;
        }

        serial_port_monitor_->set_zigbee_service(this);
        serial_port_monitor_->set_cfg_service(conf_);
        serial_port_monitor_->activate();
    }

    return 0;
}

int ZigbeeService::Close()
{
    if(conf_->enable_zigbee_ == true)
    {
        if ( serial_port_monitor_ > 0)
        {
               serial_port_monitor_->clear();

               delete serial_port_monitor_;
               serial_port_monitor_ = 0;
        }

        if (serialport_)
        {
            serialport_->Close();

            delete serialport_;

            serialport_ = 0;
        }

        if (network_)
        {
            delete network_;
            network_ = 0;
        }

        if (coap_svc_)
        {
            coap_svc_->Close();
            delete coap_svc_;
            coap_svc_ = 0;
        }
    }

    return 0;
}


void ZigbeeService::serialport_plugin()
{
    if (coap_svc_ > 0 || serialport_ > 0 || network_ > 0)
    {
        ACE_DEBUG((LM_DEBUG, "Amazing.... what was happended.....\n"));
    }

    if (conf_->enable_zigbee_ == true)
    {
        coap_svc_ = new ZigbeeCoAPService(conf_, net_);
        ZigbeeCoAPService::instance_ = coap_svc_;

        if (coap_svc_->Init() == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "failed to init coap in zigbee sevice\n"));

            return ;
        }

        serialport_ = new ZigbeeSerialportService(conf_, net_);
        ZigbeeSerialportService::instance_ = serialport_;

        if (serialport_->Init() == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "failed to init serialport at zigbee sevice\n"));

            return ;
        }

        network_ = new ZigbeeNetwork();

        serialport_->register_notify_node(network_);

    }
}

void ZigbeeService::serialport_unplugin()
{
    if(conf_->enable_zigbee_ == true)
    {
        ACE_DEBUG((LM_DEBUG, "Delete all zigbee resource after serial prot unplugined\n"));

        if (serialport_)
        {
            serialport_->Close();

            delete serialport_;

            serialport_ = 0;
        }

        if (network_)
        {
            delete network_;
            network_ = 0;
        }

        if (coap_svc_)
        {
            coap_svc_->Close();

            delete coap_svc_;

            coap_svc_ = 0;
        }
    }
}

