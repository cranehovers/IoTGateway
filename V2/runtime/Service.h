
#ifndef Service_H
#define Service_H


#include <toolkit/ReferenceCountObject.h>
#include <string>

namespace GWSP {

class Service : public toolkit::RefCountedObject
{

public:
    Service();
    virtual ~Service();

    virtual std::string &name() = 0;
    virtual bool initialize() = 0;
    virtual bool unInitialize() = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;
};

}



#endif //Service_H



