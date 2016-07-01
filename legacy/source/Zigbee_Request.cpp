/*
* This file defines Request
*/

#include "Zigbee_Request.h"
#include "Zigbee_Serialport_Command.h"
#include "Zigbee_Serialport_Service.h"
#include "Zigbee_Frame_Router.h"


ZigbeeFrame::ZigbeeFrame()
{
    cmd_ = 0;
}

ZigbeeFrame::~ZigbeeFrame()
{
    if (cmd_)
    {
        delete cmd_;
    }
}

void ZigbeeFrame::set_cmd(ZigbeeSerialportCommand *cmd)
{
    cmd_ = cmd;
}

unsigned char *ZigbeeFrame::base()
{
    if (cmd_)
    {
        return cmd_->base();
    }
    
    return 0; 
}

unsigned int ZigbeeFrame::size()
{
    if (cmd_)
    {
        return cmd_->size();
    }
    
    return 0;     
}

unsigned char ZigbeeFrame::cmd0()
{
    if (base())
    {
        return base()[0x02];
    }
    
    return 0;     

}

unsigned char ZigbeeFrame::cmd1()
{
    if (base())
    {
        return base()[0x03];
    }
    
    return 0;      
}


int ZigbeeRequest::get()
{
    return ZigbeeSerialportService::instance()->send(this);
}

int ZigbeeResponse::get()
{
    return gZigbeeFrameRouter::instance()->route(this);
}







