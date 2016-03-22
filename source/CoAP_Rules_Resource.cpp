
#include "CoAP_Rules_Resource.h"
#include "CoAP_Wrapper.h"
#include "CoAP_Resource.h"
#include "coap.h"


CoAPRulesResource::CoAPRulesResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
    //ctor
}

CoAPRulesResource::~CoAPRulesResource()
{
    //dtor
}

void* CoAPRulesResource::Create()
{
    CoAP_Attr attr;
    std::string uri("rules");

    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    void *rules_res = Create_i();
    SetCoAPResource(rules_res);

    return rules_res;
}

void CoAPRulesResource::handler_post(CoAPCallback &callback)
{
    ACE_DEBUG((LM_DEBUG, "at copa rules resource post method\n"));
}
