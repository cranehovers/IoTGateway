

#include <services/event/EventService.h>
#include <services/event/EventNotifyHandler.h>
#include <services/zigbee/ZigbeeEventHandler.h>
#include <services/zigbee/ZigbeeCommand.h>
#include <services/zigbee/ZigbeeCommandParser.h>
#include <services/zigbee/Zigbee_Request.h>
#include <services/zigbee/Zigbee_Frame_Router.h>
#include <services/zigbee/Zigbee_Network.h>
#include <services/zigbee/ZigbeeHelper.h>



    
namespace Services {
namespace Zigbee {

ZigbeeEventHandler::ZigbeeEventHandler()                                               
:_parser(new ZigbeeCommandParser())
{
}

ZigbeeEventHandler::~ZigbeeEventHandler()
{
    delete _parser;
}


int ZigbeeEventHandler::handleEvent(int id, const ACE_Message_Block &b)
{    
    if (id == EventNotifyHandler::EventSerialportRecvData)
    {
        _parser->add_data((unsigned char*)b.base(), b.size());

        ZigbeeCommand *cmd = 0;
        ZigbeeFrame *response = 0;
                
        while ((cmd = _parser->get_frame()) != 0)
        {
            if (cmd)
            {
                response = new ZigbeeResponse();

                if (response)
                {
                    response->set_cmd(cmd);
                    response->get();

                    delete response;

                    response = 0;
                    cmd = 0;
                }
            }     
        }
    }
    else if (id == EventNotifyHandler::EventSerialPortOnline)
    {
        _zigbeeHelper::instance()->network()->create_topology();
    }
    else if (id == EventNotifyHandler::EventSerialPortOffline)
    {
        _zigbeeHelper::instance()->network()->clear();
        _zigbeeHelper::instance()->cache()->clear();

        // update protocol stack.
        delete _parser;
        _parser = new  ZigbeeCommandParser();   
        
    }
    else if (id == EventNotifyHandler::EventSerialPortLongOnline)
    {
        _zigbeeHelper::instance()->network()->update();
        
    }
    

    return 0;
}


}
}

