/*
* This file defines Request
*/

#ifndef __ZIGBEE_REQUEST_H__
#define __ZIGBEE_REQUEST_H__

namespace Services{
namespace Zigbee{

class ZigbeeCommand;

class ZigbeeFrame
{
public:

    ZigbeeFrame();
    virtual ~ZigbeeFrame();

    virtual int get() = 0;
    void set_cmd(ZigbeeCommand *cmd);
    unsigned char *base();
    unsigned int size();
    unsigned char cmd0();
    unsigned char cmd1();

private:

    ZigbeeCommand *cmd_;
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


}
}

#endif //__ZIGBEE_REQUEST_H__
