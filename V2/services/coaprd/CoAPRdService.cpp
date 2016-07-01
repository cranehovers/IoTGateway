
#include <runtime/ServiceGetter.h>
#include <services/conf/ConfService.h>
#include <services/reactor/ReactorService.h>
#include <toolkit/CoAP_Wrapper.h>
#include <toolkit/CoAP_Resource.h>
#include <services/coaprd/CoAPRdService.h>
#include <services/coaprd/CoAP_RD_Resource.h>
#include <services/coaprd/CoAP_RD_Lookup_Resource.h>


using namespace GWSP;


namespace Services {
namespace CoAPRD {

GWSP::Service *instance(GWSP::ServiceContext &context){return new CoAPRDService(context);}

static std::string serviceName("service.coaprd");


CoAPRDService::CoAPRDService(GWSP::ServiceContext &context)
:Service(context)
{
}

CoAPRDService::~CoAPRDService()
{
}

std::string &CoAPRDService::name()
{
    return serviceName;
}

bool CoAPRDService::initialize()
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
    
    if (_coapWrapperPtr->Create(_confPtr->_rdAddr, 
                              _confPtr->_rdAddrPort,
                              _confPtr->_coapDebugLevel
                              ) < 0)
    {

        ACE_DEBUG((LM_DEBUG,"Failed to create coap\n"));
        return false;
    }
    
    _rdRes = new CoAP_RD_Resource(_coapWrapperPtr.get());

    if ((_rdRes->Create()) == 0)
    {
        ACE_DEBUG((LM_DEBUG,"Failed to craete rd resource\n"));
        return false;
    }

    _rdLookup = new CoAPRDLookUpResource(_coapWrapperPtr.get());

    if ((_rdLookup->Create()) == 0)
    {
        ACE_DEBUG((LM_DEBUG,"Failed to craete lookup resource\n"));
        return false;
    }

    ACE_Time_Value timeout;
    timeout.sec(5);

    _reactorPtr->register_handler(this, ACE_Event_Handler::READ_MASK);
    _reactorPtr->schedule_timer(this, 0, timeout);
    

    return true;
}

bool CoAPRDService::unInitialize()
{
    return true;
}

bool CoAPRDService::start()
{
    return true;
}

bool CoAPRDService::stop()
{
    _reactorPtr->remove_handler(this);
    _reactorPtr->cancel_timer(this);
    
    return true;
}

int CoAPRDService::handle_input (ACE_HANDLE fd)
{   
    ACE_DEBUG((LM_DEBUG,"CoAPRDService::handle_input\n"));

    _coapWrapperPtr->handle_event();
    return 0;
}

int CoAPRDService::handle_timeout (const ACE_Time_Value &tv,
                                            const void *arg)
{
    ACE_Time_Value timeout;
    timeout.sec(5);
    
    _coapWrapperPtr->time_out(timeout);
    _reactorPtr->schedule_timer(this, 0, timeout);
    
    return 0;

}


ACE_HANDLE CoAPRDService::get_handle () const
{
    ACE_DEBUG((LM_DEBUG,"CoAPRDService::get_handle\n"));
    
    return _coapWrapperPtr->get_handle();
}




}
}




