/*
* This file defines zigbee coap resource
*/

#include "Zigbee_CoAP_Resource.h"
#include "CoAP_Wrapper.h"
#include "CoAP_Resource.h"
#include "Zigbee_Node.h"
#include "Zigbee_Serialport_Command.h"
#include "Zigbee_Request.h"

#include "coap.h"
#include "cJSON.h"


// ZCL header - frame control field
typedef struct
{
  unsigned int type:2;
  unsigned int manuSpecific:1;
  unsigned int direction:1;
  unsigned int disableDefaultRsp:1;
  unsigned int reserved:3;
} zclFrameControl_t;

// ZCL header
typedef struct
{
  zclFrameControl_t fc;
  unsigned short manuCode;
  unsigned char  transSeqNum;
  unsigned char  commandID;
} zclFrameHdr_t;

typedef struct device_id_name_table
{
    unsigned char id0;
    unsigned char id1;
    const char *name;
}device_id_name_table;

static device_id_name_table zigbee_device_name_table[0xff] =
{
    {0x00,0x00,"OnOff Switch"},
    {0x00,0x01,"Level Control Switch" },
    {0x00,0x03,"OnOff Output" },
    {0x00,0x03,"Level Controllable Output" },
    {0x00,0x04,"Scene Selector"},
    {0x00,0x05,"Configuration Tool"},
    {0x00,0x06,"Remote Control"},
    {0x00,0x07,"Combined Interface" },
    {0x00,0x08,"Range Extender" },
    {0x00,0x09,"Mains Power Outlet" },
    {0x00,0x0A,"Door Lock" },
    {0x00,0x0B,"Door Lock Controller" },
    {0x00,0x0C,"Simple Sensor"},
    {0x00,0x0D,"Consumption Awareness Device" },
    {0x00,0x50,"Home Gateway" },
    {0x00,0x51,"Smart plug" },
    {0x00,0x52,"White Goods"},
    {0x00,0x53,"Meter Interface"},
    {0x01,0x01,"Dimmable Light"},
    {0x01,0x02,"Color Dimmable Light On/Off Light"},
    {0x01,0x03,"On/Off Light Switch"},
    {0x01,0x04,"Dimmer Switch"},
    {0x01,0x05,"Color Dimmer Switch"},
    {0x01,0x06,"Light Sensor"},
    {0x01,0x07,"Occupancy Sensor"}
};

static std::string find_device_type_by_id(unsigned char device[2])
{
    for (int i=0; i < 0xff; i++)
    {
        if(zigbee_device_name_table[i].id0 == device[0] &&
           zigbee_device_name_table[i].id1 == device[1] )

           return std::string(zigbee_device_name_table[i].name);
    }

    return "unknow";
}

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

static unsigned char *zclBuildHdr( zclFrameHdr_t *hdr, unsigned char *pData )
{
  // Build the Frame Control byte
  *pData = hdr->fc.type;
  *pData |= hdr->fc.manuSpecific << 2;
  *pData |= hdr->fc.direction << 3;
  *pData |= hdr->fc.disableDefaultRsp << 4;
  pData++;  // move past the frame control field

  // Add the manfacturer code
  if ( hdr->fc.manuSpecific )
  {
    *pData++ = LO_UINT16( hdr->manuCode );
    *pData++ = HI_UINT16( hdr->manuCode );
  }

  // Add the Transaction Sequence Number
  *pData++ = hdr->transSeqNum;

  // Add the Cluster's command ID
  *pData++ = hdr->commandID;

  // Should point to the frame payload
  return ( pData );
}


ZigbeeCoapResource::ZigbeeCoapResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{

}

ZigbeeCoapResource::~ZigbeeCoapResource()
{
}

void ZigbeeCoapResource::set_zigbee_ep(unsigned char ep)
{
    zigbee_ep_ = ep;
}

void ZigbeeCoapResource::set_zigbee_node(ZigbeeNode *node)
{
    zigbee_node_ = node;
}

void ZigbeeCoapResource::set_zigbee_desc(NodeSimpleDesc *desc)
{
    zigbee_desc_ = desc;
}

static void addr_to_string(unsigned char ieee_addr[8], std::string &str_addr)
{

    char buf[0xff];
    ACE_OS::sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x",ieee_addr[0],
                                                          ieee_addr[1],
                                                          ieee_addr[2],
                                                          ieee_addr[3],
                                                          ieee_addr[4],
                                                          ieee_addr[5],
                                                          ieee_addr[6],
                                                          ieee_addr[7]
                                                          );
    str_addr = buf;
}

void* ZigbeeCoapResource::Create()
{
    std::string str_addr;
    addr_to_string(zigbee_node_->get_ieee_addr(), str_addr);

    char buf[0xff];

    ACE_OS::sprintf(buf, "%s/%x", str_addr.c_str(), zigbee_ep_);
    std::string resource_uri = buf;
    std::string resource_rt = find_device_type_by_id(zigbee_desc_->deviceid_);
    ACE_OS::sprintf(buf, "%s-%x", str_addr.c_str(), zigbee_ep_);
    std::string resource_ep = buf;
    ACE_OS::sprintf(buf, "</%s>;ct=41;rt=\"%s\";if=\"sensor\"", resource_uri.c_str(),resource_rt.c_str());
    std::string payload = buf;


    CoAP_Attr attr;

    SetURI(resource_uri);
    attr.insert(std::make_pair("rt",resource_rt));
    attr.insert(std::make_pair("et",resource_rt));
    attr.insert(std::make_pair("ep",resource_ep));

    SetAttr(attr);

    set_payload(payload);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    return rd_resource;
}

