//
//  ZPTLocalNotification.cpp
//  AppGift
//
//  Created by tanjie on 15/11/5.
//
//

#include "ZPTLocalNotification.h"
#include <time.h>

using namespace std;
using namespace CrossApp;
using namespace CSJson;

const std::string ZPTLocalNotification::K_SINGLE_NOTI_COUNT = "k_single_noti_count";
const std::string ZPTLocalNotification::K_SINGLE_NOTI = "k_single_noti";
const std::string ZPTLocalNotification::K_REPEAT_NOTI = "k_repeat_noti";
ZPTLocalNotification *ZPTLocalNotification::_instance = NULL;

ZPTLocalNotification::~ZPTLocalNotification() {
    
}

/*
 alertTime: in seconds
 interval: in seconds, 0 means no repeat
 */
void ZPTLocalNotification::addNotification(uint32_t alertId, uint64_t alertTime, const std::string &alertTitle, const std::string &alertBody, IntervalType interval) {
    if (alertId == 0) {
        alertId = ++_singleNotiCount;
        interval = IntervalTypeNone;
    }
    
    Value notiConfig;
    notiConfig["alertId"] = alertId;
    notiConfig["alertTime"] = alertTime;
    notiConfig["alertTitle"] = alertTitle;
    notiConfig["alertBody"] = alertBody;
    notiConfig["interval"] = interval;
    
    const char *alertIdString = CCString::createWithFormat("%d", alertId)->getCString();
    
    if (interval == IntervalTypeNone) {
        _singleNoti[alertIdString] = notiConfig;
    } else {
        _repeatNoti[alertIdString] = notiConfig;
    }
    
    setNoti(alertId, alertTime, alertTitle, alertBody, interval);
    saveData();
}

void ZPTLocalNotification::removeNotification(uint32_t alertId) {
    const char *alertIdString = CCString::createWithFormat("%d", alertId)->getCString();
    
    Value noti;
    noti = _singleNoti[alertIdString];
    
    if (!noti.empty()) {
        cancelNoti(alertId);
        _singleNoti.removeMember(alertIdString);
        
    } else {
        noti = _repeatNoti[alertIdString];
        cancelNoti(alertId);
        _repeatNoti.removeMember(alertIdString);
    }
    
    saveData();
}

void ZPTLocalNotification::setAll() {
    loadData();
    
    Value::Members singleNotiKeys = _singleNoti.getMemberNames();
    time_t timeNow;
    timeNow = time(0);
    
    for (size_t i = 0; i < singleNotiKeys.size(); i++) {
        string key = singleNotiKeys[i];
        Value noti = _singleNoti[key];
        
        if (timeNow > noti["alertTime"].asUInt64()) {
            removeNotification(noti["alertId"].asUInt());
            continue;
        }
        
        setNoti(noti["alertId"].asUInt(),
                noti["alertTime"].asUInt64(),
                noti["alertTitle"].asString(),
                noti["alertBody"].asString(),
                IntervalTypeNone);
    }
    
    Value::Members repeateNotiKeys = _repeatNoti.getMemberNames();
    
    for (size_t i = 0; i < repeateNotiKeys.size(); i++) {
        string key = repeateNotiKeys[i];
        Value noti = _repeatNoti[key];
        
        setNoti(noti["alertId"].asUInt(),
                noti["alertTime"].asUInt64(),
                noti["alertTitle"].asString(),
                noti["alertBody"].asString(),
                (IntervalType)(noti["interval"].asUInt()));
    }
}

void ZPTLocalNotification::loadData() {
    Reader reader;
    CAUserDefault *userDefault = CAUserDefault::sharedUserDefault();
    _instance->_singleNotiCount = userDefault->getIntegerForKey(K_SINGLE_NOTI_COUNT.c_str());
    bool suc = reader.parse(userDefault->getStringForKey(K_REPEAT_NOTI.c_str()), _instance->_repeatNoti);
    suc = suc && reader.parse(userDefault->getStringForKey(K_SINGLE_NOTI.c_str()), _instance->_singleNoti);
    
    if (!suc) {
        _instance->_singleNoti.clear();
        _instance->_repeatNoti.clear();
        userDefault->setStringForKey(K_SINGLE_NOTI.c_str(), "");
        userDefault->setStringForKey(K_REPEAT_NOTI.c_str(), "");
        userDefault->flush();
    }
}

void ZPTLocalNotification::saveData() {
    CAUserDefault *userDefault = CAUserDefault::sharedUserDefault();
    userDefault->setIntegerForKey(K_SINGLE_NOTI_COUNT.c_str(), _singleNotiCount);
    userDefault->setStringForKey(K_SINGLE_NOTI.c_str(), _singleNoti.toStyledString());
    userDefault->setStringForKey(K_REPEAT_NOTI.c_str(), _repeatNoti.toStyledString());
    
    userDefault->flush();
}