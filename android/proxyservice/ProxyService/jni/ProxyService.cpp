
/*
 * This file implement ProxyService Interface
 *
 *  1. startService
 *  2. stopSercvice
 *
 *  ProxyService forwards these invokes to other modules
 *  that need to be implemented by business
 */

#include "com_hisilicon_proxyservice_ProxyService.h"
#include "ServiceManager.h"

/*
 * Class:     com_hisilicon_proxyservice_ProxyService
 * Method:    startService
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_startService
  (JNIEnv *, jobject)
{
	return ServiceManager::instance()->startService();
}

/*
 * Class:     com_hisilicon_proxyservice_ProxyService
 * Method:    stopSerice
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_stopSerice
  (JNIEnv *, jobject)
{
	return ServiceManager::instance()->stopService();

}

