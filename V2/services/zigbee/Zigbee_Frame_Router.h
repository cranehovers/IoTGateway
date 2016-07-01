/*
* This file defines frame
*/

#ifndef __ZIGBEE_FRMAE_ROUTER_H__
#define __ZIGBEE_FRMAE_ROUTER_H__

#include <toolkit/ReferenceCountObject.h>

namespace Services {
namespace Zigbee {

class ZigbeeFrame;

class ZigbeeFrameRouter :public toolkit::RefCountedObject
{
public:
    ZigbeeFrameRouter();
    ~ZigbeeFrameRouter();

    void dump(unsigned char *buf, unsigned char len);
    int handle_response(ZigbeeFrame *resp);
    
};

}
}

#endif //__ZIGBEE_FRMAE_ROUTER_H__
