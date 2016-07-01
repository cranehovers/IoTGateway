
#include <runtime/ServiceContext.h>
#include <runtime/ServiceRepository.h>
#include <runtime/ServicesRuntime.h>
#include <runtime/ServiceGetter.h>
#include <services/conf/ConfService.h>
#include <services/reactor/ReactorService.h>
#include <services/event/EventService.h>
#include <services/event/EventNotifyHandler.h>
#include <services/serialport/SerialportService.h>
#include <services/serialport/SerialportEventHandler.h>
#include <services/serialport/SerialDevice.h>



using GWSP::ServiceGetter;

namespace Services {
namespace SerialPort {


GWSP::Service *instance(GWSP::ServiceContext &context)
{
    return new SerialPortService(context);
}

static std::string serviceName("service.serialport");


SerialPortService::SerialPortService(GWSP::ServiceContext &context)
:Service(context),
_stopped(false)
{
}

SerialPortService::~SerialPortService()
{
}

std::string &SerialPortService::name()
{
    return serviceName;
}

bool SerialPortService::initialize()
{
    try
    {
        std::string conf("service.conf");
        std::string reactor("service.reactor");
        std::string eventQ("service.event");
        
        _confPtr = ServiceGetter::findByName<ConfService>(context(), conf);
        _reactorPtr = ServiceGetter::findByName<ReactorService>(context(), reactor);
        _eventQPtr = ServiceGetter::findByName<EventService>(context(), eventQ);

    }                                                           
    catch(toolkit::NullPointerException e)
    {
        ACE_DEBUG((LM_DEBUG, "get service failed at SerialPortService\n"));

        return false;
    }

    _serialDevicePtr = new SerialDevice();
    _eventHandlerPtr = new SerialportEventHandler(*_serialDevicePtr, *_eventQPtr);

    // set event handler into event queue to get data from upper layer
    _eventQPtr->set(EventNotifyHandler::EventSerialportSendData, *_eventHandlerPtr);
    
    return true;
}

bool SerialPortService::unInitialize()
{
    return true;
}

bool SerialPortService::start()
{
    // find conf service to get some parameter
    // from configration file
    {
        _serialDevicePtr->setProperty( _confPtr->_serialPort, _confPtr->_baudrate);
    }
    
    if (activate() == -1)
    {
        ACE_DEBUG((LM_DEBUG, "serialport service thread start failed\n"));
        
        return false;
    }

    return true;
}

bool SerialPortService::stop()
{
    _reactorPtr->remove_handler(_eventHandlerPtr.get());
    
    _stopped = true;
    wait();
    
    return true;
}

int SerialPortService::svc()
{
    _stopped = false;

    while (!_stopped)
    {
        if (!_serialDevicePtr->open())
        {
            ACE_OS::sleep(1);

            continue;
        }
        
        ACE_DEBUG((LM_DEBUG, "serialport online\n"));

        // notify the serialport online
        {
            ACE_Message_Block *b = new ACE_Message_Block();
            b->msg_type(EventNotifyHandler::EventSerialPortOnline);

            _eventQPtr->putQ(b);
        }

        // register the event handler into reactor
        _reactorPtr->register_handler(_eventHandlerPtr.get(),
                                    ACE_Event_Handler::READ_MASK);


        // monitor serialport online or offline
        #define ONLINE  1
        #define OFFLINE 0
        
        int portStatus = ONLINE;
        int currentStatus = ONLINE;
        int count_tick = 0;

        while(!_stopped)
        {
            currentStatus = (_serialDevicePtr->online() == true) ? (ONLINE):(OFFLINE);

            if (currentStatus  == portStatus )
            {
                if(currentStatus == ONLINE )
                {
                    count_tick ++;

                    if (count_tick > 20 )
                    {
                        count_tick = 0;
                        
                        // update the network
                        {
                            ACE_Message_Block *b = new ACE_Message_Block();
                            b->msg_type(EventNotifyHandler::EventSerialPortLongOnline);
                        
                            _eventQPtr->putQ(b);
                        }
                    }
                }
                
                ACE_OS::sleep(1);

                continue;               
            }
            else
            {
                portStatus = currentStatus;
                count_tick = 0;

                if (portStatus == OFFLINE)
                {
                    _reactorPtr->remove_handler(_eventHandlerPtr.get());
                    _serialDevicePtr->close();

                    ACE_DEBUG((LM_DEBUG, "serialport offline\n"));

                    // notify the serialport offline
                    {
                        ACE_Message_Block *b = new ACE_Message_Block();
                        b->msg_type(EventNotifyHandler::EventSerialPortOffline);

                        _eventQPtr->putQ(b);
                    }

                    break;                    
                }
            }
            
        }
        
    }
    
    return 0;
}




}
}








