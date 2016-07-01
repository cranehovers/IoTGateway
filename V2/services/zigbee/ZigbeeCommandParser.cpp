/*
* This file defines parser
*/

#include <toolkit/ACE_Header_File.h>
#include <services/zigbee/ZigbeeCommand.h>
#include <services/zigbee/ZigbeeCommandParser.h>


#define SOP_STATE      0x00
#define CMD_STATE1     0x01
#define CMD_STATE2     0x02
#define LEN_STATE      0x03
#define DATA_STATE     0x04
#define FCS_STATE      0x05
#define MT_UART_SOF    0xFE


namespace Services{
namespace Zigbee{

void DataFrame::copy(unsigned char *buf, unsigned char &len)
{
    buf[0] = 0xFE;
    buf[1] = token_len;
    buf[2] = cmd0;
    buf[3] = cmd1;

    ACE_OS::memcpy(&buf[4], data, token_len);

    buf[token_len + 4] = fcs;

    len = token_len + 5;
}


ZigbeeCommandParser::ZigbeeCommandParser()
{
    current_frame = new DataFrame();
    frame_list.clear();
}

ZigbeeCommandParser::~ZigbeeCommandParser()
{
    clear();
}

void ZigbeeCommandParser::clear()
{
    if (current_frame)
    {
        delete current_frame;
        current_frame = 0;
    }

    std::list<DataFrame*>::iterator e;
    e = frame_list.begin();

    for (;e != frame_list.end(); ++e)
    {
        DataFrame *frame = (*e);
        delete frame;
    }

    frame_list.clear();
}

int ZigbeeCommandParser::add_data(unsigned char *data, unsigned int len)
{
    unsigned char  ch;
    unsigned char  bytesInRxBuffer;

    unsigned char  LEN_Token = current_frame->token_len;
    unsigned char  tempDataLen = current_frame->copied_len;

    //ACE_DEBUG((LM_DEBUG, "\nlen=%d(copied=%d, len=%d)\n", len,
    //    current_frame->copied_len,current_frame->token_len));

    for (int i=0; i < len;)
    {
      ch = data[i];

      //ACE_DEBUG((LM_DEBUG, "(%d)%x ", i,ch));

      switch (current_frame->state)
      {
        case SOP_STATE:
          if (ch == MT_UART_SOF)
          {
            current_frame->state = LEN_STATE;

            //ACE_DEBUG((LM_DEBUG, "(*%d)%x ", i,ch));
          }
          break;

        case LEN_STATE:
          LEN_Token = ch;
          tempDataLen = current_frame->copied_len;

          /* Allocate memory for the data */
          if (LEN_Token > 0)
          {
            current_frame->data = new unsigned char[LEN_Token];
          }

          if (current_frame->data)
          {
            current_frame->token_len = LEN_Token;
            current_frame->state = CMD_STATE1;
          }
          else
          {
            current_frame->state = SOP_STATE;
            return 0;
          }
          //ACE_DEBUG((LM_DEBUG, "(*%d)%x ", i,ch));
          break;

        case CMD_STATE1:
          current_frame->cmd0 = ch;
          current_frame->state = CMD_STATE2;
          //ACE_DEBUG((LM_DEBUG, "(*%d)%x ", i,ch));
          break;

        case CMD_STATE2:
          current_frame->cmd1 = ch;
          /* If there is no data, skip to FCS state */
          if (LEN_Token)
          {
            current_frame->state = DATA_STATE;
          }
          else
          {
            current_frame->state = FCS_STATE;
          }

          //ACE_DEBUG((LM_DEBUG, "(*%d)%x ", i,ch));

          break;

        case DATA_STATE:

          //ACE_DEBUG((LM_DEBUG, "(*%d)%x ", i,ch));

          /* Check number of bytes left in the Rx buffer */
          bytesInRxBuffer = (len-i);

          /* If the remain of the data is there, read them all, otherwise, just read enough */
          if (bytesInRxBuffer <= LEN_Token - tempDataLen)
          {
            ACE_OS::memcpy( &(current_frame->data[tempDataLen]), &data[i], bytesInRxBuffer);
            tempDataLen += bytesInRxBuffer;
            current_frame->copied_len += bytesInRxBuffer;
          }
          else
          {
            ACE_OS::memcpy( &(current_frame->data[tempDataLen]), &data[i], LEN_Token - tempDataLen);
            current_frame->copied_len += (LEN_Token - tempDataLen);
            bytesInRxBuffer = (LEN_Token - tempDataLen);
          }

          //for(int j=0; j < bytesInRxBuffer; j++)
          //{
            //ACE_DEBUG((LM_DEBUG, "(**%d)%x ", i+j,data[i+j]));
          //}

          /* If number of bytes read is equal to data length, time to move on to FCS */
          if ( current_frame->copied_len == LEN_Token )
              current_frame->state = FCS_STATE;

          i += (bytesInRxBuffer-1);

          //ACE_DEBUG((LM_DEBUG, "(**%d)i ", i));

          break;

        case FCS_STATE:

          current_frame->fcs = ch;

          // FIXME: todo xor
          frame_list.push_back(current_frame);
          current_frame = new DataFrame();

          /* Reset the state, send or discard the buffers at this point */
          current_frame->state = SOP_STATE;

          //ACE_DEBUG((LM_DEBUG, "(*%d)%x ", i,ch));

          break;

        default:
         break;
      }

      i +=1;
    }

    return 0;
}

ZigbeeCommand *ZigbeeCommandParser::get_frame()
{
    unsigned char buf[0xff];
    unsigned char len;

    if (frame_list.empty())
    {
        return 0;
    }

    DataFrame *frame = frame_list.front();
    frame_list.pop_front();

    frame->copy(buf, len);
    delete frame;

    return ZigbeeCommand::create_from_buffer(buf, len);
}



}
}
