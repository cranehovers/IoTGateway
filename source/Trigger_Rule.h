#ifndef __TRIGGER_RULE_H__
#define __TRIGGER_RULE_H__

#include "Config.h"

class TriggerRule
{
public:
    TriggerRule();
    virtual ~TriggerRule();

    void setDeviceId(std::string deviceid);
    void setAction(std::string action);

protected:
private:

    std::string deviceid_;
    std::string action_;

};

#endif // __TRIGGER_RULE_H__
