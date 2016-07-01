
#ifndef COAPRDProxy_SERVICE_H
#define COAPRDProxy_SERVICE_H

#include <runtime/Service.h>
#include <toolkit/CoAP_Wrapper.h>


using toolkit::CoAPWrapper;
using Services::Conf::ConfService;
using Services::Reactor::ReactorService;


namespace Services {
namespace CoAPProxy {

class CoAPProxyService : public GWSP::Service, public ACE_Event_Handler
{

public:
    typedef toolkit::AutoPtr<CoAPProxyService> Ptr;
    
    CoAPProxyService(GWSP::ServiceContext &context);
    ~CoAPProxyService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();

    
    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
    
    virtual ACE_HANDLE get_handle (void) const;


private:
    typedef toolkit::AutoPtr<CoAPWrapper> CoAPWrapperPtr;
    typedef toolkit::AutoPtr<ConfService> ConfServicePtr;
    typedef toolkit::AutoPtr<ReactorService> ReactorServicePtr;
    
    
    CoAPWrapperPtr _coapWrapperPtr;
    ConfServicePtr _confPtr;
    ReactorServicePtr _reactorPtr;
    
    
    
};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}

#endif //COAPRDProxy_SERVICE_H


