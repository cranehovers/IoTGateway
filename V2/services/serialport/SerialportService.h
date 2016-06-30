
#ifndef SerialPort_SERVICE_H
#define SerialPort_SERVICE_H

#include <runtime/Service.h>


using Services::Conf::ConfService;
using Services::Reactor::ReactorService;

namespace Services {
namespace SerialPort {

class SerialDevice;
class SerialportEventHandler;

class SerialPortService : public GWSP::Service, public ACE_Task_Base
{

public:
    SerialPortService(GWSP::ServiceContext &context);
    ~SerialPortService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();
    virtual int svc (void);

private:

    typedef toolkit::AutoPtr<SerialDevice> SerialDevicePtr;
    typedef toolkit::AutoPtr<SerialportEventHandler> SerialportEventHandlerPtr;
    typedef toolkit::AutoPtr<ConfService> ConfServicePtr;
    typedef toolkit::AutoPtr<ReactorService> ReactorServicePtr;
    
    
    bool _stopped;
    SerialDevicePtr _serialDevicePtr;
    SerialportEventHandlerPtr _eventHandlerPtr;
    ConfServicePtr _confPtr;
    ReactorServicePtr _reactorPtr;
    
};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}



#endif //SerialPort_SERVICE_H

