#ifndef __SERVICECFG_H__
#define __SERVICECFG_H__

#include <vector>
#include <string>

class ServiceCfg
{
public:
    ServiceCfg();
    virtual ~ServiceCfg();

    int loadService();
    std::vector<std::string> &services();

protected:
private:

    std::vector<std::string> service_list_;
};

#endif // __SERVICECFG_H__
