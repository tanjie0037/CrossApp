//
//  UMHelper.h
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#ifndef __AppGift__UMHelper__
#define __AppGift__UMHelper__

#include <stdlib.h>
#include <iostream>
#include "CrossApp.h"
#include "CrossAppExt.h"

class UMHelper {
public:
    static void startWithAppkey(const char* appkey, const char* channel);
    
    static void onPageStart(const char* ViewID);
    
    static void onPageEnd(const char* ViewID);
    
    static void onEventBegin(const char* eventId);
    
    static void onEventBeginValue(const char* eventId, const char* value);
    
    static void onEventEnd(const char* eventId);
    
    static void onEventEndValue(const char* eventId, const char* value);
    
    static void onEvent(const char* eventId);
    
    static void onEventValue(const char* eventId, const char* value);
    
    static void onEventMap(const char* eventId, CSJsonDictionary& dic);
    
    static void onEventMapValue(const char* eventId, CSJsonDictionary& dic, int value);
    
    static std::string getOnlineParam(const char* key);
    
    static void showFeedback(const char* uId);
};

#endif /* defined(__AppGift__UMHelper__) */