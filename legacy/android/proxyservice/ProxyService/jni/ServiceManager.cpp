/*
 * This file implements Service Manager
 */

#include "ServiceManager.h"
#include "ServiceCfg.h"
#include "Service.h"
#include "ServiceLog.h"

ServiceManager *ServiceManager::instance_ = 0;

ServiceManager::ServiceManager()
    :service_cfg_(0)
{
    if (service_cfg_ == 0)
    {

        service_cfg_ = new ServiceCfg();
    }
}

ServiceManager::~ServiceManager()
{
    if (service_cfg_ != 0)
    {
        delete service_cfg_;
        service_cfg_ = 0;
    }

    std::vector<Service*>::iterator e = services_.begin();

    for (; e != services_.end(); e++)
    {
        Service *svc = (*e);
        delete svc;
    }

}

ServiceManager *ServiceManager::instance()
{
    if (ServiceManager::instance_ == 0)
    {
        //printOutput("Create ServiceManager");
        ServiceManager::instance_ = new ServiceManager();
    }

    return ServiceManager::instance_;
}

int ServiceManager::loadService()
{
    service_cfg_->loadService();

    return createService();
}

int ServiceManager::createService()
{
    std::vector<std::string> &services_string = service_cfg_->services();

    std::vector<std::string>::iterator e = services_string.begin();

    for (; e != services_string.end(); e++)
    {
        Service *svc = new Service();
        services_.push_back(svc);

        if (svc->load((*e)) < 0)
        {
            //printOutput("create Service failed\n");
            return -1;
        }
    }

    //printOutput("create Service successfully\n");

    return 0;
}


int ServiceManager::initService()
{
    std::vector<Service*>::iterator e = services_.begin();

    for (; e != services_.end(); e++)
    {
        Service *svc = (*e);
        svc->init();
    }

    return 0;
}

void ServiceManager::setPrintOutput(print_log printlog)
{
    std::vector<Service*>::iterator e = services_.begin();

    for (; e != services_.end(); e++)
    {
        Service *svc = (*e);
        svc->setPrintOutput(printlog);
    }

    return;
}


int ServiceManager::startService()
{
    std::vector<Service*>::iterator e = services_.begin();

    for (; e != services_.end(); e++)
    {
        Service *svc = (*e);
        svc->start();
    }

    return 0;
}

int ServiceManager::stopService()
{
    std::vector<Service*>::iterator e = services_.begin();

    for (; e != services_.end(); e++)
    {
        Service *svc = (*e);
        svc->stop();

        /*
        *  FIXME: if it need to do close action here ?
        */
        //svc->close();
    }

    return 0;
}


