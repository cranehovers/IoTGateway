#include "Rule.h"
#include "When_Rule.h"
#include "Trigger_Rule.h"


Rule::Rule()
{
    when_ = 0;
    trigger_ = 0;
}

Rule::~Rule()
{
    if (when_)
    {
        delete when_;
    }

    if ( trigger_)
    {
        delete trigger_;
    }
}

void Rule::setCondition(WhenRule *whenCondition, TriggerRule *trigger)
{
    if ( whenCondition && trigger )
    {
        when_ = whenCondition;
        trigger_ = trigger;
    }
}
