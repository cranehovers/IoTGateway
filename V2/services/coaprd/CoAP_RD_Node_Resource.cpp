/*
* This file implement Node Resource
*/

#include <toolkit/CoAP_Wrapper.h>
#include <services/coaprd/CoAP_RD_Node_Resource.h>


using toolkit::CoAPWrapper;


namespace Services {
namespace CoAPRD {


CoAPRDNodeResource::CoAPRDNodeResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
}

CoAPRDNodeResource::~CoAPRDNodeResource()
{
}

}
}









