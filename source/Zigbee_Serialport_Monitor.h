/*
* The serial port monitor
*/

#ifndef __ZIGBEE_SERIAL_PORT_MONITOR_H__
#define __ZIGBEE_SERIAL_PORT_MONITOR_H__

#include "Config.h"


class ZigbeeService;
class CfgService;


class SerialPortMonitor :public ACE_Task_Base
{
public:

    SerialPortMonitor()
    {
        zigbee_service_ = 0;
        thread_exit_flag = 0;
        conf_ = 0;
    };

    ~SerialPortMonitor()
    {
        zigbee_service_ = 0;
        conf_ = 0;
    };

    void start();

    void set_zigbee_service(ZigbeeService *svc)
    {
        zigbee_service_ = svc;
    }

    void set_cfg_service(CfgService *cfg)
    {
        conf_ = cfg;
    }

    virtual int svc (void);
    void clear();

private:

    ZigbeeService *zigbee_service_;
    CfgService *conf_;

    unsigned short thread_exit_flag;

};




#endif //__ZIGBEE_SERIAL_PORT_MONITOR_H__
