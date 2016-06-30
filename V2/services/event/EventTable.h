
#ifndef EVENT_TABLE_H
#define EVENT_TABLE_H

#include <toolkit/ReferenceCountObject.h>
#include <vector>

namespace Services {
namespace Event {

class EventNotifyHandler;

class EventTable : public toolkit::RefCountedObject
{
public:
    EventTable();
    ~EventTable();

    void set(int id, EventNotifyHandler &handler);
    int handleEvent(int id, const ACE_Message_Block &b);

    class EventNode
    {
    public:
        EventNode(int id);
        ~EventNode();

        void add (EventNotifyHandler &handler);
        int handleEvent(int id, const ACE_Message_Block &b);

    private:
        typedef ACE_Recursive_Thread_Mutex NodeMutex; 
        
        int _id;
        std::vector<EventNotifyHandler*> _handleVec;
        NodeMutex _nodeMutex;
    };
    
private:

    EventNode* _handlerTable[0xff];
    
};

}
}



#endif //EVENT_TABLE_H

