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
using namespace CSJson;

std::map<std::string, AdHelper::AdType> AdHelper::AD_TYPE = AdHelper::createMap();
string AdHelper::K_NATIVEX_PLACEMENT_OFFER = "Store Open Offerwall";
string AdHelper::K_NATIVEX_PLACEMENT_VIDEO = "Game Launch Video";
string AdHelper::PLUGIN_PACKAGE = "com.zero.diaobaole";
string AdHelper::PLUGIN_ACTIVITY = "com.zero.diaobaole.MainActivity";
string AdHelper::PLUGIN_INTENT = "com.diao.diaobaole.OFFERWALL";
string AdHelper::PLUGIN_APK = "goldmine.apk";

Value AdHelper::_configCache;

static const char *AdHelperPath = "com/zpt/utils/AdHelper";
static const char *ZPTNativeHelperPath = "com/zpt/utils/ZPTNativeHelper";
bool AdHelper::_debug = false;

void AdHelper::setDebug(bool debug) {
    AdHelper::_debug = debug;
}

void AdHelper::initAdInGoldMine(const CSJson::Value &params) {
    JniMethodInfo mi;
    if (!JniHelper::getStaticMethodInfo(mi, ZPTNativeHelperPath, "callGoldMine", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        assert(0);
        return;
    }
    
    jstring j_package = mi.env->NewStringUTF(PLUGIN_PACKAGE.c_str());
    jstring j_activity = mi.env->NewStringUTF(PLUGIN_ACTIVITY.c_str());
    jstring j_intent = mi.env->NewStringUTF(PLUGIN_INTENT.c_str());
    
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
}

void AdHelper::callOfferwall(AdType type, bool inGoldMine) {
    if (inGoldMine) {
        JniMethodInfo mi;
        if (!JniHelper::getStaticMethodInfo(mi, ZPTNativeHelperPath, "callGoldMine", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
            assert(0);
            return;
        }
        
        jstring j_package = mi.env->NewStringUTF(PLUGIN_PACKAGE.c_str());
        jstring j_activity = mi.env->NewStringUTF(PLUGIN_ACTIVITY.c_str());
        jstring j_intent = mi.env->NewStringUTF(PLUGIN_INTENT.c_str());
        
        Value data;
        Value params;
        params["adType"] = type;
        
        data["function"] = "callOfferwall";
        data["configs"] = _configCache;
        data["params"] = params;
        
        jstring j_data = mi.env->NewStringUTF(data.toStyledString().c_str());
        
        mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_package, j_activity, j_intent, j_data);
        mi.env->DeleteLocalRef(mi.classID);
        
    } else {
        JniMethodInfo mi;
        if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "callOfferwall", "(I)V")) {
            assert(0);
            return;
        }
        
        mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type);
        mi.env->DeleteLocalRef(mi.classID);
    }
}

void AdHelper::playVideo(AdType type, bool inGoldMine) {
    if (inGoldMine) {
        JniMethodInfo mi;
        if (!JniHelper::getStaticMethodInfo(mi, ZPTNativeHelperPath, "callGoldMine", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
            assert(0);
            return;
        }
        
        jstring j_package = mi.env->NewStringUTF(PLUGIN_PACKAGE.c_str());
        jstring j_activity = mi.env->NewStringUTF(PLUGIN_ACTIVITY.c_str());
        jstring j_intent = mi.env->NewStringUTF(PLUGIN_INTENT.c_str());
        
        Value data;
        Value params;
        params["adType"] = type;
        
        data["function"] = "playVideo";
        data["configs"] = _configCache;
        data["params"] = params;
        
        jstring j_data = mi.env->NewStringUTF(data.toStyledString().c_str());
        
        mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_package, j_activity, j_intent, j_data);
        mi.env->DeleteLocalRef(mi.classID);
        
    } else {
        JniMethodInfo mi;
        if (!JniHelper::getStaticMethodInfo(mi, AdHelperPath, "playVideo", "(I)V")) {
            assert(0);
            return;
        }
        
        mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, (int)type);
        mi.env->DeleteLocalRef(mi.classID);
    }
}

#endif