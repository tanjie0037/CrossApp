//
//  ZPTNativeHelper.h
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#ifndef __AppGift__ZPTNativeHelper__
#define __AppGift__ZPTNativeHelper__

#include <stdio.h>
#include "CrossApp.h"
#include "CrossAppExt.h"

class ZPTNativeHelper
{
public:
    static void getDeviceInfo(CSJsonDictionary& dic);
    static void getPrivateInfo(CSJsonDictionary& dic);
    static void openUrl(const char* url);
    static void sendMail(const string &target, const string &title, CSJsonDictionary& extra, const string &format);
    static string getDeviceId(bool simple = false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static string getIdfa();
#endif
    static string getAppVersion(bool replaceDot = false);
    static int getAppBuild();
    static string getStatusKey();
    static string getLanguage();
    static string getCountyCode();
    static void closeApp();
    static void init(const std::string &appName) {
        ZPTNativeHelper::_appName = appName;
    }
    static void setAppUrl(const std::string &appUrl) {
        ZPTNativeHelper::_appUrl = appUrl;
    }
    static string getAppUrl() {
        return ZPTNativeHelper::_appUrl;
    }
    
private:
    static std::string _appName;
    static std::string _appUrl;
};

#endif /* defined(__AppGift__ZPTNativeHelper__) */
