/*
* This file defines Request
*/

#ifndef __ZIGBEE_REQUEST_H__
#define __ZIGBEE_REQUEST_H__

#include "Config.h"


class ZigbeeSerialportCommand;

class ZigbeeFrame
{
public:

    ZigbeeFrame();
    virtual ~ZigbeeFrame();

    void set_cmd(ZigbeeSerialportCommand *cmd);
    virtual int get() = 0;
    unsigned char *base();
    unsigned int size();
    unsigned char cmd0();
    unsigned char cmd1();

private:

    ZigbeeSerialportCommand *cmd_;
};

class ZigbeeResponse : public ZigbeeFrame
{
public:
    virtual int get();
};

class ZigbeeRequest : public ZigbeeFrame
{
public:
    virtual int get();
};



#endif //__ZIGBEE_REQUEST_H__
