/*
 * This file implements Service Manager
 */

#include "ServiceManager.h"

ServiceManager *ServiceManager::instance_ = 0;

ServiceManager::ServiceManager()
{
}

ServiceManager::~ServiceManager()
{
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
	return 0;
}

int ServiceManager::startService()
{
	return 0;
}

int ServiceManager::stopService()
{
	return 0;
}


