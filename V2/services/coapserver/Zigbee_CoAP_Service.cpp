/*
* This file defines CoAP for zigbee
*/


#include <runtime/ServiceGetter.h>
#include <services/conf/ConfService.h>
#include <services/reactor/ReactorService.h>
#include <toolkit/CoAP_Wrapper.h>
#include <toolkit/CoAP_Resource.h>
#include <services/zigbee/Zigbee_Node.h>
#include <services/coapserver/Zigbee_CoAP_Resource.h>
#include <services/coapserver/Zigbee_CoAP_Service.h>


#include <coap.h>


using namespace GWSP;


namespace Services {
namespace CoAPServer {

GWSP::Service *instance(GWSP::ServiceContext &context){return new ZigbeeServer(context);}

static std::string serviceName("service.coapserver");


ZigbeeServer::ZigbeeServer(GWSP::ServiceContext &context)
:Service(context)
{
}

ZigbeeServer::~ZigbeeServer()
{
}

std::string &ZigbeeServer::name()
{
    return serviceName;
}

bool ZigbeeServer::initialize()
{
    try
    {
        std::string conf("service.conf");
        std::string reactor("service.reactor");
        
        _confPtr = ServiceGetter::findByName<ConfService>(context(), conf);
        _reactorPtr = ServiceGetter::findByName<ReactorService>(context(), reactor);

    }                                                           
    catch(toolkit::NullPointerException e)
    {
        ACE_DEBUG((LM_DEBUG, "get service failed at SerialPortService\n"));

        return false;
    }


    _coapWrapperPtr = new CoAPWrapper();
    
    if (_coapWrapperPtr->Create(_confPtr->_svcAddr, 
                              _confPtr->_svcAddrPort,
                              _confPtr->_coapDebugLevel
                              ) < 0)
    {

        ACE_DEBUG((LM_DEBUG,"Failed to create coap\n"));
        return false;
    }
            
    return true;
}

bool ZigbeeServer::unInitialize()
{
    return true;
}

bool ZigbeeServer::start()
{
    ACE_Time_Value timeout;
    timeout.sec(5);

    _reactorPtr->register_handler(this, ACE_Event_Handler::READ_MASK);
    _reactorPtr->schedule_timer(this, 0, timeout);
    

    return true;
}

bool ZigbeeServer::stop()
{
    _reactorPtr->remove_handler(this);
    _reactorPtr->cancel_timer(this);
    
    return true;
}

int ZigbeeServer::handle_input (ACE_HANDLE fd)
{   
    _coapWrapperPtr->handle_event();
    return 0;
}

int ZigbeeServer::handle_timeout (const ACE_Time_Value &tv,
                                            const void *arg)
{
    ACE_Time_Value timeout;
    timeout.sec(5);
    
    _coapWrapperPtr->time_out(timeout);
    _reactorPtr->schedule_timer(this, 0, timeout);
    
    return 0;

}


ACE_HANDLE ZigbeeServer::get_handle () const
{
    return _coapWrapperPtr->get_handle();
}

void ZigbeeServer::reset()
{
    _reactorPtr->remove_handler(this);
    _reactorPtr->cancel_timer(this);

    _coapWrapperPtr->clear();
    
    std::list<CoAPResource*>::iterator e =
    zigbee_coap_resource_list_.begin();

    for (; e != zigbee_coap_resource_list_.end(); ++e)
    {
        ZigbeeCoapResource *r = dynamic_cast<ZigbeeCoapResource*>(*e);
        delete (r);
    }

    zigbee_coap_resource_list_.clear();

   
   _coapWrapperPtr = new CoAPWrapper();
   
   if (_coapWrapperPtr->Create(_confPtr->_svcAddr, 
                             _confPtr->_svcAddrPort,
                             _confPtr->_coapDebugLevel
                             ) < 0)
   {
   
       ACE_DEBUG((LM_DEBUG,"Failed to reset coap\n"));
       return;
   }

   start();
   
}



void ZigbeeServer::create_resource_by_zigbeenode(unsigned char ep,
                                   NodeSimpleDesc *desc,
                                   ZigbeeNode *node)
{
    ZigbeeCoapResource *r;

    if ((r = new ZigbeeCoapResource(_coapWrapperPtr.get())) == 0)
    {
        ACE_DEBUG((LM_DEBUG, "Failed to zigbee coap resource()\n"));
        return;
    }

    r->set_zigbee_desc(desc);
    r->set_zigbee_ep(ep);
    r->set_zigbee_node(node);

    zigbee_coap_resource_list_.push_back(r);

    if ((r->Create()) == 0)
    {
        ACE_DEBUG((LM_DEBUG,"Failed to craete rd resource\n"));
        return;
    }

    register_resource_to_rd(r);
}


static int
order_opts(void *a, void *b) {
  if (!a || !b)
    return a < b ? -1 : 1;

  if (COAP_OPTION_KEY(*(coap_option *)a) < COAP_OPTION_KEY(*(coap_option *)b))
    return -1;

  return COAP_OPTION_KEY(*(coap_option *)a) == COAP_OPTION_KEY(*(coap_option *)b);
}

static coap_list_t *
new_option_node(unsigned short key, unsigned int length, unsigned char *data) {
  coap_option *option;
  coap_list_t *node;

  option = (coap_option*)coap_malloc(sizeof(coap_option) + length);
  if ( !option )
    goto error;

  COAP_OPTION_KEY(*option) = key;
  COAP_OPTION_LENGTH(*option) = length;
  memcpy(COAP_OPTION_DATA(*option), data, length);

  /* we can pass NULL here as delete function since option is released automatically  */
  node = coap_new_listnode(option, NULL);

  if ( node )
    return node;

 error:
  perror("new_option_node: malloc");
  coap_free( option );
  return NULL;
}


static void
handle_full_uri(std::string &full_url, coap_uri_t &uri, coap_list_t *&optlist)
{
    unsigned char portbuf[2];
#define BUFSIZE 40
    unsigned char _buf[BUFSIZE];
    unsigned char *buf = _buf;
    size_t buflen;
    int res;

    /* split arg into Uri-* options */
    coap_split_uri((unsigned char *)full_url.c_str(), full_url.length(), &uri );

    if (uri.port != COAP_DEFAULT_PORT)
    {
        coap_insert( &optlist,
           new_option_node(COAP_OPTION_URI_PORT,
        		   coap_encode_var_bytes(portbuf, uri.port),
        		 portbuf),
           order_opts);
    }

    if (uri.path.length)
    {
        buflen = BUFSIZE;
        res = coap_split_path(uri.path.s, uri.path.length, buf, &buflen);

        while (res--)
        {
            coap_insert(&optlist, new_option_node(COAP_OPTION_URI_PATH,
            			      COAP_OPT_LENGTH(buf),
            			      COAP_OPT_VALUE(buf)),
                order_opts);

            buf += COAP_OPT_SIZE(buf);
        }
    }

    if (uri.query.length)
    {
        buflen = BUFSIZE;
        buf = _buf;
        res = coap_split_query(uri.query.s, uri.query.length, buf, &buflen);

        while (res--)
        {
        coap_insert(&optlist, new_option_node(COAP_OPTION_URI_QUERY,
        			      COAP_OPT_LENGTH(buf),
        			      COAP_OPT_VALUE(buf)),
            order_opts);

        buf += COAP_OPT_SIZE(buf);
        }
    }
}

static void
create_token(str &token)
{
  static unsigned long i;

  ACE_OS::snprintf((char *)token.s, 8, "hello%lu",i++);

  token.length = strlen((char *)token.s);
}

static coap_pdu_t *
coap_new_request(coap_context_t *ctx, unsigned char m, coap_list_t *&options, std::string &payload)
{
    coap_pdu_t *pdu;
    coap_list_t *opt;

    unsigned char _token_data[8];
    str the_token = { 0, _token_data };

    create_token(the_token);

    if ( ! ( pdu = coap_new_pdu() ) )
    return NULL;

    pdu->hdr->type = COAP_MESSAGE_NON;
    pdu->hdr->id = coap_new_message_id(ctx);
    pdu->hdr->code = m;

    pdu->hdr->token_length = the_token.length;

    if ( !coap_add_token(pdu, the_token.length, the_token.s))
    {
        ACE_DEBUG((LM_DEBUG,"cannot add token to request\n"));
    }

    for (opt = options; opt; opt = opt->next)
    {
        coap_add_option(pdu, COAP_OPTION_KEY(*(coap_option *)opt->data),
        	    COAP_OPTION_LENGTH(*(coap_option *)opt->data),
        	    COAP_OPTION_DATA(*(coap_option *)opt->data));
    }

    if (payload.length())
    {
      coap_add_data(pdu, payload.length(), (const unsigned char*)payload.c_str());
    }

    return pdu;
}

static int
resolve_address(const str *server, struct sockaddr *dst) {

  struct addrinfo *res, *ainfo;
  struct addrinfo hints;
  static char addrstr[256];
  int error, len=-1;

  memset(addrstr, 0, sizeof(addrstr));
  if (server->length)
    memcpy(addrstr, server->s, server->length);
  else
    memcpy(addrstr, "localhost", 9);

  memset ((char *)&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_family = AF_UNSPEC;

  error = getaddrinfo(addrstr, "", &hints, &res);

  if (error != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
    return error;
  }

  for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
    switch (ainfo->ai_family) {
    case AF_INET6:
    case AF_INET:
      len = ainfo->ai_addrlen;
      memcpy(dst, ainfo->ai_addr, len);
      goto finish;
    default:
      ;
    }
  }

 finish:
  freeaddrinfo(res);
  return len;
}

void ZigbeeServer::register_resource_to_rd(CoAPResource *r)
{
   char buf[0xff];

   std::string payload = r->get_payload();
   std::string ep = r->get_ep();
   std::string uri = r->uri();

   ACE_OS::sprintf(buf, "coap://127.0.0.1:%d/rd?ep=%s",_confPtr->_rdAddrPort,
                                                       ep.c_str());

    std::string full_url = buf;

    // send coap request to rd.
    {
        coap_list_t *optlist = NULL;
        coap_uri_t uri;

        coap_address_t dst;
        char addr[INET6_ADDRSTRLEN];
        void *addrptr = NULL;

        coap_pdu_t  *pdu;
        str server;
        unsigned short port = COAP_DEFAULT_PORT;
        int opt, res;

        unsigned char method = 2;

        handle_full_uri(full_url, uri, optlist);

        server = uri.host;
        port = uri.port;

        /* resolve destination address where server should be sent */
        res = resolve_address(&server, &dst.addr.sa);

        if (res < 0)
        {
            ACE_DEBUG((LM_DEBUG,"failed to resolve address\n"));
        }

        dst.size = res;
        dst.addr.sin.sin_port = htons(port);

        /* add Uri-Host if server address differs from uri.host */
        switch (dst.addr.sa.sa_family)
        {
        case AF_INET:
          addrptr = &dst.addr.sin.sin_addr;
          break;
        case AF_INET6:
          addrptr = &dst.addr.sin6.sin6_addr;

          break;
        default:
          ;
        }

        if (addrptr
            && (inet_ntop(dst.addr.sa.sa_family, addrptr, addr, sizeof(addr)) != 0)
            && (strlen(addr) != uri.host.length
            || memcmp(addr, uri.host.s, uri.host.length) != 0))
        {
            /* add Uri-Host */
            coap_insert(&optlist, new_option_node(COAP_OPTION_URI_HOST,
                            uri.host.length, uri.host.s),
              order_opts);
        }

        if (!(pdu = coap_new_request(_coapWrapperPtr->coap_ctx_, method, optlist, payload)))
        {
            ACE_DEBUG((LM_DEBUG, "cant create pdu\n"));
        }
        else
        {
            //coap_show_pdu(pdu);
            coap_send(_coapWrapperPtr->coap_ctx_, &dst, pdu);
            coap_delete_pdu(pdu);
        }

        coap_delete_list(optlist);
    }
}


}
}

