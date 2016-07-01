/*
* This file defines Request
*/


#include <services/zigbee/Zigbee_Frame_Router.h>
#include <services/zigbee/ZigbeeCommand.h>
#include <services/zigbee/Zigbee_Request.h>
#include <services/zigbee/ZigbeeHelper.h>
#include <services/event/EventNotifyHandler.h>


using Services::Event::EventNotifyHandler;



namespace Services{
namespace Zigbee{

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

void ZigbeeFrame::set_cmd(ZigbeeCommand *cmd)
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
    ACE_Message_Block *b = new ACE_Message_Block(this->size());

    if(b)
    {
        ACE_OS::memcpy(b->base(), this->base(), this->size());
        b->msg_type(EventNotifyHandler::EventSerialportSendData);

        if ( -1 == _zigbeeHelper::instance()->eventQ()->putQ(b))
        {
            b->release();
        }

        delete this;
    }
    else
    {
        delete this;
    }

    return 0;
}

int ZigbeeResponse::get()
{
    return _zigbeeHelper::instance()->router()->handle_response(this);
}




}
}




