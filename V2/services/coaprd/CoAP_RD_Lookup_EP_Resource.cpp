/*
* This file implement lookup EP resource
*/

#include <toolkit/CoAP_Wrapper.h>
#include <toolkit/CoAP_Resource.h>
#include <services/coaprd/CoAP_RD_Lookup_EP_Resource.h>
#include <coap.h>


using toolkit::CoAPWrapper;
using toolkit::CoAP_Attr;

namespace Services {
namespace CoAPRD {



CoAPRDLookUpEPResource::CoAPRDLookUpEPResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
}

CoAPRDLookUpEPResource::~CoAPRDLookUpEPResource()
{

}

void* CoAPRDLookUpEPResource::Create()
{
    CoAP_Attr attr;
    std::string uri("rd-lookup/ep");    
    
    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd-lookup.ep\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    return rd_resource;
}

int send_data_by_block(coap_pdu_t *response, 
                       coap_block_t *block_opt, 
                       unsigned char *data, 
                       unsigned int len)
{
    
    if (1 == coap_write_block_opt(block_opt, COAP_OPTION_BLOCK2, response, len))
    {
        if (1== coap_add_block(response, 
                                len, 
                                data,
                                block_opt->num,
                                block_opt->szx))
        {
                return 1;               
        }
        else
        {
            ACE_DEBUG((LM_DEBUG, "Failed to send data by block-wise\n"));
        }
    }
    else
    {
        ACE_DEBUG((LM_DEBUG, "Failed to set block opt into repsonse\n"));
    }

    return 0;
    
}

void CoAPRDLookUpEPResource::handler_get(CoAPCallback &callback)
{
    std::string ep_result;
    
    find_ep_result(ep_result);

    if (ep_result.empty())
    {
    
        coap_pdu_t *response = (coap_pdu_t*)callback.response_;
        
        /* create response */
        response->hdr->code = COAP_RESPONSE_404;
    }
    else
    {

        unsigned char buf[3];
        std::string result;
        coap_block_t block_opt;
        
        coap_pdu_t *response = (coap_pdu_t*)callback.response_;
        
        /* create response */
        response->hdr->code = COAP_RESPONSE_CODE(205);
        
        coap_add_option(response, COAP_OPTION_CONTENT_TYPE,
                        coap_encode_var_bytes(buf, COAP_MEDIATYPE_APPLICATION_LINK_FORMAT), buf);
        
        coap_add_option(response, COAP_OPTION_MAXAGE,
                        coap_encode_var_bytes(buf, 0x2ffff), buf);
                        
        /* 1.if result more than 1024 byte, block wise transfer,
         * otherwise directly transfer
         * 2. if client set block2 option, response must be block-wise transferred
         */
        if (1 == coap_get_block((coap_pdu_t*)callback.request_, COAP_OPTION_BLOCK2, &block_opt))
        {
            ACE_DEBUG((LM_DEBUG, "send data by block from %d(size=%d)\n",block_opt.num,block_opt.szx));

            if (1 == send_data_by_block(response, &block_opt, (unsigned char*)ep_result.c_str(), ep_result.length()))
            {
                return;
            }
        }

        if (ep_result.length() > 1024 )
        {
            block_opt.szx = 6;
            block_opt.m = 1;
            block_opt.num = 0;

            ACE_DEBUG((LM_DEBUG, "send data by block from %d(size=%d)\n",block_opt.num,block_opt.szx));

            if (1 == send_data_by_block(response, &block_opt, (unsigned char*)ep_result.c_str(), ep_result.length()))
            {
                return;
            }            
        }

        /* at here, we send data directly*/
        {
        
            ACE_DEBUG((LM_DEBUG, "send data directly(len=%d)\n", ep_result.length()));
            
            coap_add_data(response, ep_result.length(), (unsigned char*)ep_result.c_str());
        }
    }
    
}




}
}
