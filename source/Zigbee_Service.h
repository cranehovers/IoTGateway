/*
* Zigbee service
*/

#ifndef __ZIGBEE_SERVICE_H__
#define __ZIGBEE_SERVICE_H__

#include "Config.h"

class CfgService;
class NetService;
class ZigbeeSerialportService;
class ZigbeeNetwork;
class ZigbeeCoAPService;

class ZigbeeService
{
public:

    ZigbeeService(CfgService *conf, NetService *net);
    ~ZigbeeService();

    int Init();
    int Close();
    
private:

    CfgService *conf_;
    NetService *net_;
    ZigbeeSerialportService *serialport_;
    ZigbeeNetwork *network_;
    ZigbeeCoAPService *coap_svc_;
};




#endif //__ZIGBEE_SERVICE_H__
