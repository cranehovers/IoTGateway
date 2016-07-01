/*
**  This file defined CoAP Rules Resource for Rules Manager
*/


#ifndef __COAP_RULESRE_SOURCE_H__
#define __COAP_RULESRE_SOURCE_H__


#include "CoAP_Resource.h"

class CoAPWrapper;
class CoAPCallback;

class CoAPRulesResource : public CoAPResource
{
public:
    CoAPRulesResource(CoAPWrapper* ctx);
    virtual ~CoAPRulesResource();

    virtual void* Create();
    virtual void handler_post(CoAPCallback &callback);

private:
};

#endif // __COAP_RULESRE_SOURCE_H__
