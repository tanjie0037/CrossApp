//
//  JPushHelper.m
//  AppGift
//
//  Created by tanjie on 15/7/29.
//
//

#include "JPushHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "SDKCommon.h"
#import <Foundation/Foundation.h>
#import "AppController.h"
#import "APService.h"

#define K_REQUESTED_NOTI_IOS @"request_notification"

string JPushHelper::NOTI_PATH_SUFFIX = "ca_remote_notis.json";
string JPushHelper::NOTI_INDEX_FLAG_PATH = "ca_remote_noti_index.json";
string JPushHelper::NOTI_KEY_NOTI = "notis";
string JPushHelper::NOTI_KEY_INDEX = "index";
string JPushHelper::NOTI_KEY_CONTENT = "content";

using namespace CSJson;

void JPushHelper::initJPush(const std::string &uId, const std::set<std::string>& tags) {
    NSSet *ntags = [NSSet set];
    
    for (std::set<std::string>::iterator it = tags.begin(); it != tags.end(); it++) {
        std::string tag = *it;
        [ntags setByAddingObject:nsstr(tag.c_str())];
    }
    
    [APService setTags:ntags alias:nsstr(str_replace(uId.c_str(), "-", "")) callbackSelector:nil object:nil];
}

void JPushHelper::initIOS() {
    NSUserDefaults *userDefaultes = [NSUserDefaults standardUserDefaults];
    
    if (![userDefaultes boolForKey:K_REQUESTED_NOTI_IOS]) {
        [userDefaultes setBool:YES forKey:K_REQUESTED_NOTI_IOS];
        [userDefaultes synchronize];
    }
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_7_1
    if ([[UIDevice currentDevice].systemVersion floatValue] >= 8.0) {
        //可以添加自定义categories
        [APService registerForRemoteNotificationTypes:(UIUserNotificationTypeBadge |
                                                       UIUserNotificationTypeSound |
                                                       UIUserNotificationTypeAlert)
                                           categories:nil];
    } else {
        //categories 必须为nil
        [APService registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
                                                       UIRemoteNotificationTypeSound |
                                                       UIRemoteNotificationTypeAlert)
                                           categories:nil];
    }
#else
    //categories 必须为nil
    [APService registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
                                                   UIRemoteNotificationTypeSound |
                                                   UIRemoteNotificationTypeAlert)
                                       categories:nil];
#endif
    
}

bool JPushHelper::requestedNotiIOS() {
    NSUserDefaults *userDefaultes = [NSUserDefaults standardUserDefaults];
    return (bool)[userDefaultes boolForKey:K_REQUESTED_NOTI_IOS];
}

void JPushHelper::checkNewNotis() {
    //iOS dont use it now.    
}

#endif
