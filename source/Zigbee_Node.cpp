/*
* This file defines zigbee node
*/

#include "Zigbee_Node.h"
#include "Zigbee_Node_Cache.h"

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

void ZigbeeNode::set()
{
    gZigbeeNodeCache::instance()->add(key_, this);
}

void ZigbeeNode::set_child(unsigned char* child_list, unsigned char count)
{
    if (child_list_ == 0 )
    {
        child_list_ = new unsigned char[count];
        child_count_ = count;
        
        ACE_OS::memcpy(child_list_, child_list, count);

        create_device();
    }
    //FIXME:todo exception
    else
    {
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

void ZigbeeNode::set_ep_simple_desc(unsigned char ep, NodeSimpleDesc *desc)
{
    ep_simple_desc_.insert(std::pair<unsigned char,NodeSimpleDesc* >(ep, desc));
}


