/*
 * This file implements Service Manager
 */

#include "ServiceManager.h"
#include "ServiceCfg.h"
#include "Service.h"

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
        ServiceManager::instance_ = new ServiceManager();

        /*
         * when instance created , then load services from cfg file
         */
        ServiceManager::instance_->loadService();
    }

    return ServiceManager::instance_;
}

int ServiceManager::loadService()
{
    service_cfg_->loadService();
    createService();

    return 0;
}

int ServiceManager::createService()
{
    std::vector<std::string> &services_string = service_cfg_->services();

    std::vector<std::string>::iterator e = services_string.begin();

    for (; e != services_string.end(); e++)
    {
        Service *svc = new Service();
        services_.push_back(svc);

        svc->load((*e));
    }

    return 0;
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
        svc->close();
    }

    return 0;
}


