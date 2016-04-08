/*
 * This file implements Service Manager
 */

#include "ServiceManager.h"
#include "ServiceCfg.h"


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
	return service_cfg_->loadService();
}

int ServiceManager::startService()
{
	return 0;
}

int ServiceManager::stopService()
{
	return 0;
}


