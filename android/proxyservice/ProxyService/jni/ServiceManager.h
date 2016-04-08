/*
 * This file defines Service Manager that manage the services from CFG file.
 */

#ifndef __SERVICE_MANAGER_H__
#define __SERVICE_MANAGER_H__

class ServiceManager
{
public:
	ServiceManager();
	virtual ~ServiceManager();
	static ServiceManager *instance();

	int startService();
	int stopService();


private:
	int loadService();

	static ServiceManager *instance_;
};


#endif //__SERVICE_MANAGER_H__
