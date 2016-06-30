#ifndef CONSOLE_SERVICE_H
#define CONSOLE_SERVICE_H

#include <runtime/Service.h>



namespace Services {
namespace Console {

class ConsoleService : public GWSP::Service, public ACE_Task_Base
{

public:
    typedef toolkit::AutoPtr<ConsoleService> Ptr;
    
    ConsoleService(GWSP::ServiceContext &context);
    ~ConsoleService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();
    virtual int svc (void);


private:
    bool _stopped;
    

};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}

#endif //CONSOLE_SERVICE_H