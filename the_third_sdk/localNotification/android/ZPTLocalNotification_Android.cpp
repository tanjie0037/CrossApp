//
//  ZPTLocalNotification_Android.cpp
//  AppGift
//
//  Created by tanjie on 15/11/5.
//
//

#include <stdio.h>
#include "ZPTLocalNotification_Android.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>

using namespace std;
using namespace CSJson;

ZPTLocalNotification *ZPTLocalNotification::getInstance() {
    if (_instance == NULL) {
        _instance = new ZPTLocalNotification_Android();
    }
    
    _instance->loadData();
    
    _instance->_intervalTypeMap.clear();
    _instance->_intervalTypeMap[IntervalTypeWeek] = 7 * 24 * 3600;
    _instance->_intervalTypeMap[IntervalTypeDay] = 24 * 3600;
    _instance->_intervalTypeMap[IntervalTypeHour] = 3600;
    _instance->_intervalTypeMap[IntervalTypeMinute] = 60;
    _instance->_intervalTypeMap[IntervalTypeSecond] = 1;
    _instance->_intervalTypeMap[IntervalTypeNone] = 0;

    return _instance;
}


void ZPTLocalNotification_Android::setNoti(uint32_t alertId, uint64_t alertTime, const std::string &alertTitle, const std::string &alertBody, IntervalType interval) {
    JniMethodInfo mi;
    //String alertTitle, String alertBody, long alertTime, int requestCode,long repeatInterval
    if (!JniHelper::getStaticMethodInfo(mi, "com/zpt/utils/NotificationHelper", "createLocalNotification", "(Ljava/lang/String;Ljava/lang/String;JIJ)V")) {
        assert(0);
        return;
    }
    
    jstring j_alertTitle = mi.env->NewStringUTF(alertTitle.c_str());
    jstring j_alertBody = mi.env->NewStringUTF(alertBody.c_str());
        
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, j_alertTitle, j_alertBody, alertTime, alertId, _intervalTypeMap[interval]);
    mi.env->DeleteLocalRef(mi.classID);
}

void ZPTLocalNotification_Android::cancelNoti(uint32_t alertId) {
    JniMethodInfo mi;
    
    if (!JniHelper::getStaticMethodInfo(mi, "com/zpt/utils/NotificationHelper", "cancelLocalNotification", "(I)V")) {
        assert(0);
        return;
    }
    
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, alertId);
    mi.env->DeleteLocalRef(mi.classID);

}

void ZPTLocalNotification_Android::cancelAll() {
    Value::Members singleNotiKeys = _singleNoti.getMemberNames();
    
    for (size_t i = 0; i < singleNotiKeys.size(); i++) {
        string key = singleNotiKeys[i];
        uint32_t notiId = atoi(key.c_str());
        cancelNoti(notiId);
    }
    
    Value::Members repeateNotiKeys = _repeatNoti.getMemberNames();
    
    for (size_t i = 0; i < repeateNotiKeys.size(); i++) {
        string key = repeateNotiKeys[i];
        uint32_t notiId = atoi(key.c_str());
        cancelNoti(notiId);
    }
}

#endif