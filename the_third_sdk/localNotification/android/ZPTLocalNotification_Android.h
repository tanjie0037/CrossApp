//
//  ZPTLocalNotification_Android.h
//  AppGift
//
//  Created by tanjie on 15/11/11.
//
//

#ifndef ZPTLocalNotification_Android_h
#define ZPTLocalNotification_Android_h

#include "ZPTLocalNotification.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

class ZPTLocalNotification_Android : public ZPTLocalNotification {
    void setNoti(uint32_t alertId, uint64_t alertTime, const std::string &alertTitle, const std::string &alertBody, IntervalType interval);
    void cancelNoti(uint32_t alertId);
    void cancelAll();
};

#endif
#endif /* ZPTLocalNotification_Android_h */
