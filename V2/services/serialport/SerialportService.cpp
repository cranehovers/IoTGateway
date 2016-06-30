
#include <runtime/ServiceContext.h>
#include <runtime/ServiceRepository.h>
#include <runtime/ServicesRuntime.h>
#include <runtime/ServiceGetter.h>
#include <services/serialport/SerialportEventHandler.h>
#include <services/serialport/SerialDevice.h>
#include <services/conf/ConfService.h>
#include <services/reactor/ReactorService.h>
#include <services/serialport/SerialportService.h>


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
    _serialDevicePtr = new SerialDevice();
    _eventHandlerPtr = new SerialportEventHandler(*_serialDevicePtr.get());

    try
    {
        std::string conf("service.conf");
        std::string reactor("service.reactor");
        
        _confPtr = ServiceGetter::findByName<ConfService>(context(), conf);
        _reactorPtr = ServiceGetter::findByName<ReactorService>(context(), reactor);

    }                                                           
    catch(toolkit::NullPointerException e)
    {
        ACE_DEBUG((LM_DEBUG, "get service failed at SerialPortService\n"));

        return false;
    }
    
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

        // register the event handler into reactor
        _reactorPtr->register_handler(_eventHandlerPtr.get(),
                                    ACE_Event_Handler::READ_MASK);


        // monitor serialport online or offline
        #define ONLINE  1
        #define OFFLINE 0
        
        int portStatus = ONLINE;
        int currentStatus = ONLINE;

        while(!_stopped)
        {
            currentStatus = (_serialDevicePtr->online() == true) ? (ONLINE):(OFFLINE);

            if (currentStatus  == portStatus )
            {
                ACE_OS::sleep(1);

                continue;               
            }
            else
            {
                portStatus = currentStatus;

                if (portStatus == OFFLINE)
                {
                    _reactorPtr->remove_handler(_eventHandlerPtr.get());
                    _serialDevicePtr->close();

                    ACE_DEBUG((LM_DEBUG, "serialport offline\n"));

                    break;                    
                }
            }
            
        }
        
    }
    


    return 0;
}




}
}








