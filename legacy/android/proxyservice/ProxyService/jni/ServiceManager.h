/*
 * This file defines Service Manager that manage the services from CFG file.
 */

#ifndef __SERVICE_MANAGER_H__
#define __SERVICE_MANAGER_H__

#include <vector>
#include "ServiceLog.h"

class ServiceCfg;
class Service;

class ServiceManager
{
public:
    ServiceManager();
    virtual ~ServiceManager();
    static ServiceManager *instance();

    int startService();
    int stopService();

    int loadService();
    int createService();
    int initService();
    void setPrintOutput(print_log printlog);

private:
    static ServiceManager *instance_;
    ServiceCfg *service_cfg_;
    std::vector<Service*> services_;
};


#endif //__SERVICE_MANAGER_H__
