/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_hisilicon_proxyservice_ProxyService */

#ifndef _Included_com_hisilicon_proxyservice_ProxyService
#define _Included_com_hisilicon_proxyservice_ProxyService
#ifdef __cplusplus
extern "C" {
#endif
    /*
     * Class:     com_hisilicon_proxyservice_ProxyService
     * Method:    startService
     * Signature: ()I
     */
    JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_startService
    (JNIEnv *, jobject);

    /*
     * Class:     com_hisilicon_proxyservice_ProxyService
     * Method:    stopSerice
     * Signature: ()I
     */
    JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_stopSerice
    (JNIEnv *, jobject);

    /*
     * Class:     com_hisilicon_proxyservice_ProxyService
     * Method:    init
     * Signature: ()I
     */
    JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_init
    (JNIEnv *, jobject);

    JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved);
    JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved);


#ifdef __cplusplus
}
#endif
#endif
