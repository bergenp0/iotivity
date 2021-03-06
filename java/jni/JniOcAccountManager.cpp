/*
 *******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#include "JniOcAccountManager.h"
#include "JniOcRepresentation.h"
#include "JniUtils.h"

#define VERIFY_NON_NULL_THROW_EXCEPTION(arg, log_message, exc) \
    if (!(arg)) \
    { \
        ThrowOcException(exc, log_message); \
        return; \
    } \

JniOcAccountManager::JniOcAccountManager(std::shared_ptr<OCAccountManager> accountManager)
    : m_sharedAccountManager(accountManager)
{
}

JniOcAccountManager::~JniOcAccountManager()
{
    LOGD("~JniOcAccountManager()");

    m_sharedAccountManager = nullptr;

    jint envRet = JNI_ERR;
    JNIEnv *env = GetJNIEnv(envRet);
    if (nullptr == env)
    {
        return;
    }

    m_onGetManager.removeAllListeners(env);
    m_onPostManager.removeAllListeners(env);
    m_onDeleteManager.removeAllListeners(env);
    m_onObserveManager.removeAllListeners(env);

    if (JNI_EDETACHED == envRet)
    {
        g_jvm->DetachCurrentThread();
    }
}

std::string JniOcAccountManager::host()
{
    return m_sharedAccountManager->host();
}

OCConnectivityType JniOcAccountManager::connectivityType() const
{
    return m_sharedAccountManager->connectivityType();
}

JniOcAccountManager* JniOcAccountManager::getJniOcAccountManagerPtr(JNIEnv *env, jobject thiz)
{
    JniOcAccountManager *accountManager = GetHandle<JniOcAccountManager>(env, thiz);
    if (env->ExceptionCheck())
    {
        LOGE("Failed to get native handle from OcAccountManager");
    }
    if (!accountManager)
    {
        ThrowOcException(JNI_NO_NATIVE_POINTER, "");
    }
    return accountManager;
}

JniOnGetListener* JniOcAccountManager::addOnGetListener(JNIEnv* env, jobject jListener)
{
    return this->m_onGetManager.addListener(env, jListener, this);
}

JniOnPostListener* JniOcAccountManager::addOnPostListener(JNIEnv* env, jobject jListener)
{
    return this->m_onPostManager.addListener(env, jListener, this);
}

JniOnDeleteListener* JniOcAccountManager::addOnDeleteListener(JNIEnv* env, jobject jListener)
{
    return this->m_onDeleteManager.addListener(env, jListener, this);
}

JniOnObserveListener* JniOcAccountManager::addOnObserveListener(JNIEnv* env, jobject jListener)
{
    return this->m_onObserveManager.addListener(env, jListener, this);
}

void JniOcAccountManager::removeOnGetListener(JNIEnv* env, jobject jListener)
{
    this->m_onGetManager.removeListener(env, jListener);
}

void JniOcAccountManager::removeOnPostListener(JNIEnv* env, jobject jListener)
{
    this->m_onPostManager.removeListener(env, jListener);
}

void JniOcAccountManager::removeOnDeleteListener(JNIEnv* env, jobject jListener)
{
    this->m_onDeleteManager.removeListener(env, jListener);
}

void JniOcAccountManager::removeOnObserveListener(JNIEnv* env, jobject jListener)
{
    this->m_onObserveManager.removeListener(env, jListener);
}

OCStackResult JniOcAccountManager::signUp(JNIEnv* env, const std::string& authProvider,
                                          const std::string& authCode, jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->signUp(authProvider, authCode, postCallback);
}

OCStackResult JniOcAccountManager::signUp(JNIEnv* env, const std::string& authProvider,
                                          const std::string& authCode,
                                          const QueryParamsMap& options, jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->signUp(authProvider, authCode, options, postCallback);
}

OCStackResult JniOcAccountManager::signIn(JNIEnv* env, const std::string& userUuid,
                                          const std::string& accessToken, jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->signIn(userUuid, accessToken, postCallback);
}

OCStackResult JniOcAccountManager::signOut(JNIEnv* env, const std::string& accessToken,
                                           jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->signOut(accessToken, postCallback);
}

OCStackResult JniOcAccountManager::refreshAccessToken(JNIEnv* env, const std::string& userUuid,
                                                      const std::string& refreshToken,
                                                      jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->refreshAccessToken(userUuid, refreshToken, postCallback);
}

OCStackResult JniOcAccountManager::searchUser(JNIEnv* env, const QueryParamsMap& queryMap,
                                              jobject jListener)
{
    JniOnGetListener *onGetListener = addOnGetListener(env, jListener);
    if (nullptr == onGetListener)
    {
        LOGE("onGetListener is null");
        return OC_STACK_ERROR;
    }

    GetCallback getCallback = [onGetListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onGetListener->onGetCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->searchUser(queryMap, getCallback);
}

OCStackResult JniOcAccountManager::deleteDevice(JNIEnv* env, const std::string& accessToken,
                                                const std::string& deviceId, jobject jListener)
{
    JniOnDeleteListener *onDeleteListener = addOnDeleteListener(env, jListener);
    if (nullptr == onDeleteListener)
    {
        LOGE("onDeleteListener is null");
        return OC_STACK_ERROR;
    }

    DeleteCallback deleteCallback = [onDeleteListener](const HeaderOptions& opts,
        const int eCode)
    {
        onDeleteListener->onDeleteCallback(opts, eCode);
    };

    return m_sharedAccountManager->deleteDevice(accessToken, deviceId, deleteCallback);
}

OCStackResult JniOcAccountManager::createGroup(JNIEnv* env, jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->createGroup(postCallback);
}

OCStackResult JniOcAccountManager::createGroup(JNIEnv* env, const QueryParamsMap& queryMap,
                                               jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->createGroup(queryMap, postCallback);
}

OCStackResult JniOcAccountManager::deleteGroup(JNIEnv* env, const std::string& groupId,
                                               jobject jListener)
{
    JniOnDeleteListener *onDeleteListener = addOnDeleteListener(env, jListener);
    if (nullptr == onDeleteListener)
    {
        LOGE("onDeleteListener is null");
        return OC_STACK_ERROR;
    }

    DeleteCallback deleteCallback = [onDeleteListener](const HeaderOptions& opts,
        const int eCode)
    {
        onDeleteListener->onDeleteCallback(opts, eCode);
    };

    return m_sharedAccountManager->deleteGroup(groupId, deleteCallback);
}

OCStackResult JniOcAccountManager::getGroupInfoAll(JNIEnv* env, jobject jListener)
{
    JniOnGetListener *onGetListener = addOnGetListener(env, jListener);
    if (nullptr == onGetListener)
    {
        LOGE("onGetListener is null");
        return OC_STACK_ERROR;
    }

    GetCallback getCallback = [onGetListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onGetListener->onGetCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->getGroupInfoAll(getCallback);
}

OCStackResult JniOcAccountManager::getGroupInfo(JNIEnv* env, const std::string& groupId,
                                                jobject jListener)
{
    JniOnGetListener *onGetListener = addOnGetListener(env, jListener);
    if (nullptr == onGetListener)
    {
        LOGE("onGetListener is null");
        return OC_STACK_ERROR;
    }

    GetCallback getCallback = [onGetListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onGetListener->onGetCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->getGroupInfo(groupId, getCallback);
}

OCStackResult JniOcAccountManager::addPropertyValueToGroup(JNIEnv* env, const std::string& groupId,
                                                           const OCRepresentation& propertyValue,
                                                           jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->addPropertyValueToGroup(groupId, propertyValue, postCallback);
}

OCStackResult JniOcAccountManager::deletePropertyValueFromGroup(JNIEnv* env,
                                                        const std::string& groupId,
                                                        const OCRepresentation& propertyValue,
                                                        jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->deletePropertyValueFromGroup(groupId, propertyValue,
                                                                postCallback);
}

OCStackResult JniOcAccountManager::updatePropertyValueOnGroup(JNIEnv* env,
                                                        const std::string& groupId,
                                                        const OCRepresentation& propertyValue,
                                                        jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->updatePropertyValueOnGroup(groupId, propertyValue,
                                                              postCallback);
}

OCStackResult JniOcAccountManager::observeGroup(JNIEnv* env, jobject jListener)
{
    JniOnObserveListener *onObserveListener = addOnObserveListener(env, jListener);
    if (nullptr == onObserveListener)
    {
        LOGE("onObserveListener is null");
        return OC_STACK_ERROR;
    }

    ObserveCallback observeCallback = [onObserveListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int& eCode, const int& sequenceNumber)
    {
        onObserveListener->onObserveCallback(opts, rep, eCode, sequenceNumber);
    };

    return m_sharedAccountManager->observeGroup(observeCallback);
}

OCStackResult JniOcAccountManager::cancelObserveGroup()
{
    return m_sharedAccountManager->cancelObserveGroup();
}

OCStackResult JniOcAccountManager::observeInvitation(JNIEnv* env, jobject jListener)
{
    JniOnObserveListener *onObserveListener = addOnObserveListener(env, jListener);
    if (nullptr == onObserveListener)
    {
        LOGE("onObserveListener is null");
        return OC_STACK_ERROR;
    }

    ObserveCallback observeCallback = [onObserveListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int& eCode, const int& sequenceNumber)
    {
        onObserveListener->onObserveCallback(opts, rep, eCode, sequenceNumber);
    };

    return m_sharedAccountManager->observeInvitation(observeCallback);
}

OCStackResult JniOcAccountManager::cancelObserveInvitation()
{
    return m_sharedAccountManager->cancelObserveInvitation();
}

OCStackResult JniOcAccountManager::sendInvitation(JNIEnv* env, const std::string& groupId,
                                                  const std::string& userUuid, jobject jListener)
{
    JniOnPostListener *onPostListener = addOnPostListener(env, jListener);
    if (nullptr == onPostListener)
    {
        LOGE("onPostListener is null");
        return OC_STACK_ERROR;
    }

    PostCallback postCallback = [onPostListener](const HeaderOptions& opts,
        const OCRepresentation& rep, const int eCode)
    {
        onPostListener->onPostCallback(opts, rep, eCode);
    };

    return m_sharedAccountManager->sendInvitation(groupId, userUuid, postCallback);
}

OCStackResult JniOcAccountManager::cancelInvitation(JNIEnv* env, const std::string& groupId,
                                                    const std::string& userUuid, jobject jListener)
{
    JniOnDeleteListener *onDeleteListener = addOnDeleteListener(env, jListener);
    if (nullptr == onDeleteListener)
    {
        LOGE("onDeleteListener is null");
        return OC_STACK_ERROR;
    }

    DeleteCallback deleteCallback = [onDeleteListener](const HeaderOptions& opts,
        const int eCode)
    {
        onDeleteListener->onDeleteCallback(opts, eCode);
    };

    return m_sharedAccountManager->cancelInvitation(groupId, userUuid, deleteCallback);
}

OCStackResult JniOcAccountManager::replyToInvitation(JNIEnv* env, const std::string& groupId,
                                                     const bool accept, jobject jListener)
{
    JniOnDeleteListener *onDeleteListener = addOnDeleteListener(env, jListener);
    if (nullptr == onDeleteListener)
    {
        LOGE("onDeleteListener is null");
        return OC_STACK_ERROR;
    }

    DeleteCallback deleteCallback = [onDeleteListener](const HeaderOptions& opts,
        const int eCode)
    {
        onDeleteListener->onDeleteCallback(opts, eCode);
    };

    return m_sharedAccountManager->replyToInvitation(groupId, accept, deleteCallback);
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    getHost
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_org_iotivity_base_OcAccountManager_getHost
    (JNIEnv *env, jobject thiz)
{
    LOGD("OcAccountManager_getHost");
    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return nullptr;
    }

    return env->NewStringUTF(accountManager->host().c_str());
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    getConnectivityTypeN
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_org_iotivity_base_OcAccountManager_getConnectivityTypeN
    (JNIEnv *env, jobject thiz)
{
    LOGD("OcAccountManager_getConnectivityType");
    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return -1;
    }

    OCConnectivityType connectivityType = accountManager->connectivityType();
    return static_cast<jint>(connectivityType);
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    signUp0
* Signature: (Ljava/lang/String;Ljava/lang/String;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signUp0
    (JNIEnv *env, jobject thiz, jstring jAuthProvider, jstring jAuthCode, jobject jListener)
{
    LOGD("OcAccountManager_signUp");
    VERIFY_NON_NULL_THROW_EXCEPTION(jAuthProvider, "authProvider cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jAuthCode, "authCode cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charAuthProvider = env->GetStringUTFChars(jAuthProvider, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charAuthProvider, "charAuthProvider is null", JNI_EXCEPTION);
    std::string authProvider(charAuthProvider);
    env->ReleaseStringUTFChars(jAuthProvider, charAuthProvider);

    const char *charAuthCode = env->GetStringUTFChars(jAuthCode, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charAuthCode, "charAuthCode is null", JNI_EXCEPTION);
    std::string authCode(charAuthCode);
    env->ReleaseStringUTFChars(jAuthCode, charAuthCode);

    try
    {
        OCStackResult result = accountManager->signUp(env,
                                                      authProvider,
                                                      authCode,
                                                      jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_signUp");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    signUp1
* Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/util/Map;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signUp1
    (JNIEnv *env, jobject thiz, jstring jAuthProvider, jstring jAuthCode, jobject jOptionsMap,
     jobject jListener)
{
    LOGD("OcAccountManager_signUp");
    VERIFY_NON_NULL_THROW_EXCEPTION(jAuthProvider, "authProvider cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jAuthCode, "authCode cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jOptionsMap, "options cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charAuthProvider = env->GetStringUTFChars(jAuthProvider, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charAuthProvider, "charAuthProvider is null", JNI_EXCEPTION);
    std::string authProvider(charAuthProvider);
    env->ReleaseStringUTFChars(jAuthProvider, charAuthProvider);

    const char *charAuthCode = env->GetStringUTFChars(jAuthCode, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charAuthCode, "charAuthCode is null", JNI_EXCEPTION);
    std::string authCode(charAuthCode);
    env->ReleaseStringUTFChars(jAuthCode, charAuthCode);

    QueryParamsMap optionsMap;
    JniUtils::convertJavaMapToQueryParamsMap(env, jOptionsMap, optionsMap);

    try
    {
        OCStackResult result = accountManager->signUp(env,
                                                      authProvider,
                                                      authCode,
                                                      optionsMap,
                                                      jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_signUp");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    signIn0
* Signature: (Ljava/lang/String;Ljava/lang/String;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signIn0
    (JNIEnv *env, jobject thiz, jstring jUserUuid, jstring jAccessToken, jobject jListener)
{
    LOGD("OcAccountManager_signIn");
    VERIFY_NON_NULL_THROW_EXCEPTION(jUserUuid, "userUuid cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jAccessToken, "accessToken cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charUserUuid = env->GetStringUTFChars(jUserUuid, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charUserUuid, "charUserUuid is null", JNI_EXCEPTION);
    std::string userUuid(charUserUuid);
    env->ReleaseStringUTFChars(jUserUuid, charUserUuid);

    const char *charAccessToken = env->GetStringUTFChars(jAccessToken, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charAccessToken, "charAccessToken is null", JNI_EXCEPTION);
    std::string accessToken(charAccessToken);
    env->ReleaseStringUTFChars(jAccessToken, charAccessToken);

    try
    {
        OCStackResult result = accountManager->signIn(env,
                                                      userUuid,
                                                      accessToken,
                                                      jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_signIn");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    signOut0
* Signature: (Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_signOut0
    (JNIEnv *env, jobject thiz, jstring jAccessToken, jobject jListener)
{
    LOGD("OcAccountManager_signOut");
    VERIFY_NON_NULL_THROW_EXCEPTION(jAccessToken, "accessToken cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charAccessToken = env->GetStringUTFChars(jAccessToken, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charAccessToken, "charAccessToken is null", JNI_EXCEPTION);
    std::string accessToken(charAccessToken);
    env->ReleaseStringUTFChars(jAccessToken, charAccessToken);

    try
    {
        OCStackResult result = accountManager->signOut(env,
                                                       accessToken,
                                                       jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_signOut");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    refreshAccessToken0
* Signature: (Ljava/lang/String;Ljava/lang/String;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_refreshAccessToken0
    (JNIEnv *env, jobject thiz, jstring jUserUuid, jstring jRefreshAccessToken, jobject jListener)
{
    LOGD("OcAccountManager_refreshAccessToken");
    VERIFY_NON_NULL_THROW_EXCEPTION(jUserUuid, "userUuid cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jRefreshAccessToken, "refreshAccessToken cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charUserUuid = env->GetStringUTFChars(jUserUuid, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charUserUuid, "charUserUuid is null", JNI_EXCEPTION);
    std::string userUuid(charUserUuid);
    env->ReleaseStringUTFChars(jUserUuid, charUserUuid);

    const char *charRefreshAccessToken = env->GetStringUTFChars(jRefreshAccessToken, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charRefreshAccessToken, "charRefreshAccessToken is null",
                                    JNI_EXCEPTION);
    std::string refreshAccessToken(charRefreshAccessToken);
    env->ReleaseStringUTFChars(jRefreshAccessToken, charRefreshAccessToken);

    try
    {
        OCStackResult result = accountManager->refreshAccessToken(env,
                                                                  userUuid,
                                                                  refreshAccessToken,
                                                                  jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_refreshAccessToken");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    searchUser0
* Signature: (Ljava/util/Map;Lorg/iotivity/base/OcAccountManager/OnGetListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_searchUser0
    (JNIEnv *env, jobject thiz, jobject jQueryMap, jobject jListener)
{
    LOGD("OcAccountManager_searchUser");
    VERIFY_NON_NULL_THROW_EXCEPTION(jQueryMap, "queryMap cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onGetListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    QueryParamsMap queryMap;
    JniUtils::convertJavaMapToQueryParamsMap(env, jQueryMap, queryMap);

    try
    {
        OCStackResult result = accountManager->searchUser(env,
                                                          queryMap,
                                                          jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_searchUser");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    deleteDevice0
* Signature: (Ljava/lang/String;Ljava/lang/String;
*             Lorg/iotivity/base/OcAccountManager/onDeleteListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_deleteDevice0
    (JNIEnv *env, jobject thiz, jstring jAccessToken, jstring jDeviceId, jobject jListener)
{
    LOGD("OcAccountManager_deleteDevice");
    VERIFY_NON_NULL_THROW_EXCEPTION(jAccessToken, "accessToken cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jDeviceId, "deviceId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onDeleteListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charAccessToken = env->GetStringUTFChars(jAccessToken, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charAccessToken, "charAccessToken is null", JNI_EXCEPTION);
    std::string accessToken(charAccessToken);
    env->ReleaseStringUTFChars(jAccessToken, charAccessToken);

    const char *charDeviceId = env->GetStringUTFChars(jDeviceId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charDeviceId, "charDeviceId is null", JNI_EXCEPTION);
    std::string deviceId(charDeviceId);
    env->ReleaseStringUTFChars(jDeviceId, charDeviceId);

    try
    {
        OCStackResult result = accountManager->deleteDevice(env,
                                                            accessToken,
                                                            deviceId,
                                                            jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_deleteDevice");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    createGroup0
* Signature: (Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_createGroup0
    (JNIEnv *env, jobject thiz, jobject jListener)
{
    LOGD("OcAccountManager_createGroup");
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    try
    {
        OCStackResult result = accountManager->createGroup(env,
                                                           jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_createGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    createGroup1
* Signature: (Ljava/util/Map;Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_createGroup1
    (JNIEnv *env, jobject thiz, jobject jQueryMap, jobject jListener)
{
    LOGD("OcAccountManager_createGroup");
    VERIFY_NON_NULL_THROW_EXCEPTION(jQueryMap, "queryMap cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    QueryParamsMap queryMap;
    JniUtils::convertJavaMapToQueryParamsMap(env, jQueryMap, queryMap);

    try
    {
        OCStackResult result = accountManager->createGroup(env,
                                                           queryMap,
                                                           jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_createGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    deleteGroup0
* Signature: (Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/onDeleteListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_deleteGroup0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jobject jListener)
{
    LOGD("OcAccountManager_deleteGroup");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onDeleteListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    try
    {
        OCStackResult result = accountManager->deleteGroup(env,
                                                           groupId,
                                                           jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_deleteGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    getGroupInfoAll0
* Signature: (Lorg/iotivity/base/OcAccountManager/OnGetListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_getGroupInfoAll0
    (JNIEnv *env, jobject thiz, jobject jListener)
{
    LOGD("OcAccountManager_getGroupInfoAll");
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onGetListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    try
    {
        OCStackResult result = accountManager->getGroupInfoAll(env,
                                                               jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_getGroupInfoAll");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    getGroupInfo0
* Signature: (Ljava/lang/String;Lorg/iotivity/base/OcAccountManager/OnGetListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_getGroupInfo0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jobject jListener)
{
    LOGD("OcAccountManager_getGroupInfo");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onGetListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    try
    {
        OCStackResult result = accountManager->getGroupInfo(env,
                                                            groupId,
                                                            jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_getGroupInfo");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    addPropertyValueToGroup0
* Signature: (Ljava/lang/String;Lorg/iotivity/base/OcRepresentation;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_addPropertyValueToGroup0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jobject jPropertyValue, jobject jListener)
{
    LOGD("OcAccountManager_addPropertyValueToGroup");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jPropertyValue, "propertyValue cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    OCRepresentation *propertyValue = JniOcRepresentation::getOCRepresentationPtr(env,
                                                                                  jPropertyValue);
    if (!propertyValue)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    try
    {
        OCStackResult result = accountManager->addPropertyValueToGroup(env,
                                                                       groupId,
                                                                       *propertyValue,
                                                                       jListener);

        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_addPropertyValueToGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    deletePropertyValueFromGroup0
* Signature: (Ljava/lang/String;Lorg/iotivity/base/OcRepresentation;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_deletePropertyValueFromGroup0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jobject jPropertyValue, jobject jListener)
{
    LOGD("OcAccountManager_deletePropertyValueFromGroup");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jPropertyValue, "propertyValue cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    OCRepresentation *propertyValue = JniOcRepresentation::getOCRepresentationPtr(env,
                                                                                  jPropertyValue);
    if (!propertyValue)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    try
    {
        OCStackResult result = accountManager->deletePropertyValueFromGroup(env,
                                                                            groupId,
                                                                            *propertyValue,
                                                                            jListener);

        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_deletePropertyValueFromGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    updatePropertyValueOnGroup0
* Signature: (Ljava/lang/String;Lorg/iotivity/base/OcRepresentation;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_updatePropertyValueOnGroup0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jobject jPropertyValue, jobject jListener)
{
    LOGD("OcAccountManager_updatePropertyValueOnGroup");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jPropertyValue, "propertyValue cannot be null",
                                    OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    OCRepresentation *propertyValue = JniOcRepresentation::getOCRepresentationPtr(env,
                                                                                  jPropertyValue);
    if (!propertyValue)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    try
    {
        OCStackResult result = accountManager->updatePropertyValueOnGroup(env,
                                                                          groupId,
                                                                          *propertyValue,
                                                                          jListener);

        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_updatePropertyValueOnGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    observeGroup0
* Signature: (Lorg/iotivity/base/OcResource/OnObserveListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_observeGroup0
    (JNIEnv *env, jobject thiz, jobject jListener)
{
    LOGD("OcAccountManager_observeGroup");
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onObserveListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    try
    {
        OCStackResult result = accountManager->observeGroup(env,
                                                            jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_observeGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    cancelObserveGroup0
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_cancelObserveGroup0
    (JNIEnv *env, jobject thiz)
{
    LOGD("OcAccountManager_cancelObserveGroup");

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    try
    {
        OCStackResult result = accountManager->cancelObserveGroup();
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_cancelObserveGroup");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    observeInvitation0
* Signature: (Lorg/iotivity/base/OcResource/OnObserveListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_observeInvitation0
    (JNIEnv *env, jobject thiz, jobject jListener)
{
    LOGD("OcAccountManager_observeInvitation");
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onObserveListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    try
    {
        OCStackResult result = accountManager->observeInvitation(env,
                                                                 jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_observeInvitation");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    cancelObserveInvitation0
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_cancelObserveInvitation0
    (JNIEnv *env, jobject thiz)
{
    LOGD("OcAccountManager_cancelObserveInvitation");

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    try
    {
        OCStackResult result = accountManager->cancelObserveInvitation();
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_cancelObserveInvitation");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    sendInvitation0
* Signature: (Ljava/lang/String;Ljava/lang/String;
*             Lorg/iotivity/base/OcAccountManager/OnPostListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_sendInvitation0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jstring jUserUuid, jobject jListener)
{
    LOGD("OcAccountManager_sendInvitation");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jUserUuid, "userUuid cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onPostListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    const char *charUserUuid = env->GetStringUTFChars(jUserUuid, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charUserUuid, "charUserUuid is null", JNI_EXCEPTION);
    std::string userUuid(charUserUuid);
    env->ReleaseStringUTFChars(jUserUuid, charUserUuid);

    try
    {
        OCStackResult result = accountManager->sendInvitation(env,
                                                              groupId,
                                                              userUuid,
                                                              jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_sendInvitation");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    cancelInvitation0
* Signature: (Ljava/lang/String;Ljava/lang/String;
*             Lorg/iotivity/base/OcAccountManager/onDeleteListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_cancelInvitation0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jstring jUserUuid, jobject jListener)
{
    LOGD("OcAccountManager_cancelInvitation");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jUserUuid, "userUuid cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onDeleteListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    const char *charUserUuid = env->GetStringUTFChars(jUserUuid, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charUserUuid, "charUserUuid is null", JNI_EXCEPTION);
    std::string userUuid(charUserUuid);
    env->ReleaseStringUTFChars(jUserUuid, charUserUuid);

    try
    {
        OCStackResult result = accountManager->cancelInvitation(env,
                                                                groupId,
                                                                userUuid,
                                                                jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_cancelInvitation");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}

/*
* Class:     org_iotivity_base_OcAccountManager
* Method:    replyToInvitation0
* Signature: (Ljava/lang/String;ZLorg/iotivity/base/OcAccountManager/onDeleteListener;)V
*/
JNIEXPORT void JNICALL Java_org_iotivity_base_OcAccountManager_replyToInvitation0
    (JNIEnv *env, jobject thiz, jstring jGroupId, jboolean jAccept, jobject jListener)
{
    LOGD("OcAccountManager_replyToInvitation");
    VERIFY_NON_NULL_THROW_EXCEPTION(jGroupId, "groupId cannot be null", OC_STACK_INVALID_PARAM);
    VERIFY_NON_NULL_THROW_EXCEPTION(jListener, "onDeleteListener cannot be null",
                                    OC_STACK_INVALID_PARAM);

    JniOcAccountManager *accountManager = JniOcAccountManager::getJniOcAccountManagerPtr(env,
                                                                                         thiz);
    if (!accountManager)
    {
        return;
    }

    const char *charGroupId = env->GetStringUTFChars(jGroupId, nullptr);
    VERIFY_NON_NULL_THROW_EXCEPTION(charGroupId, "charGroupId is null", JNI_EXCEPTION);
    std::string groupId(charGroupId);
    env->ReleaseStringUTFChars(jGroupId, charGroupId);

    try
    {
        OCStackResult result = accountManager->replyToInvitation(env,
                                                                 groupId,
                                                                 static_cast<bool>(jAccept),
                                                                 jListener);
        if (OC_STACK_OK != result)
        {
            ThrowOcException(result, "OcAccountManager_replyToInvitation");
        }
    }
    catch (OCException& e)
    {
        LOGE("%s", e.reason().c_str());
        ThrowOcException(e.code(), e.reason().c_str());
    }
}


