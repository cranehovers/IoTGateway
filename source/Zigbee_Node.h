/*
* This file defines zigbee node
*/

#ifndef __ZIGBEE_NODE_H__
#define __ZIGBEE_NODE_H__

#include "Config.h"
#include <map>


class ZigbeeNodeKey
{
public:

    unsigned char short_addr_[2];
    unsigned char ieee_addr_[8];

    bool compare_by_short_addr(unsigned char short_addr[2]);
    bool compare_by_ieee_addr(unsigned char ieee_addr[8]);
};

class NodeSimpleDesc
{
public:

    NodeSimpleDesc()
    {
     profileid_[0] = 0;
     profileid_[1] = 0;
     deviceid_[0] = 0;
     deviceid_[1] = 0;
     device_ver_ = 0;
     num_of_in_cls_ = 0;
     num_of_out_cls_ = 0;
     in_cls_ = 0;
     out_cls_ = 0;
    };

    ~NodeSimpleDesc()
    {
        if (in_cls_)
        {
            delete in_cls_;
        }

        if (out_cls_)
        {
            delete out_cls_;
        }
    };
    
    unsigned char profileid_[2];
    unsigned char deviceid_[2];
    unsigned char device_ver_;
    unsigned char num_of_in_cls_;
    unsigned char num_of_out_cls_;
    unsigned char *in_cls_;
    unsigned char *out_cls_;
};

class ZigbeeNode
{
public:

     typedef enum NODE_TYPE
     {
        Coordination,
        Router,
        Device
     }NODE_TYPE;

     ZigbeeNode();
     virtual ~ZigbeeNode();

     void set_short_addr(unsigned char short_addr[2]);
     void set_ieee_addr(unsigned char ieee_addr[8]);

     unsigned char *get_short_addr();
     unsigned char *get_ieee_addr();

     void set_node_type(NODE_TYPE type);

     virtual void get_self_basic_info(){};
     virtual void get_self_ep_count(){};
     virtual void get_self_ep_desc(){};
     
     virtual void set();
     virtual void set_child(unsigned char* child_list, unsigned char count);
     virtual void set_ep(unsigned char* ep_list, unsigned char count);
     virtual void set_ep_simple_desc(unsigned char ep, NodeSimpleDesc *desc);
     virtual void create_device(){};
protected:

        unsigned char *get_ep_list();
        unsigned char get_ep_count();        
        unsigned char *get_child_list();
        unsigned char get_child_count();

private:

    ZigbeeNodeKey *key_;
    NODE_TYPE node_type_;
    unsigned char *child_list_;
    unsigned char child_count_;
    unsigned char *ep_list_;
    unsigned char ep_count_;
    std::map<unsigned char, NodeSimpleDesc*> ep_simple_desc_;
    
};

#endif //__ZIGBEE_NODE_H__