void ZigbeeCoapResource::handler_get(CoAPCallback &callback)
{
    std::string payload;

    CoAPResource::handler_get(callback);
}

void ZigbeeCoapResource::handler_put(CoAPCallback &callback)
{
    std::string payload;
    bool check_flag = false;

    get_wrapper()->get_payload(callback, payload);

    if (payload.empty())
    {
        ACE_DEBUG((LM_DEBUG, "the payload is empty\n"));
    }
    else // json formate,
    {
        cJSON *result = cJSON_Parse(payload.c_str());

        if (result == 0 )
        {
            ACE_DEBUG((LM_DEBUG, "failed to parse json string(%s)\n",payload.c_str()));
        }
        else //get cluster id
        {
            cJSON *cluster_id = cJSON_GetObjectItem(result, "cluster_id");

            if (cluster_id != 0) // get command id
            {
                cJSON *command_id = cJSON_GetObjectItem(result, "command_id");

                if (command_id != 0)
                {
                    if (cluster_id->type == cJSON_Number &&
                        command_id->type == cJSON_Number)
                    {
                        unsigned short clusterid = cluster_id->valueint;
                        unsigned cmdid = command_id->valueint;

                        if (clusterid == 0x06 &&
                            cmdid >=0 && cmdid <=2) // OnOff cluster
                        {
                            check_flag = true;
                            do_on_off_cmd(cmdid);
                        }
                        else if (clusterid == 0x03 &&
                            cmdid>=0 && cmdid < 0x01) // identify cluster
                        {
                            cJSON *attributes = cJSON_GetObjectItem(result, "attributes");

                            if (attributes != 0)
                            {
                                  cJSON *identify_time = cJSON_GetObjectItem(attributes, "IdentifyTime");

                                  if (identify_time != 0)
                                  {
                                        if (identify_time->type == cJSON_Number)
                                        {
                                            unsigned int time_value = identify_time->valueint;

                                            check_flag = true;
                                            do_identify(cmdid, time_value);
                                        }
                                  }
                            }

                        }
                    }
                }
             }

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

void ZigbeeCoapResource::do_on_off_cmd(unsigned char id)
{
    zclFrameHdr_t zcl_hdr;
    unsigned char data_buf[0xff];
    unsigned char *zcl_data_buf = 0;
    unsigned char data_buf_len = 0;

    ACE_OS::memset(&zcl_hdr, 0, sizeof(zclFrameHdr_t));

    zcl_hdr.fc.type = 0x01;
    zcl_hdr.commandID = id;
    zcl_hdr.transSeqNum = 0;

    zcl_data_buf = zclBuildHdr(&zcl_hdr, data_buf);
    data_buf_len = zcl_data_buf - data_buf;

    if (zigbee_node_)
    {
        unsigned char cluster_id[2] = {0x06,0x00};

        ZigbeeSerialportCommand * c =
        ZigbeeSerialportCommand::create_AF_DATA_REQ_cmd(zigbee_node_->get_short_addr(),
                                                        zigbee_ep_,
                                                        zigbee_node_->get_bind_ep(),
                                                        cluster_id,
                                                        data_buf_len,
                                                        data_buf
                                                        );
        ZigbeeRequest *req = new ZigbeeRequest();
        req->set_cmd(c);
        req->get();
    }

}

void ZigbeeCoapResource::do_identify(unsigned char id, unsigned short time_value)
{
    zclFrameHdr_t zcl_hdr;
    unsigned char data_buf[0xff];
    unsigned char *zcl_data_buf = 0;
    unsigned char data_buf_len = 0;

    ACE_OS::memset(&zcl_hdr, 0, sizeof(zclFrameHdr_t));

    zcl_hdr.fc.type = 0x01;
    zcl_hdr.commandID = id;
    zcl_hdr.transSeqNum = 0;

    zcl_data_buf = zclBuildHdr(&zcl_hdr, data_buf);
    data_buf_len = zcl_data_buf - data_buf;

    zcl_data_buf[0] = LO_UINT16(time_value);
    zcl_data_buf[1] = HI_UINT16(time_value);
    data_buf_len  += 2;

    if (zigbee_node_)
    {
        unsigned char cluster_id[2] = {0x03,0x00};

        ZigbeeSerialportCommand * c =
        ZigbeeSerialportCommand::create_AF_DATA_REQ_cmd(zigbee_node_->get_short_addr(),
                                                        zigbee_ep_,
                                                        zigbee_node_->get_bind_ep(),
                                                        cluster_id,
                                                        data_buf_len,
                                                        data_buf
                                                        );
        ZigbeeRequest *req = new ZigbeeRequest();
        req->set_cmd(c);
        req->get();
    }
}
