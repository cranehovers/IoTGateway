#ifndef __WHEN_RULE_H__
#define __WHEN_RULE_H__

#include "Config.h"

class WhenRule
{
public:
    WhenRule();
    virtual ~WhenRule();

    void setDeviceId(std::string deviceid);
    void setAction(std::string action);


protected:
private:

    std::string deviceid_;
    std::string action_;
};

#endif // __WHEN_RULE_H__
