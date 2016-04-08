//
//  JPushHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/7/29.
//
//

#include "JPushHelper.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <string>
#include "SDKCommon.h"

static const char *JPushHelperPath = "com/zpt/libjpush/JPushHelper";

void JPushHelper::initJPush(const std::string &uId, const std::set<std::string>& tags) {
    JniMethodInfo mi;
    
    JniHelper::getMethodInfo(mi
                             , "java/util/HashSet"
                             , "<init>"
                             , "(I)V");
    
    jobject javaSet = mi.env->NewObject(mi.classID, mi.methodID, tags.size());
    
    JniHelper::getMethodInfo(mi
                             , "java/util/HashSet"
                             , "add"
                             , "(Ljava/lang/Object;)Z");

    
    for (std::set<std::string>::iterator it = tags.begin(); it != tags.end(); it++) {
        std::string tag = *it;
        mi.env->CallBooleanMethod(javaSet, mi.methodID, mi.env->NewStringUTF(tag.c_str()));
    }
    
    if (!JniHelper::getStaticMethodInfo(mi, JPushHelperPath, "initUser", "(Ljava/lang/String;Ljava/util/HashSet;)V")) {
        assert(0);
        return;
    }
    
    jstring juId = mi.env->NewStringUTF(str_replace(uId.c_str(), "-", ""));
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, juId, javaSet);
    mi.env->DeleteLocalRef(mi.classID);
}

void JPushHelper::initIOS(){}

#endif