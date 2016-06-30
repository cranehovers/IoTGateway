
#ifndef SerialPort_SERVICE_H
#define SerialPort_SERVICE_H

#include <runtime/Service.h>



namespace Services {
namespace SerialPort {

class SerialPortService : public GWSP::Service
{

public:
    SerialPortService(GWSP::ServiceContext &context);
    ~SerialPortService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();
};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}



#endif //SerialPort_SERVICE_H

