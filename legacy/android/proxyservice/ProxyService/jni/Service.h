#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <string>

#include <ace/Log_Msg.h>
#include <ace/Singleton.h>
#include <ace/Mutex.h>
#include <ace/Null_Mutex.h>
#include <ace/Task.h>
#include <ace/Thread_Manager.h>
#include <ace/Synch.h>

#include "ServiceLog.h"


class Service : public ACE_Task_Base
{
public:
    Service();
    virtual ~Service();

    int load(std::string serviceName);
    int close();
    int start();
    int stop();
    int init();
    void setPrintOutput(print_log printlog);


    virtual int svc (void);

protected:
private:

    std::string service_name_;
    void *service_handle_;
    void *service_start_;
    void *service_stop_;
    void *service_init_;
    void *service_print_;
};

#endif // __SERVICE_H__
