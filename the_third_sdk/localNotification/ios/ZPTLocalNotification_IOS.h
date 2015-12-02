//
//  ZPTLocalNotification_IOS.h
//  AppGift
//
//  Created by tanjie on 15/11/11.
//
//

#ifndef ZPTLocalNotification_IOS_h
#define ZPTLocalNotification_IOS_h

#include "ZPTLocalNotification.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

class ZPTLocalNotification_IOS : public ZPTLocalNotification {
    void setNoti(uint32_t alertId, uint64_t alertTime, const std::string &alertTitle, const std::string &alertBody, IntervalType interval);
    void cancelNoti(uint32_t alertId);
    void cancelAll();
    void requestPermission();
};

#endif
#endif /* ZPTLocalNotification_IOS_h */