/*
* This file defines Gateway feature
*/

#ifndef __GATE_WAY_H__
#define __GATE_WAY_H__

#include "Config.h"


class CfgService;
class NetService;
class RDService;
class FWProxyService;
class ZigbeeService;

class Gateway
{
public:

    Gateway()
    {
        svc_conf_ = 0;
        svc_net_ = 0;
        svc_rd_ = 0;
        svc_proxy_ = 0;
        svc_zigbee_ = 0;
    }

    int Init();
    int Start();
    int Stop();
    int Close();

private:

    CfgService *svc_conf_;
    NetService *svc_net_;
    RDService  *svc_rd_;
    FWProxyService *svc_proxy_;
    ZigbeeService *svc_zigbee_;

};

// Use ACE Singleton wrapper Gateway object.
typedef ACE_Singleton<Gateway, ACE_Mutex> IoTGateway;

#endif /* __GATE_WAY_H__*/






