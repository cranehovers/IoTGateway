
#include <runtime/ServiceGetter.h>
#include <services/conf/ConfService.h>
#include <services/reactor/ReactorService.h>
#include <toolkit/CoAP_Wrapper.h>
#include <toolkit/CoAP_Resource.h>
#include <services/coapproxy/CoAPProxyService.h>


using namespace GWSP;


namespace Services {
namespace CoAPProxy {

GWSP::Service *instance(GWSP::ServiceContext &context){return new CoAPProxyService(context);}

static std::string serviceName("service.coapproxy");


CoAPProxyService::CoAPProxyService(GWSP::ServiceContext &context)
:Service(context)
{
}

CoAPProxyService::~CoAPProxyService()
{
}

std::string &CoAPProxyService::name()
{
    return serviceName;
}

bool CoAPProxyService::initialize()
{
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


    _coapWrapperPtr = new CoAPWrapper();
    
    if (_coapWrapperPtr->Create(_confPtr->_proxyAddr, 
                              _confPtr->_proxyAddrPort,
                              _confPtr->_coapDebugLevel
                              ) < 0)
    {

        ACE_DEBUG((LM_DEBUG,"Failed to create coap\n"));
        return false;
    }


    _coapWrapperPtr->enable_proxy();  

    
    return true;
}

bool CoAPProxyService::unInitialize()
{
    return true;
}

bool CoAPProxyService::start()
{
    ACE_Time_Value timeout;
    timeout.sec(5);

    _reactorPtr->register_handler(this, ACE_Event_Handler::READ_MASK);
    _reactorPtr->schedule_timer(this, 0, timeout);
    

    return true;
}

bool CoAPProxyService::stop()
{
    _reactorPtr->remove_handler(this);
    _reactorPtr->cancel_timer(this);
    
    return true;
}

int CoAPProxyService::handle_input (ACE_HANDLE fd)
{   
    _coapWrapperPtr->handle_event();
    return 0;
}

int CoAPProxyService::handle_timeout (const ACE_Time_Value &tv,
                                            const void *arg)
{
    ACE_Time_Value timeout;
    timeout.sec(5);
    
    _coapWrapperPtr->time_out(timeout);
    _reactorPtr->schedule_timer(this, 0, timeout);
    
    return 0;

}


ACE_HANDLE CoAPProxyService::get_handle () const
{
    return _coapWrapperPtr->get_handle();
}




}
}





