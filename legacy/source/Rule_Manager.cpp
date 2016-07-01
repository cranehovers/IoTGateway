#include "Rule_Manager.h"
#include "Rule.h"

RuleManager::RuleManager()
{
    //ctor
}

RuleManager::~RuleManager()
{
    removeAllRules();
}

void RuleManager::setRule(Rule *rule)
{
    if (rule)
    {
        rule_list_.push_back(rule);
    }
}

void RuleManager::removeAllRules()
{
    std::list<Rule*>::iterator e;

    e = rule_list_.begin();

    for (; e != rule_list_.end(); ++e)
    {
        Rule *tmp = (*e);

        // delete the rule object
        // that have been saved into list
        delete tmp;
    }
}
