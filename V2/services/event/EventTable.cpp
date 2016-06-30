

#include <services/event/EventTable.h>
#include <services/event/EventNotifyHandler.h>

#include <algorithm>


namespace Services {
namespace Event {

EventTable::EventTable()
{
    for (int i =0; i < 0xff; i++)
    {
        _handlerTable[i] = new EventNode(i);
    }
}

EventTable::~EventTable()
{
    for (int i =0; i < 0xff; i++)
    {
        delete _handlerTable[i];
    }
}

void EventTable::set(int id, EventNotifyHandler &handler)
{
    if (id < 0xff && id >=0)
        _handlerTable[id]->add(handler);
}

int EventTable::handleEvent(int id, const ACE_Message_Block &b)
{
    if (id < 0xff && id >=0)
        return _handlerTable[id]->handleEvent(id, b);

    return -1;
}


EventTable::EventNode::EventNode(int id)
{
    _handleVec.clear();
}

EventTable::EventNode::~EventNode()
{
    _handleVec.clear();
}


void EventTable::EventNode::add (EventNotifyHandler &handler)
{
    ACE_Guard<NodeMutex> guard(_nodeMutex);

    std::vector<EventNotifyHandler*>::iterator e;

    e = std::find(_handleVec.begin(), _handleVec.end(), &handler);

    if (e != _handleVec.end())
        return;
    
    _handleVec.push_back(&handler);
}

int EventTable::EventNode::handleEvent(int id, const ACE_Message_Block &b)
{
    int res = 0;
    std::vector<EventNotifyHandler*>::iterator e;
    
    for (e = _handleVec.begin(); e != _handleVec.end(); ++e)
    {
        res |=(*e)->handleEvent(id, b);
    }

    return res;
}

}
}

