/*
* This file implement lookup EP resource
*/

#include "Config.h"
#include "CoAP_RD_Lookup_EP_Resource.h"
#include "CoAP_Wrapper.h"
#include "coap.h"

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

void CoAPRDLookUpEPResource::handler_get(CoAPCallback &callback)
{
    std::string ep_result;
    
    find_ep_result(ep_result);

    ACE_DEBUG((LM_DEBUG, "\nep = %s\n", ep_result.c_str()));

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
        
        coap_pdu_t *response = (coap_pdu_t*)callback.response_;
        
        /* create response */
        response->hdr->code = COAP_RESPONSE_CODE(205);
        
        coap_add_option(response, COAP_OPTION_CONTENT_TYPE,
                        coap_encode_var_bytes(buf, COAP_MEDIATYPE_APPLICATION_LINK_FORMAT), buf);
        
        coap_add_option(response, COAP_OPTION_MAXAGE,
                        coap_encode_var_bytes(buf, 0x2ffff), buf);

        
        coap_add_data(response, ep_result.length(), (unsigned char*)ep_result.c_str());
    }
    
}
