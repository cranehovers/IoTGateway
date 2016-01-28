
/*
* This file define zigbee notify
*/

#ifndef __ZIGBEE_NOTIFY_H__
#define __ZIGBEE_NOTIFY_H__


#define ZIGBEE_NOTIFY_SERIALPORT_CONNECTED 0x10


class ZigbeeNotify
{
public:

    virtual void notify(unsigned short n) = 0;
    
};

#endif //__ZIGBEE_NOTIFY_H__