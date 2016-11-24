//
//  JPushHelper.h
//  AppGift
//
//  Created by tanjie on 15/7/29.
//
//

#ifndef __AppGift__JPushHelper__
#define __AppGift__JPushHelper__

#include <stdio.h>
#include <string>
#include <set>
#include "CrossApp.h"

class JPushHelper
{
    /*
     noti data type:
     {
        "notis": [
            {
                index: 1,
                content: {}
            },
            {
                index: 2,
                content: {}
            }
        ]
     }
     */
public:
    static std::string NOTI_PATH_SUFFIX;
    static std::string NOTI_INDEX_FLAG_PATH;
    static std::string NOTI_KEY_NOTI;
    static std::string NOTI_KEY_INDEX;
    static std::string NOTI_KEY_CONTENT;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static bool requestedNotiIOS();
    static void initIOS();
#endif
    
    static void initJPush(const std::string &uId, const std::set<std::string>& tags);
    static void checkNewNotis();
};

#endif /* defined(__AppGift__JPushHelper__) */
