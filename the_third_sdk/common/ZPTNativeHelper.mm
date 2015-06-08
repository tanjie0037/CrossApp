//
//  ZPTNativeHelper.mm
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#include "ZPTNativeHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "SDKCommon.h"
#import <Foundation/Foundation.h>
#import "AppController.h"
#import <AdSupport/ASIdentifierManager.h>

void ZPTNativeHelper::getDeviceInfo(CSJsonDictionary& dic)
{
    UIDevice *device=[[UIDevice alloc] init];
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    
//    dic.insertItem("name", [device.name UTF8String]);
    dic.insertItem("model", [device.model UTF8String]);
//    dic.insertItem("localizedModel", [device.localizedModel UTF8String]);
    dic.insertItem("systemName", [device.systemName UTF8String]);
    dic.insertItem("systemVersion", [device.systemVersion UTF8String]);
    dic.insertItem("appVersion", utf8cstr([infoDictionary objectForKey:@"CFBundleShortVersionString"]));
    [device dealloc];
}

void ZPTNativeHelper::openUrl(const char* url)
{
    [[UIApplication sharedApplication] openURL: [NSURL URLWithString:nsstr(url)]];
}


void ZPTNativeHelper::sendMail(const char *target, const char *title, CSJsonDictionary& extra)
{
    NSMutableString *mailUrl = [[[NSMutableString alloc]init]autorelease];
    //添加收件人
    [mailUrl appendFormat:@"mailto:%@", nsstr(target)];
//    //添加抄送
//    NSArray *ccRecipients = [NSArray arrayWithObjects:@"second@example.com", @"third@example.com", nil];
//    [mailUrl appendFormat:@"?cc=%@", [ccRecipients componentsJoinedByString:@","]];
//    //添加密送
//    NSArray *bccRecipients = [NSArray arrayWithObjects:@"fourth@example.com", nil];
//    [mailUrl appendFormat:@"&bcc=%@", [bccRecipients componentsJoinedByString:@","]];
    //添加主题
    [mailUrl appendFormat:@"?subject=%@", nsstr(title)];
    //添加邮件内容
    if (!extra.isEmpty()) {
        [mailUrl appendFormat:@"&body=%@", nsstr(extra.getStyledDescription().c_str())];
    }
    
    NSString* email = [mailUrl stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL: [NSURL URLWithString:email]];
}

string ZPTNativeHelper::getDeviceId()
{
    return string(utf8cstr([[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString]));
}

#endif