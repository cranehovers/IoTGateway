/*
* This file implement lookup D resource
*/

#include "Config.h"
#include "CoAP_RD_Lookup_D_Resource.h"
#include "CoAP_Wrapper.h"
#include "coap.h"

CoAPRDLookUpDomainResource::CoAPRDLookUpDomainResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
}

CoAPRDLookUpDomainResource::~CoAPRDLookUpDomainResource()
{

}

void* CoAPRDLookUpDomainResource::Create()
{
    CoAP_Attr attr;
    std::string uri("rd-lookup/d");    
    
    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd-lookup.d\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    return rd_resource;
}

void CoAPRDLookUpDomainResource::handler_get(CoAPCallback &callback)
{
    std::list<std::string> domain_list;

    find_domain_list(domain_list);

    if (domain_list.empty())
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

        std::list<std::string>::iterator e = domain_list.begin();

        for (; e != domain_list.end(); ++e)
        {
            result += "</rd>;d=\"" + *e + "\",";
        }

        coap_add_data(response, result.length(), (unsigned char*)result.c_str());
            
    }
    
}


