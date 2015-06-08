//
//  UMHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#include "UMHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "SDKCommon.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

void UMHelper::startWithAppkey(const char* appkey, const char* channel)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "startWithAppkey", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_appkey = mi.env->NewStringUTF(appkey);
    jstring j_channel = mi.env->NewStringUTF(channel);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_appkey, j_channel);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onPageStart(const char* ViewID)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onPageStart", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_viewID = mi.env->NewStringUTF(ViewID);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_viewID);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onPageEnd(const char* ViewID)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onPageEnd", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_viewID = mi.env->NewStringUTF(ViewID);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_viewID);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEventBegin(const char* eventId)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEventBegin", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEventBeginValue(const char* eventId, const char* value)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEventBeginValue", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    jstring j_value = mi.env->NewStringUTF(value);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId, j_value);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEventEnd(const char* eventId)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEventEnd", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEventEndValue(const char* eventId, const char* value)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEventEndValue", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    jstring j_value = mi.env->NewStringUTF(value);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId, j_value);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEvent(const char* eventId)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEvent", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEventValue(const char* eventId, const char* value)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEventValue", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEventMap(const char* eventId, CSJsonDictionary& dic)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEventMap", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    jstring j_dic = mi.env->NewStringUTF(dic.getDescription().c_str());
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId);
    mi.env->DeleteLocalRef(mi.classID);
}

void UMHelper::onEventMapValue(const char* eventId, CSJsonDictionary& dic, int value)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "onEventMapValue", "(Ljava/lang/String;Ljava/lang/String;I)V")) {
        assert(0);
        return;
    }
    
    jstring j_eventId = mi.env->NewStringUTF(eventId);
    jstring j_dic = mi.env->NewStringUTF(dic.getDescription().c_str());
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_eventId, value);
    mi.env->DeleteLocalRef(mi.classID);
}

std::string UMHelper::getOnlineParam(const char* key)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "getOnlineParam", "(Ljava/lang/String;)Ljava/lang/String;")) {
        assert(0);
        return "";
    }
    
    jstring j_key = mi.env->NewStringUTF(key);
    
    jstring returnString = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID, j_key);
    
    std::string param(mi.env->GetStringUTFChars(returnString, NULL));
    mi.env->DeleteLocalRef(mi.classID);
    
    return param;
}

void UMHelper::showFeedback(const char* uId)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.UmengHelper", "showFeedback", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_uId = mi.env->NewStringUTF(uId);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_uId);
    mi.env->DeleteLocalRef(mi.classID);
}

#endif
