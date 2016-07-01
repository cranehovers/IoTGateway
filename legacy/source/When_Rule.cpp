#include "When_Rule.h"

WhenRule::WhenRule()
{
    //ctor
}

WhenRule::~WhenRule()
{
    //dtor
}

void WhenRule::setDeviceId(std::string deviceid)
{
    deviceid_ = deviceid;
}

void WhenRule::setAction(std::string action)
{
    action_ = action;
}

