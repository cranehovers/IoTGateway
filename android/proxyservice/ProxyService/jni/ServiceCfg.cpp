#include "ServiceCfg.h"

#define IOT_GATEWAY_SHARE_LIB "libiotgateway.so"
#define IOT_CLOUD_CLIENT_SHARE_LIB "libiotcloudclient.so"

ServiceCfg::ServiceCfg()
{
    //ctor
}

ServiceCfg::~ServiceCfg()
{
    //dtor
}


int ServiceCfg::loadService()
{
    service_list_.clear();

    /*
    * maybe the service name should be setted into cfg file,but
    * now, only wrote into code
    */
    service_list_.push_back(IOT_GATEWAY_SHARE_LIB);
    service_list_.push_back(IOT_CLOUD_CLIENT_SHARE_LIB);

    return 0;
}

std::vector<std::string> &ServiceCfg::services()
{
    return service_list_;
}
