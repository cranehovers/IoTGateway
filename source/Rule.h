#ifndef __RULE_H__
#define __RULE_H__


class WhenRule;
class TriggerRule;

class Rule
{
public:
    Rule();
    virtual ~Rule();

    void setCondition(WhenRule *whenCondition, TriggerRule *trigger);

private:

    WhenRule *when_;
    TriggerRule *trigger_;
};

#endif // __RULE_H__
