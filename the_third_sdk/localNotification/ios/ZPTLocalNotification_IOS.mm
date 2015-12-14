//
//  ZPTLocalNotification_IOS.mm
//  AppGift
//
//  Created by tanjie on 15/11/5.
//
//

#include "ZPTLocalNotification_IOS.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#include "SDKCommon.h"

using namespace std;

ZPTLocalNotification *ZPTLocalNotification::getInstance() {
    if (_instance == NULL) {
        _instance = new ZPTLocalNotification_IOS();
    }
    
    _instance->loadData();
    
    _instance->_intervalTypeMap.clear();
    _instance->_intervalTypeMap[IntervalTypeWeek] = NSCalendarUnitYearForWeekOfYear;
    _instance->_intervalTypeMap[IntervalTypeDay] = NSCalendarUnitDay;
    _instance->_intervalTypeMap[IntervalTypeHour] = NSCalendarUnitHour;
    _instance->_intervalTypeMap[IntervalTypeMinute] = NSCalendarUnitMinute;
    _instance->_intervalTypeMap[IntervalTypeSecond] = NSCalendarUnitSecond;
    _instance->_intervalTypeMap[IntervalTypeNone] = 0;
    
    return _instance;
}

void ZPTLocalNotification_IOS::requestPermission() {
    if ([UIApplication instancesRespondToSelector:@selector(registerUserNotificationSettings:)]) {
        [[UIApplication sharedApplication] registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert|UIUserNotificationTypeSound|UIUserNotificationTypeBadge categories:nil]];
    }
}

void ZPTLocalNotification_IOS::setNoti(uint32_t alertId, uint64_t alertTime, const std::string &alertTitle, const std::string &alertBody, IntervalType interval) {
    UILocalNotification *notification = [[UILocalNotification alloc] init];
    
    if (notification != nil) {
        notification.fireDate = [NSDate dateWithTimeIntervalSince1970: (NSTimeInterval)(alertTime)];
        notification.alertBody = nsstr(alertTitle.c_str());
        notification.repeatInterval = _intervalTypeMap[interval];
        notification.hasAction = YES;
        notification.applicationIconBadgeNumber = 1;
        notification.timeZone = [NSTimeZone defaultTimeZone];
        notification.userInfo = [NSDictionary dictionaryWithObject:[NSString stringWithFormat:@"%d", alertId] forKey:@"alertId"];
        
        NSLog(@"fireDate: %@", notification.fireDate);
        [[UIApplication sharedApplication] scheduleLocalNotification:notification];
        [notification release];
    }
}

void ZPTLocalNotification_IOS::cancelNoti(uint32_t alertId) {
    NSArray *notifications = [[UIApplication sharedApplication] scheduledLocalNotifications];
    
    for (UILocalNotification *notification in notifications ) {
        if([[notification.userInfo objectForKey:@"alertId"] integerValue] == alertId) {
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
        }
    }
}

void ZPTLocalNotification_IOS::cancelAll() {
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

#endif