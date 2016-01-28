/*
* This file defines frame
*/

#ifndef __ZIGBEE_FRMAE_ROUTER_H__
#define __ZIGBEE_FRMAE_ROUTER_H__

#include "Config.h"

class ZigbeeResponse;

class ZigbeeFrameRouter :public ACE_Task_Base
{
public:
    ZigbeeFrameRouter();
    ~ZigbeeFrameRouter();

    int route(ZigbeeResponse *resp);
    void dump(unsigned char *buf, unsigned char len);

    virtual int svc (void);
    
    void clear();

private:

    int handle_response(ZigbeeResponse *resp);
    
    unsigned short thread_exit_flag;    
    ACE_Message_Queue<ACE_MT_SYNCH> message_queue;
};

typedef ACE_Singleton<ZigbeeFrameRouter, ACE_Mutex> gZigbeeFrameRouter;

#endif //__ZIGBEE_FRMAE_ROUTER_H__
