
#include <dlfcn.h>
#include "Service.h"
#include "ace/Log_Msg.h"


typedef int (*service_action)();

Service::Service()
:service_handle_(0),
service_start_(0),
service_stop_(0)
{
    //ctor
}

Service::~Service()
{
    //dtor
}


int Service::load(std::string serviceName)
{
    service_name_ = serviceName;

    ACE_DEBUG((LM_DEBUG, "load service library: %s\n", service_name_.c_str()));

    service_handle_ = dlopen(service_name_.c_str(), RTLD_LAZY);

    if (service_handle_ == 0)
    {
        const char* error_message = dlerror();

        if (error_message != 0)
        {
            ACE_DEBUG((LM_DEBUG,"load servie: %s faied[%s]\n", service_name_.c_str(), error_message));
        }
        else
        {
            ACE_DEBUG((LM_DEBUG,"load servie: %s faied\n", service_name_.c_str()));
        }

        return -1;
    }


    service_start_ = dlsym(service_handle_, "startService");

    if (service_start_ == 0)
    {
        ACE_DEBUG((LM_DEBUG, "find symbol startService failed from %s\n", service_name_.c_str()));

        return -1;
    }

    service_stop_  = dlsym(service_handle_, "stopService");

    if (service_stop_ == 0)
    {
        ACE_DEBUG((LM_DEBUG, "find symbol stopService failed from %s\n", service_name_.c_str()));

        return -1;
    }

    return 0;
}

int Service::close()
{
    if (service_handle_)
    {
        dlclose(service_handle_);

        service_handle_ = 0;
        service_start_ = 0;
        service_stop_ = 0;
    }
}

int Service::svc()
{
    if (service_start_)
    {
        ACE_DEBUG((LM_DEBUG, "start the service %s in standalone thread\n", service_name_.c_str()));

        service_action action = (service_action)service_start_;
        /*
        *  if here need the custom module to block the thread???
        */
        (*action)();
    }
}

int Service::start()
{
    if (service_start_  == 0 )
    {
        return -1;
    }

    if (activate() == -1 )
    {
        return -1;
    }

    return 0;
}

int Service::stop()
{
    if (service_stop_  == 0 )
    {
        return -1;
    }

    service_action action = (service_action)service_stop_;
    return (*action)();
}
