//
//  UMHelper.mm
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#include "UMHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "MobClick.h"
#include "SDKCommon.h"
#import <Foundation/Foundation.h>
#import "AppController.h"
#import "UMFeedback.h"


void UMHelper::startWithAppkey(const char* appkey, const char* channel)
{
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    [MobClick setAppVersion:version];
    [MobClick startWithAppkey:nsstr(appkey) reportPolicy:BATCH channelId:nsstr(channel)];
    [MobClick updateOnlineConfig];
    [MobClick getConfigParams];
    [UMFeedback setAppkey:nsstr(appkey)];
    [MobClick setCrashReportEnabled:YES];
}

void UMHelper::onPageStart(const char* ViewID)
{
    [MobClick beginLogPageView:nsstr(ViewID)];
}

void UMHelper::onPageEnd(const char* ViewID)
{
    [MobClick endLogPageView:nsstr(ViewID)];
}

void UMHelper::onEventBegin(const char* eventId)
{
    [MobClick beginEvent:nsstr(eventId)];
}

void UMHelper::onEventBeginValue(const char* eventId, const char* value)
{
    [MobClick beginEvent:nsstr(eventId) label:nsstr(value)];
}

void UMHelper::onEventEnd(const char* eventId)
{
    [MobClick endEvent:nsstr(eventId)];
}

void UMHelper::onEventEndValue(const char* eventId, const char* value)
{
    [MobClick endEvent:nsstr(eventId) label:nsstr(value)];
}

void UMHelper::onEvent(const char* eventId)
{
    [MobClick event:nsstr(eventId)];
}

void UMHelper::onEventValue(const char* eventId, const char* value)
{
    [MobClick event:nsstr(eventId) label:nsstr(value)];
}

void UMHelper::onEventMap(const char* eventId, CSJsonDictionary& dic)
{
    string jsonDesc = dic.getDescription();
    NSDictionary* nsDic = dictionaryWithJsonString(nsstr(jsonDesc.c_str()));
    [MobClick event:nsstr(eventId) attributes: nsDic];
}

void UMHelper::onEventMapValue(const char* eventId, CSJsonDictionary& dic, int value)
{
    string jsonDesc = dic.getDescription();
    NSDictionary* nsDic = dictionaryWithJsonString(nsstr(jsonDesc.c_str()));
    [MobClick event:nsstr(eventId) attributes:nsDic counter:value];
}

std::string UMHelper::getOnlineParam(const char* key)
{
    return utf8cstr([MobClick getConfigParams:nsstr(key)]);
}

void UMHelper::showFeedback(const char* uId)
{
    [[AppController getRootView] presentModalViewController:[UMFeedback feedbackModalViewController]
                            animated:YES];
    NSDictionary* dic = [NSDictionary dictionaryWithObjectsAndKeys:nsstr(uId), @"uId", nil];
    [[UMFeedback sharedInstance] setRemarkInfo:dic];
}

#endif