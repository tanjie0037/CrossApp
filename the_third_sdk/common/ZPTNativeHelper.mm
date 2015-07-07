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
    [device autorelease];
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    
//    dic.insertItem("name", [device.name UTF8String]);
    dic.insertItem("model", [device.model UTF8String]);
//    dic.insertItem("localizedModel", [device.localizedModel UTF8String]);
    dic.insertItem("systemName", [device.systemName UTF8String]);
    dic.insertItem("systemVersion", [device.systemVersion UTF8String]);
    dic.insertItem("appVersion", utf8cstr([infoDictionary objectForKey:@"CFBundleShortVersionString"]));
}

string ZPTNativeHelper::getAppVersion(bool replaceDot)
{
    UIDevice *device=[[UIDevice alloc] init];
    [device autorelease];
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    
    string appversion = string(utf8cstr([infoDictionary objectForKey:@"CFBundleShortVersionString"]));
    
    if (replaceDot) {
        char *outStr = str_replace(appversion.c_str(), ".", "_");
        appversion = string(outStr);
        delete[] (outStr);
    }
    
    return appversion;
}

void ZPTNativeHelper::openUrl(const char* url)
{
    [[UIApplication sharedApplication] openURL: [NSURL URLWithString:nsstr(url)]];
}


void ZPTNativeHelper::sendMail(const string &target, const string &title, CSJsonDictionary& extra, const string &format)
{
    NSMutableString *mailUrl = [[[NSMutableString alloc]init]autorelease];
    //添加收件人
    [mailUrl appendFormat:@"mailto:%@", nsstr(target.c_str())];
//    //添加抄送
//    NSArray *ccRecipients = [NSArray arrayWithObjects:@"second@example.com", @"third@example.com", nil];
//    [mailUrl appendFormat:@"?cc=%@", [ccRecipients componentsJoinedByString:@","]];
//    //添加密送
//    NSArray *bccRecipients = [NSArray arrayWithObjects:@"fourth@example.com", nil];
//    [mailUrl appendFormat:@"&bcc=%@", [bccRecipients componentsJoinedByString:@","]];
    //添加主题
    [mailUrl appendFormat:@"?subject=%@", nsstr(title.c_str())];
    //添加邮件内容
    if (!extra.isEmpty()) {
        [mailUrl appendFormat:@"&body=%@", nsstr(("\n\n\n\n" + extra.getStyledDescription() + format).c_str()) ];
    }
    
    NSString* email = [mailUrl stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL: [NSURL URLWithString:email]];
}

string ZPTNativeHelper::getDeviceId()
{
    return string(utf8cstr([[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString]));
}

string ZPTNativeHelper::getStatusKey()
{
    return "status-v" + ZPTNativeHelper::getAppVersion(true);
}

string ZPTNativeHelper::getLanguage()
{
    // get the current language and country config
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *currentLanguage = [languages objectAtIndex:0];
    
    // get the current language code.(such as English is "en", Chinese is "zh" and so on)
    NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
    NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
    
    string ret = "";
    
    if ([languageCode isEqualToString:@"zh"]) {
        ret = string(utf8cstr(currentLanguage));
    } else {
        ret = string(utf8cstr(languageCode));
    }
    
    return ret;
}

#endif