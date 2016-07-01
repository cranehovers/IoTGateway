/*
* This file defines COMMON configuration
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#undef ACE_NLOGGING

#include <stdlib.h>
#include <stdio.h>
#include <ace/Log_Msg.h>
#include <ace/Singleton.h>
#include <ace/Mutex.h>
#include <ace/Null_Mutex.h>
#include <ace/Configuration_Import_Export.h>
#include <ace/INET_Addr.h>
#include <ace/Reactor.h>
#include <ace/Task.h>
#include <ace/Select_Reactor.h>
#include <ace/Thread_Manager.h>
#include <ace/SOCK_Dgram.h>
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/OS_NS_string.h>
#include <ace/OS_NS_unistd.h>
#include <ace/os_include/os_netdb.h>
#include <ace/DEV_Addr.h>
#include <ace/DEV_Connector.h>
#include <ace/TTY_IO.h>
#include "ace/Message_Queue.h"
#include <ace/Synch.h>
#include <ace/Log_Record.h>
#include <ace/Log_Msg_Callback.h>
#include <ace/ACE.h>

extern "C" {
 extern void init_ace_log();
}


#endif //__CONFIG_H__





