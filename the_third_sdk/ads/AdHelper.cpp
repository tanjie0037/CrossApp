//
//  AdHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/6/2.
//
//

#include "AdHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "SDKCommon.h"

USING_NS_CC;

std::map<std::string, AdHelper::AdType> AdHelper::AD_TYPE = AdHelper::createMap();

void AdHelper::initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token) {
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.AdHelper", "initAd", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_uId = mi.env->NewStringUTF(uId.c_str());
    jstring j_appkey = mi.env->NewStringUTF(appkey.c_str());
    jstring j_token = mi.env->NewStringUTF(token.c_str());
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type, j_uId, j_appkey, j_token);
    mi.env->DeleteLocalRef(mi.classID);
}

void AdHelper::callOfferwall(AdType type) {
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.AdHelper", "callOfferwall", "(I)V")) {
        assert(0);
        return;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type);
    mi.env->DeleteLocalRef(mi.classID);
}

void AdHelper::playVideo(AdType type) {
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, "com.zpt.utils.AdHelper", "playVideo", "(I)V")) {
        assert(0);
        return;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type);
    mi.env->DeleteLocalRef(mi.classID);
}

#endif