/*
* This file define route
*/

#include "Zigbee_Frame_Router.h"
#include "Zigbee_Request.h"
#include "Zigbee_Node.h"
#include "Zigbee_Node_Cache.h"

#define IEEE_ADDR_RSP_CMD0 0x45
#define IEEE_ADDR_RSP_CMD1 0x81

#define ACTIVE_EP_RSP_CMD0 0x45
#define ACTIVE_EP_RSP_CMD1 0x85

#define SIMPLE_EP_DESC_RSP_CMD0 0x45
#define SIMPLE_EP_DESC_RSP_CMD1 0x84

/// internal message type 
#define ROUTER_RESPONSE 0x10
#define ROUTER_EXIT_THREAD 0x20


typedef struct RouteObject
{
    ZigbeeResponse *o;
}RouteObject;

ZigbeeFrameRouter::ZigbeeFrameRouter()
{
    thread_exit_flag = 0;
    activate();
}

ZigbeeFrameRouter::~ZigbeeFrameRouter()
{
    clear();
}

int ZigbeeFrameRouter::svc (void)
{
    while( !thread_exit_flag)
    {
        ACE_Message_Block *b = 0;
        
        if (message_queue.dequeue_head(b) == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "faile get block from queue\n"));
            continue;
        }

        if (b->msg_type() ==ROUTER_EXIT_THREAD)
        {
            break;
        }
        else if (b->msg_type() == ROUTER_RESPONSE)
        {
            RouteObject *ro = (RouteObject*)b->base();
            handle_response(ro->o);

            delete ro->o;
        }

        b->release();
    }
    
    return 0;    
}

void ZigbeeFrameRouter::clear()
{
    thread_exit_flag = 1;
    {        
        ACE_Message_Block *b = new ACE_Message_Block();
        b->msg_type(ROUTER_EXIT_THREAD);
        
        message_queue.enqueue_head(b);
        wait();
    }
    
    while (!message_queue.is_empty())
    {
        ACE_Message_Block *b = 0;
        message_queue.dequeue(b);

        RouteObject *ro = (RouteObject*)b->base();
        delete ro->o;

        b->release();
    }

    message_queue.close();
}

int ZigbeeFrameRouter::route(ZigbeeResponse *resp)
{
    ACE_Message_Block *b =  new ACE_Message_Block(sizeof(RouteObject));
    b->msg_type(ROUTER_RESPONSE);

    RouteObject *ro = (RouteObject*)b->base();
    ro->o = resp;

    if (message_queue.enqueue_tail(b) == -1 )
    {            
        ACE_DEBUG((LM_DEBUG, "faild to input node block into queue\n"));

        b->release();
        delete resp;
    }

    return 0;
}

int ZigbeeFrameRouter::handle_response(ZigbeeResponse *resp)
{
    dump(resp->base(), resp->size());
    
    // IEEE_ADDR_RSP
    if (resp->cmd0() == IEEE_ADDR_RSP_CMD0 &&
        resp->cmd1() == IEEE_ADDR_RSP_CMD1)
    {
       unsigned char short_addr[2];
       unsigned char ieee_addr[8];
       unsigned char status;
       
       status = resp->base()[4];

       //dump(&status, 1);

       if (status == 0)
       {
           ACE_OS::memcpy(ieee_addr, &resp->base()[5],8);
           ACE_OS::memcpy(short_addr, &resp->base()[13],2);

           //dump(ieee_addr, 8);           
           //dump(short_addr, 2);
           
           ZigbeeNode *node =
           gZigbeeNodeCache::instance()->find_node_by_shortaddr(short_addr);

           if (node)
           {
               unsigned char start_index;
               unsigned char child_count;
               unsigned char *child_list;

               node->set_ieee_addr(ieee_addr); 
                              
               start_index = resp->base()[15];
               child_count = resp->base()[16];

               //dump(&start_index, 1);
               //dump(&child_count, 1);

               if (child_count > 0)
               {
                   child_list = &resp->base()[17];
                   node->set_child(child_list, child_count*2);

                   //dump(child_list, child_count *2);
               }

               node->get_self_ep_count();
           }
       }
       
    }
    else if (resp->cmd0() == ACTIVE_EP_RSP_CMD0 &&
        resp->cmd1() == ACTIVE_EP_RSP_CMD1)
    {
        unsigned char status;
        
        status = resp->base()[6];

        //dump(&status, 1);

        if (status == 0)
        {
            unsigned char short_addr[2];
            unsigned char ep_count;
            unsigned char *ep_list;

            ACE_OS::memcpy(short_addr, &resp->base()[7],2);

            //dump(short_addr, 2);

            ep_count = resp->base()[9];

            //dump(&ep_count, 1);

            if (ep_count > 0)
            {
                ep_list = &resp->base()[10];
                //dump(ep_list, ep_count);

                ZigbeeNode *node =
                gZigbeeNodeCache::instance()->find_node_by_shortaddr(short_addr);

                if(node)
                {
                    node->set_ep(ep_list, ep_count);
                    node->get_self_ep_desc();
                }
            }
            
        }        
    }
    else if (resp->cmd0() == SIMPLE_EP_DESC_RSP_CMD0 &&
        resp->cmd1() == SIMPLE_EP_DESC_RSP_CMD1)
    {
        unsigned char status;
        
        status = resp->base()[6];

        //dump(&status, 1);

        if (status == 0)
        {
            unsigned char short_addr[2];
            unsigned char ep;
            NodeSimpleDesc *desc = new NodeSimpleDesc();
           
            ACE_OS::memcpy(short_addr, &resp->base()[7],2);

            //dump(short_addr, 2);

            ep = resp->base()[10];

            //dump(&ep, 1);
            
            desc->profileid_[0] = resp->base()[11];
            desc->profileid_[1] = resp->base()[12];

            //dump(desc->profileid_, 2);
            
            desc->deviceid_[0] = resp->base()[13];
            desc->deviceid_[1] = resp->base()[14];

            //dump(desc->deviceid_, 2);
            
            desc->device_ver_ = resp->base()[15];

            //dump(&desc->device_ver_, 1);
            

            desc->num_of_in_cls_= resp->base()[16];
            desc->num_of_out_cls_= resp->base()[16 + desc->num_of_in_cls_*2+1];

            //dump(&desc->num_of_in_cls_, 1);
            //dump(&desc->num_of_out_cls_, 1);
            

            if (desc->num_of_in_cls_ > 0)
            {
                desc->in_cls_ = new unsigned char[desc->num_of_in_cls_ * 2];
                
                ACE_OS::memcpy(desc->in_cls_, &resp->base()[17], desc->num_of_in_cls_ *2);

                
                //dump(desc->in_cls_, desc->num_of_in_cls_*2);
            }

            if (desc->num_of_out_cls_ > 0)
            {
                desc->out_cls_ = new unsigned char[desc->num_of_out_cls_*2];

                ACE_OS::memcpy(desc->out_cls_, &resp->base()[16 + desc->num_of_in_cls_ *2+2], desc->num_of_out_cls_*2);

                //dump(desc->out_cls_, desc->num_of_out_cls_*2);
            }
            
            ZigbeeNode *node =
            gZigbeeNodeCache::instance()->find_node_by_shortaddr(short_addr);

            if(node)
            {
                node->set_ep_simple_desc(ep, desc);
            }
            
        }        
    }
    
    return 0;
}


void ZigbeeFrameRouter::dump(unsigned char *buf, unsigned char len)
{
    for (int i=0; i < len; i++)
        ACE_DEBUG((LM_DEBUG,"%0x ",buf[i]));
        
    ACE_DEBUG((LM_DEBUG, "\n"));
}


