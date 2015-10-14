//
//  AdHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/6/2.
//
//

#include "AdHelper.h"
//todo: test
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "SDKCommon.h"
#include "CrossAppExt.h"

USING_NS_CC;
using namespace CSJson;

std::map<std::string, AdHelper::AdType> AdHelper::AD_TYPE = AdHelper::createMap();
string AdHelper::K_NATIVEX_PLACEMENT_OFFER = "Store Open Offerwall";
string AdHelper::K_NATIVEX_PLACEMENT_VIDEO = "Game Launch Video";
Value AdHelper::_configCache;

//static const char *AdHelperPath = "com/zpt/utils/AdHelper";
//todo: init by config
static const char *AdHelperPath = "com/zpt/utils/ZPTNativeHelper";
static const char *PackageName = "com.zero.diaobaole";
static const char *ActivityName = "com.zero.diaobaole.MainActivity";
static const char *IntentName = "com.diao.diaobaole.OFFERWALL";

void AdHelper::initAdOnce(const CSJson::Value &params) {
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "callGoldMine", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_package = mi.env->NewStringUTF(PackageName);
    jstring j_activity = mi.env->NewStringUTF(ActivityName);
    jstring j_intent = mi.env->NewStringUTF(IntentName);
    
    Value data;
    data["function"] = "initAd";
    data["configs"] = params;
    // cache config
    _configCache = params;
    
    jstring j_data = mi.env->NewStringUTF(data.toStyledString().c_str());
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_package, j_activity, j_intent, j_data);
    mi.env->DeleteLocalRef(mi.classID);
}

void AdHelper::initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token) {
/*    
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "initAd", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_uId = mi.env->NewStringUTF(uId.c_str());
    jstring j_appkey = mi.env->NewStringUTF(appkey.c_str());
    jstring j_token = mi.env->NewStringUTF(token.c_str());
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type, j_uId, j_appkey, j_token);
    mi.env->DeleteLocalRef(mi.classID);
 */
 /*
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "callGoldMine", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    Value data;
    Value params;
    params["type"] = type;
    params["uId"] = uId;
    params["appkey"] = appkey;
    params["token"] = token;
    data["function"] = "initAd";
    data["params"] = params;
    
    jstring j_package = mi.env->NewStringUTF(PackageName);
    jstring j_activity = mi.env->NewStringUTF(ActivityName);
    jstring j_intent = mi.env->NewStringUTF(IntentName);
    jstring j_data = mi.env->NewStringUTF(data.toStyledString().c_str());
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_package, j_activity, j_intent, j_data);
    mi.env->DeleteLocalRef(mi.classID);
  */
}

void AdHelper::callOfferwall(AdType type) {
/*
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "callOfferwall", "(I)V")) {
        assert(0);
        return;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type);
    mi.env->DeleteLocalRef(mi.classID);
 */
    
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "callGoldMine", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_package = mi.env->NewStringUTF(PackageName);
    jstring j_activity = mi.env->NewStringUTF(ActivityName);
    jstring j_intent = mi.env->NewStringUTF(IntentName);
    
    Value data;
    Value params;
    params["adType"] = type;
    
    data["function"] = "callOfferwall";
    data["configs"] = _configCache;
    data["params"] = params;
    
    jstring j_data = mi.env->NewStringUTF(data.toStyledString().c_str());
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_package, j_activity, j_intent, j_data);
    mi.env->DeleteLocalRef(mi.classID);
}

void AdHelper::playVideo(AdType type) {
/*
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "playVideo", "(I)V")) {
        assert(0);
        return;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type);
    mi.env->DeleteLocalRef(mi.classID);
 */
}

//#endif