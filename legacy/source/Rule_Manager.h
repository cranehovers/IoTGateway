#ifndef __RULE_MANAGER_H__
#define __RULE_MANAGER_H__

#include "Config.h"
#include <list>

class Rule;

class RuleManager
{
public:
    RuleManager();
    virtual ~RuleManager();

    void setRule(Rule *rule);


private:

    void removeAllRules();

    std::list<Rule*> rule_list_;

};


// Rule manager singleton object
typedef ACE_Singleton<RuleManager, ACE_Mutex> Rule_Manager;

#endif // __RULE_MANAGER_H__
