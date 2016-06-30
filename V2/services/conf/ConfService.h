
#ifndef ConfService_H
#define ConfService_H

#include <runtime/Service.h>

namespace Services{
namespace Conf {

class ConfService : public GWSP::Service
{

public:
    typedef toolkit::AutoPtr<ConfService> Ptr;
    
    ConfService(GWSP::ServiceContext &context);
    ~ConfService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();


    std::string _mcastAddr;
    std::string _rdAddr;
    std::string _svcAddr;
    std::string _proxyAddr;
    std::string _serialPort;
      
    int         _mcastAddrPort;
    int         _svcAddrPort;
    int         _proxyAddrPort;
    int         _rdAddrPort;
    int         _coapDebugLevel;    
    int         _baudrate;
        

    bool _enableProxy;
    bool _enableRd;    
    bool _enableZigbee;

private:
    void defaultValue();
    
};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}




#endif //ConfService_H




















