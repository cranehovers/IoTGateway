
#ifndef ReactorService_H
#define ReactorService_H

#include <runtime/Service.h>



namespace Services {
namespace Reactor {

class ReactorService : public GWSP::Service, public ACE_Task_Base
{

public:
    typedef toolkit::AutoPtr<ReactorService> Ptr;

    ReactorService(GWSP::ServiceContext &context);
    ~ReactorService();
    
    virtual std::string &name();
    virtual bool initialize();
    virtual bool unInitialize();
    virtual bool start();
    virtual bool stop();

    virtual int svc (void);
    
    int register_handler (ACE_Event_Handler *eh,
                          ACE_Reactor_Mask mask);
    long schedule_timer(ACE_Event_Handler *eh,
                        const void *arg,
                        const ACE_Time_Value &delay);
    void remove_handler(ACE_Event_Handler *eh);
    void cancel_timer(ACE_Event_Handler *eh);

private:
    ACE_Reactor _reactor;
    bool _stopped;

};

GWSP::Service *instance(GWSP::ServiceContext &context);

}
}


#endif //ReactorService_H









