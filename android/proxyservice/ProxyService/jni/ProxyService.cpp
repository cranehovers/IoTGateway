/*
 * This file implement ProxyService Interface
 *
 *  1. startService
 *  2. stopSercvice
 *
 *  ProxyService forwards these invokes to other modules
 *  that need to be implemented by business
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "com_hisilicon_proxyservice_ProxyService.h"
#include "ServiceManager.h"
#include "ServiceLog.h"

static JNIEnv *jniEnv = 0;
static jclass jProxyServiceClass = 0;
static jobject jProxyServiceObject = 0;
static jmethodID jLogMethod = 0;
static JavaVM *jniVM = 0;

#define LOG_BUF_SIZE    512


void set_correct_jnienv()
{
    jniVM->AttachCurrentThread(&jniEnv, NULL);
    jProxyServiceClass = (jniEnv)->GetObjectClass(jProxyServiceObject);
    jLogMethod = (jniEnv)->GetMethodID(jProxyServiceClass, "log","(Ljava/lang/String;)V");

}

void printOutput(const char *fmt, ...)

{
    set_correct_jnienv();

    if (jProxyServiceClass != 0 && jProxyServiceObject != 0 &&  jLogMethod != 0)
    {
        va_list ap;
        char buf[LOG_BUF_SIZE];

        va_start(ap, fmt);
        vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
        va_end(ap);

        jstring jstrMSG = 0;
        jstrMSG =(jniEnv)->NewStringUTF(buf);
        (jniEnv)->CallVoidMethod(jProxyServiceObject, jLogMethod,jstrMSG);
        (jniEnv)->DeleteLocalRef(jstrMSG);
        jniVM->DetachCurrentThread();
    }
}

static void init_java_env(JNIEnv * jni_env, jobject jni_object)
{
    jniEnv = jni_env;
    jProxyServiceObject = (jobject)jni_env->NewGlobalRef(jni_object);

    #if 0
    if(jProxyServiceClass ==0)
    {

        jclass tmp = (jni_env)->FindClass("com/hisilicon/proxyservice/ProxyService");
        jProxyServiceClass = (jclass)jni_env->NewGlobalRef(tmp);

        if(jProxyServiceClass == 0)
        {
            jProxyServiceClass = 0;
            jProxyServiceObject = 0;
            jLogMethod = 0;

            return ;
        }


        jProxyServiceObject = (jobject)jni_env->NewGlobalRef(jni_object);


        if (jLogMethod == 0)
        {
            jLogMethod = (jni_env)->GetMethodID(jProxyServiceClass, "log","(Ljava/lang/String;)V");
            if (jLogMethod == 0)
            {
                /*
                * FIXME: where to release these globale objects?
                */

                //(jni_env)->DeleteLocalRef(jProxyServiceClass);
                //(jni_env)->DeleteLocalRef(jProxyServiceObject);

                jProxyServiceClass = 0;
                jProxyServiceObject = 0;
                jLogMethod = 0;

                return ;
            }
        }
    }

    #endif
}


#define JNIREG_CLASS "com/hisilicon/proxyservice/ProxyService"

static JNINativeMethod gMethods[] = {
	{ "startService", "()I", (void*)Java_com_hisilicon_proxyservice_ProxyService_startService },
    { "stopService", "()I", (void*)Java_com_hisilicon_proxyservice_ProxyService_stopSerice },
    { "init", "()I", (void*)Java_com_hisilicon_proxyservice_ProxyService_init }
};


/*
* Register several native methods for one class.
*/
static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	clazz = env->FindClass(className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}


/*
* Register native methods for all classes we know about.
*/
static int registerNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, JNIREG_CLASS, gMethods,
                                 sizeof(gMethods) / sizeof(gMethods[0])))
		return JNI_FALSE;

	return JNI_TRUE;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	jniVM = vm;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
	{
		return -1;
	}

	if (!registerNatives(env))
	{
		return -1;
	}

	result = JNI_VERSION_1_4;

	return result;
}


JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved)
{

}


/*
 * Class:     com_hisilicon_proxyservice_ProxyService
 * Method:    stopSerice
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_init
(JNIEnv * jni_env, jobject jni_object)
{
    init_java_env(jni_env, jni_object);

    //printOutput("Load service from configure file");

    ServiceManager::instance()->loadService();
    ServiceManager::instance()->setPrintOutput(printOutput);
    ServiceManager::instance()->initService();

    return 0;
}


/*
 * Class:     com_hisilicon_proxyservice_ProxyService
 * Method:    startService
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_startService
(JNIEnv *jni_env, jobject jni_object)
{
    //printOutput("Call Proxy Service startService interface");

    return ServiceManager::instance()->startService();
}

/*
 * Class:     com_hisilicon_proxyservice_ProxyService
 * Method:    stopSerice
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_hisilicon_proxyservice_ProxyService_stopSerice
(JNIEnv *jni_env, jobject jni_object)
{
    //printOutput("Call Proxy Service stopService interface");

    return ServiceManager::instance()->stopService();

}

