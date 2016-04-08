//
//  ZPTLocalNotification.h
//  AppGift
//
//  Created by tanjie on 15/11/5.
//
//

#ifndef ZPTLocalNotification_h
#define ZPTLocalNotification_h

#include <stdio.h>
#include <string>
#include "CrossApp.h"
#include "CrossAppExt.h"

#define REPEAT_ID_START 1000000

//enum IntervalType {
//    IntervalTypeNone = 0,
//    IntervalTypeSecond,
//    IntervalTypeMinute,
//    IntervalTypeHour,
//    IntervalTypeDay,
//    IntervalTypeWeek
//};
typedef std::string IntervalType;

class ZPTLocalNotification {
public:
    std::map<IntervalType, uint64_t> _intervalTypeMap;
    
    ~ZPTLocalNotification();
    static ZPTLocalNotification *getInstance();
    /*
     alertTime: in seconds
     interval: in seconds, 0 means no repeat
     */
    void addNotification(uint32_t alertId, uint64_t alertTime, const std::string &alertTitle, const std::string &alertBody, IntervalType interval);
    void removeNotification(uint32_t alertId);
    void setAll();
    virtual void cancelAll() {}
    virtual void requestPermission() {}
    
protected:
    static ZPTLocalNotification *_instance;
    CSJson::Value _repeatNoti;
    CSJson::Value _singleNoti;
    uint32_t _singleNotiCount;
    static const std::string K_SINGLE_NOTI_COUNT;
    static const std::string K_SINGLE_NOTI;
    static const std::string K_REPEAT_NOTI;
    static const IntervalType IntervalTypeNone;
    static const IntervalType IntervalTypeSecond;
    static const IntervalType IntervalTypeMinute;
    static const IntervalType IntervalTypeHour;
    static const IntervalType IntervalTypeDay;
    static const IntervalType IntervalTypeWeek;
    
    ZPTLocalNotification()
    {}
    virtual void setNoti(uint32_t alertId, uint64_t alertTime, const std::string &alertTitle, const std::string &alertBody, IntervalType interval) {}
    virtual void cancelNoti(uint32_t alertId) {}
    void loadData();
    void saveData();
};

#endif /* ZPTLocalNotification_h */