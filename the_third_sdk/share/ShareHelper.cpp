//
//  ShareHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/5/28.
//
//
#include "ShareHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "SDKCommon.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;
using namespace cn::sharesdk;

#ifdef __cplusplus
extern "C" {
#endif
    
    C2DXAuthResultEvent authCb = NULL;
    C2DXGetUserInfoResultEvent infoCb = NULL;
    C2DXShareResultEvent shareCb = NULL;
    
    JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onJavaCallback
    (JNIEnv * env, jclass thiz, jstring resp) {
        
        CSJsonDictionary jsonDict, resDict, emptyDic;
        const char* ccResp = env->GetStringUTFChars(resp, JNI_FALSE);
        jsonDict.initWithDescription(ccResp);
        env->ReleaseStringUTFChars(resp, ccResp);
        
        int status = jsonDict.getItemIntValue("status", -1);
        int action = jsonDict.getItemIntValue("action", -1);
        int platform = jsonDict.getItemIntValue("platform", -1);
        jsonDict.getSubDictionary("res", resDict);
        
        if (status == -1 || action == -1 || platform == -1) {
            assert(0);
            return;
        }
        
        if (action == 1 && NULL != authCb) { // 1 = ACTION_AUTHORIZING
            authCb((C2DXResponseState)status, (C2DXPlatType) platform, emptyDic);
        } else if (action == 8 && NULL != infoCb) { // 8 = ACTION_USER_INFOR
            infoCb((C2DXResponseState)status, (C2DXPlatType) platform, resDict, emptyDic);
        } else if (action == 9 && NULL != shareCb) { // 9 = ACTION_SHARE
            shareCb((C2DXResponseState)status, (C2DXPlatType) platform, resDict, emptyDic);
        }
    }
    
#ifdef __cplusplus
}
#endif

bool getMethod(JniMethodInfo &mi, const char *methodName, const char *paramCode) {
    return JniHelper::getStaticMethodInfo(mi, "cn/sharesdk/ShareSDKUtils", methodName, paramCode);
}

void releaseMethod(JniMethodInfo &mi) {
    mi.env->DeleteLocalRef(mi.classID);
}

void ShareHelper::open(const std::string& appKey, bool useAppTrusteeship) {
    JniMethodInfo mi;
    bool isHave = getMethod(mi, "initSDK", "(Ljava/lang/String;Z)V");
    if (!isHave) {
        assert(0);
        return;
    }
    
    jstring appKeyStr = mi.env->NewStringUTF(appKey.c_str());
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, appKeyStr, useAppTrusteeship);
    releaseMethod(mi);
}

bool ShareHelper::close() {
    JniMethodInfo mi;
    bool isHave = getMethod(mi, "stopSDK", "()V");
    if (!isHave) {
        assert(0);
        return false;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
    releaseMethod(mi);
    return true;
}

void ShareHelper::setPlatformConfig(C2DXPlatType platType, CSJsonDictionary& configInfo) {
    
    // This is not a necessary method for Android, you can setup your platform configs more efficiently in "assets/ShareSDK.xml"
    return;
    
//    JniMethodInfo mi;
//    bool isHave = getMethod(mi, "setPlatformConfig", "(ILjava/lang/String;)V");
//    if (!isHave) {
//        assert(0);
//        return;
//    }
//    
//    jstring jInfo = mi.env->NewStringUTF(configInfo.getDescription().c_str());
//    
//    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platType, jInfo);
//    releaseMethod(mi);
}

void ShareHelper::authorize(C2DXPlatType platType, C2DXAuthResultEvent callback) {
    JniMethodInfo mi;
    bool isHave = getMethod(mi, "authorize", "(I)V");
    if (!isHave) {
        assert(0);
        return;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platType);
    releaseMethod(mi);
    authCb = callback;
}

void ShareHelper::cancelAuthorize(C2DXPlatType platType) {
    JniMethodInfo mi;
    bool isHave = getMethod(mi, "removeAccount", "(I)V");
    if (!isHave) {
        assert(0);
        return;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platType);
    releaseMethod(mi);
}

bool ShareHelper::hasAutorized(C2DXPlatType platType) {
    JniMethodInfo mi;
    bool isHave = getMethod(mi, "isValid", "(I)Z");
    if (!isHave) {
        assert(0);
        return false;
    }
    
    jboolean valid = mi.env->CallStaticBooleanMethod(mi.classID, mi.methodID, platType);
    releaseMethod(mi);
    return valid == JNI_TRUE;
}

void ShareHelper::getUserInfo(C2DXPlatType platType, C2DXGetUserInfoResultEvent callback) {
    //todo
}

void ShareHelper::shareContent(cn::sharesdk::C2DXPlatType platType, CSJsonDictionary &content, cn::sharesdk::C2DXShareResultEvent callback) {
    
    JniMethodInfo mi;
    bool isHave = getMethod(mi, "share", "(ILjava/lang/String;)V");
    if (!isHave) {
        assert(0);
        return;
    }
    
    jstring jContent = mi.env->NewStringUTF(content.getDescription().c_str());
    // free(ccContent);
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platType, jContent);
    releaseMethod(mi);
    shareCb = callback;
}

void ShareHelper::oneKeyShareContent(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback) {
    //todo
}

void ShareHelper::showShareMenu(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback) {
    //todo
}

void ShareHelper::showShareMenu(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, CrossApp::CCPoint pt, cn::sharesdk::C2DXMenuArrowDirection direction, cn::sharesdk::C2DXShareResultEvent callback) {
    //todo
}

void ShareHelper::showShareView(cn::sharesdk::C2DXPlatType platType, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback) {
    //todo
}

//bool ShareHelper::isClientInstalled(cn::sharesdk::C2DXPlatType platType) {
//    //todo
//    return true;
//}


#endif