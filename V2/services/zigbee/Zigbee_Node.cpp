/*
* This file defines zigbee node
*/

#include <services/zigbee/ZigbeeHelper.h>
#include <services/zigbee/Zigbee_Node.h>


namespace Services {
namespace Zigbee {



bool ZigbeeNodeKey::compare_by_short_addr(unsigned char short_addr[2])
{
    return (ACE_OS::memcmp(short_addr_, short_addr, 2)== 0 ? true: false);
}

bool ZigbeeNodeKey::compare_by_ieee_addr(unsigned char ieee_addr[8])
{
    return (ACE_OS::memcmp(ieee_addr_, ieee_addr, 8)== 0 ? true: false);
}


ZigbeeNode::ZigbeeNode()
:key_(new ZigbeeNodeKey())
{
    child_list_ = 0;
    child_count_ = 0;
    ep_list_ = 0;
    ep_count_ = 0;
    bind_ep_ = 0;
}

ZigbeeNode::~ZigbeeNode()
{
    delete key_;

    if (child_list_)
        delete child_list_;

    if (ep_list_)
        delete ep_list_;
}

void ZigbeeNode::set_short_addr(unsigned char short_addr[2])
{
    ACE_OS::memcpy(key_->short_addr_, short_addr, 2);
}

void ZigbeeNode::set_ieee_addr(unsigned char ieee_addr[8])
{
    ACE_OS::memcpy(key_->ieee_addr_, ieee_addr, 8);
}

unsigned char *ZigbeeNode::get_short_addr()
{
    return key_->short_addr_;
}

unsigned char *ZigbeeNode::get_ieee_addr()
{
    return key_->ieee_addr_;
}

void ZigbeeNode::set_node_type(NODE_TYPE type)
{
    node_type_ = type;
}

ZigbeeNode::NODE_TYPE ZigbeeNode::get_node_type()
{
    return node_type_;
}

bool ZigbeeNode::set()
{
    ZigbeeNode *n = _zigbeeHelper::instance()->cache()->find_node_by_shortaddr(get_short_addr());

    if (n == 0)
    {
        _zigbeeHelper::instance()->cache()->add(key_, this);

        return true;
    }

    return false;
}

void ZigbeeNode::set_child(unsigned char* child_list, unsigned char count)
{
    if (child_list_ == 0 )
    {
        child_list_ = new unsigned char[count];
        child_count_ = count;
        
        ACE_OS::memcpy(child_list_, child_list, count);

        //create_device();
    }
    //FIXME:todo exception
    else
    {
        if ( child_count_ != count || 
             ACE_OS::memcmp(child_list_, child_list, count) != 0 )
        {
            ACE_DEBUG((LM_DEBUG, "******* update the child_count and child_list *******\n\n"));
            
            child_count_ = count;

            if ( child_list_ != 0 )
            {
                delete [] child_list_;
                
                child_list_ = new unsigned char[count]; 
            }

            if ( child_list_ != 0 )
            {
                ACE_OS::memcpy(child_list_, child_list, count); 
            }
        }
    } 
}

void ZigbeeNode::set_ep(unsigned char* ep_list, unsigned char count)
{
    if (ep_list_ == 0 )
    {
        ep_list_ = new unsigned char[count];
        ep_count_ = count;
        
        ACE_OS::memcpy(ep_list_, ep_list, count);
    }
    //FIXME:todo exception
    else
    {
        if ( ep_count_ != count ||
             ACE_OS::memcmp(ep_list_, ep_list, count) != 0 )
        {
            ACE_DEBUG((LM_DEBUG, "****** update the ep_count and ep_list *****\n\n"));
            
            ep_count_ = count;

            if ( ep_list_ != 0 )
            {
                delete [] ep_list_;
                
                ep_list_ = new unsigned char[count]; 
            }

            if ( ep_list_ != 0 )
            {
                ACE_OS::memcpy(ep_list_, ep_list, count); 
            }            
        }
    }
}

unsigned char *ZigbeeNode::get_ep_list()
{
    return ep_list_;
}

unsigned char ZigbeeNode::get_ep_count()
{
    return ep_count_;
}

unsigned char *ZigbeeNode::get_child_list()
{
    return child_list_;
}

unsigned char ZigbeeNode::get_child_count()
{
    return child_count_;
}

void ZigbeeNode::set_bind_ep(unsigned char ep)
{
    bind_ep_ = ep;
}

unsigned char ZigbeeNode::get_bind_ep()
{
    return bind_ep_;
}

void ZigbeeNode::set_ep_simple_desc(unsigned char ep, NodeSimpleDesc *desc)
{
    if (ep_simple_desc_.find(ep) == ep_simple_desc_.end())
    {
        ep_simple_desc_.insert(std::pair<unsigned char,NodeSimpleDesc* >(ep, desc));

        create_coap_resource(ep, desc);
    }
    else
    {
        // update case
        // now, only delete the description
        delete desc;
    }

    if(get_node_type() == Coordination)
    {
        set_bind_ep(ep);
        create_device();
    }
}

void ZigbeeNode::create_coap_resource(unsigned char ep, NodeSimpleDesc *desc)
{
    //ZigbeeCoAPService::instance()->create_resource_by_zigbeenode(ep, desc, this);
    ACE_DEBUG((LM_DEBUG, "create ep :%x\n", ep));
    
}

}
}

