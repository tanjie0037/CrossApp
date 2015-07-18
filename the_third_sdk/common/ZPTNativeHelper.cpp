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

void ZPTNativeHelper::sendMail(const string &target, const string &title, CSJsonDictionary &extra, const string &format)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "sendMail", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_target = mi.env->NewStringUTF(target.c_str());
    jstring j_title = mi.env->NewStringUTF(title.c_str());
    jstring j_extra = mi.env->NewStringUTF(("\n\n\n\n" + extra.getStyledDescription() + format).c_str());
        
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_target, j_title, j_extra);
    mi.env->DeleteLocalRef(mi.classID);
}

string ZPTNativeHelper::getDeviceId()
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "getDeviceId", "()Ljava/lang/String;")) {
        assert(0);
        return "";
    }
    
    jstring returnString = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID);
    string deviceId = string(mi.env->GetStringUTFChars(returnString, NULL));
    mi.env->DeleteLocalRef(mi.classID);
                             
    return deviceId;
}

string ZPTNativeHelper::getAppVersion(bool replaceDot)
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "getAppVersion", "()Ljava/lang/String;")) {
        assert(0);
        return "";
    }
    
    jstring returnString = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID);
    string appversion = string(mi.env->GetStringUTFChars(returnString, NULL));
    mi.env->DeleteLocalRef(mi.classID);
    
    if (replaceDot) {
        char *outStr = str_replace(appversion.c_str(), ".", "_");
        appversion = string(outStr);
        delete[] (outStr);
    }
    
    return appversion;
}

int ZPTNativeHelper::getAppBuild()
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "getAppBuild", "()I")) {
        assert(0);
        return 0;
    }
    
    int appbuild = (int)(mi.env->CallStaticObjectMethod(mi.classID, mi.methodID));
    return appbuild;
}

string ZPTNativeHelper::getStatusKey()
{
    return "status-v" + ZPTNativeHelper::getAppVersion(true);
}

string ZPTNativeHelper::getLanguage()
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "getLanguage", "()Ljava/lang/String;")) {
        assert(0);
        return "";
    }
    
    jstring returnString = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID);
    string language = string(mi.env->GetStringUTFChars(returnString, NULL));
    mi.env->DeleteLocalRef(mi.classID);
    
    return language;
}

string ZPTNativeHelper::getCountyCode()
{
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.ZPTNativeHelper", "getCountyCode", "()Ljava/lang/String;")) {
        assert(0);
        return "";
    }
    
    jstring returnString = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID);
    string countyCode = string(mi.env->GetStringUTFChars(returnString, NULL));
    mi.env->DeleteLocalRef(mi.classID);
    
    CCLOG("---CountyCode:%s", countyCode.c_str());
    
    return countyCode;
}

#endif