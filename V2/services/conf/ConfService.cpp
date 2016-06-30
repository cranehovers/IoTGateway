#include <runtime/ServiceContext.h>
#include <services/conf/ConfService.h>

namespace Services {
namespace Conf {


GWSP::Service *instance(GWSP::ServiceContext &context)
{
    return new ConfService(context);
}

static std::string serviceName("service.conf");

static int getValue(const char* item, 
                    std::string &value, 
                    ACE_Configuration_Heap &confHeap
                    )
{
    ACE_Configuration_Section_Key key;
    std::string gateway_section("gateway");

    int code = confHeap.open_section(confHeap.root_section(),
                     gateway_section.c_str(),
                     0,
                     key);


    if (code < 0 )
    {
        ACE_DEBUG((LM_DEBUG,
        "failed to get section %s\n",gateway_section.c_str()));

        return -1;
    }

    ACE_TString v;
    int r = confHeap.get_string_value(key, item, v);

    if (r != -1 ) value = v.c_str();
    
    return r;

}

static int getValue(const char* item, 
                    int &value,
                    ACE_Configuration_Heap &confHeap
                    )
{
    std::string value_s;

    if (getValue(item, value_s, confHeap) >= 0 )
    {
        value = ACE_OS::atoi(value_s.c_str());
    }

    return 0;
}

static int getValue(const char* item, 
                    bool &value,
                    ACE_Configuration_Heap &confHeap
                    )
{
    std::string value_s;

    if (getValue(item, value_s, confHeap) >= 0 )
    {
        value = (::strcasecmp(
                value_s.c_str(), "true")
                ) == 0 ? true: false;
    }

    return 0;

}


ConfService::ConfService(GWSP::ServiceContext &context)
:Service(context)
{
}

ConfService::~ConfService()
{
}

std::string &ConfService::name()
{
    return serviceName;
}

bool ConfService::initialize()
{
    std::string iniName("gateway.conf");
    ACE_Configuration_Heap confHeap;
    ACE_Ini_ImpExp ini(confHeap);

    defaultValue();

    confHeap.open();
    
    if ( ini.import_config(iniName.c_str()) == -1)
    {
        ACE_DEBUG((LM_DEBUG,
        "failed to find config file %s. please check...\n",
        iniName.c_str()));
    }
    else // find a configuration, then load all values
    {
        getValue("multicast_addr", _mcastAddr, confHeap);
        getValue("multicast_addr_port", _mcastAddrPort, confHeap);
        getValue("server_addr", _svcAddr, confHeap);
        getValue("server_addr_port", _svcAddrPort, confHeap);
        getValue("proxy_addr", _proxyAddr, confHeap);
        getValue("proxy_addr_port", _proxyAddrPort, confHeap);
        getValue("rd_addr", _rdAddr, confHeap);
        getValue("rd_addr_port", _rdAddrPort, confHeap);

        getValue("enable_proxy", _enableProxy, confHeap);
        getValue("enable_rd", _enableRd, confHeap);
        getValue("coap_debug", _coapDebugLevel, confHeap);

        getValue("serial_port", _serialPort, confHeap);
        getValue("bautrate", _baudrate, confHeap);
        getValue("enable_zigbee", _enableZigbee, confHeap);       
    }

    return true;
}

bool ConfService::unInitialize()
{
    return true;
}

bool ConfService::start()
{
    return true;
}

bool ConfService::stop()
{
    return true;
}


void ConfService::defaultValue()
{
    _mcastAddr  = "ff05::fd";
    _rdAddr     = "::";
    _svcAddr    = "::";
    _proxyAddr  = "::";
    _serialPort = "/dev/ttyUSB0";
      
    _mcastAddrPort  = 5683;
    _svcAddrPort    = 6683;
    _proxyAddrPort  = 7683;
    _rdAddrPort     = 8683;
    _coapDebugLevel = 3;    
    _baudrate       = 38400;
        

    _enableProxy    = true;
    _enableRd       = true;    
    _enableZigbee   = true;

}


}
}





