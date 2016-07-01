
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
    // get the rule string from payload.
    std::string payload;
    bool check_flag = false;

    get_wrapper()->get_payload(callback, payload);

    if (payload.empty())
    {
        ACE_DEBUG((LM_DEBUG, "the payload of rules is empty\n"));
    }
    else // json formate,
    {
        cJSON *result = cJSON_Parse(payload.c_str());

        if (result == 0 )
        {
            ACE_DEBUG((LM_DEBUG, "failed to parse json string(%s)\n",payload.c_str()));
        }
        else //get when object
        {
            cJSON *whenObject = cJSON_GetObjectItem(result, "when");

            if (whenObject != 0) // get command id
            {
                cJSON *deviceid = cJSON_GetObjectItem(result, "deviceid");

                if (deviceid != 0)
                {
                    if (whenObject->type == cJSON_Number &&
                        deviceid->type == cJSON_Number)
                    {

                    }
                }
             }

             //TODO: triggger object

             cJSON_Delete(result);
        }
    }

    if (!check_flag)
    {
        get_wrapper()->bad_request(callback);
    }
    else
    {
        get_wrapper()->ok_request(callback);
    }


}
