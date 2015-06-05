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
#include <android/log.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

#ifndef LOG_TAG
#define  LOG_TAG    "jni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#endif

void UMHelper::startWithAppkey(const char* appkey, const char* channel)
{
    
}

void UMHelper::onPageStart(const char* ViewID)
{
    
}

void UMHelper::onPageEnd(const char* ViewID)
{}

void UMHelper::onEventBegin(const char* eventId)
{}

void UMHelper::onEventBeginValue(const char* eventId, const char* value)
{}

void UMHelper::onEventEnd(const char* eventId)
{}

void UMHelper::onEventEndValue(const char* eventId, const char* value)
{}

void UMHelper::onEvent(const char* eventId)
{}

void UMHelper::onEventValue(const char* eventId, const char* value)
{}

void UMHelper::onEventMap(const char* eventId, CSJsonDictionary& dic)
{}

void UMHelper::onEventMapValue(const char* eventId, CSJsonDictionary& dic, int value)
{}

std::string UMHelper::getOnlineParam(const char* key)
{}

void UMHelper::showFeedback(const char* uId)
{}


#endif