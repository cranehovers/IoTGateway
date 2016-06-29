

#ifndef NullService_H
#define NullService_H


#include <runtime/Service.h>

namespace GWSP {

class NullService : public Service
{
public:
    NullService(ServiceContext &context);
    ~NullService();


    virtual std::string &name();
    virtual bool initialize() {return true;};
    virtual bool unInitialize(){return true;};
    virtual bool start() {return true;};
    virtual bool stop(){return true;};
};

Service *nullServiceInstance(ServiceContext &context);

}

#endif //NullService_H

