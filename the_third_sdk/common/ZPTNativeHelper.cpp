//
//  ZPTNativeHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#include "ZPTNativeHelper.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "SDKCommon.h"

void ZPTNativeHelper::getDeviceInfo(CSJsonDictionary& dic)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "getDeviceInfo", "()Ljava/lang/String;")) {
        assert(0);
        return;
    }
    
    jstring returnString = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID);
    dic.initWithDescription(mi.env->GetStringUTFChars(returnString, NULL));
    
    mi.env->DeleteLocalRef(mi.classID);
}

void ZPTNativeHelper::openUrl(const char* url)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "openUrl", "(Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_url = mi.env->NewStringUTF(url);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_url);
    mi.env->DeleteLocalRef(mi.classID);
}

void ZPTNativeHelper::sendMail(const char *target, const char *title, CSJsonDictionary& extra)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "sendMail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_target = mi.env->NewStringUTF(target);
    jstring j_title = mi.env->NewStringUTF(title);
    jstring j_extra = mi.env->NewStringUTF(extra.getStyledDescription().c_str());
        
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_target, j_title, j_extra);
    mi.env->DeleteLocalRef(mi.classID);
}

string ZPTNativeHelper::getDeviceId()
{
    return "todo-android";
}

#endif
