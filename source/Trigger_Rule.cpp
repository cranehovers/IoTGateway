#include "Trigger_Rule.h"

TriggerRule::TriggerRule()
{
    //ctor
}

TriggerRule::~TriggerRule()
{
    //dtor
}


void TriggerRule::setDeviceId(std::string deviceid)
{
    deviceid_ = deviceid;
}

void TriggerRule::setAction(std::string action)
{
    action_ = action;
}


