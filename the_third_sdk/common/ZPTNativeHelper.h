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
#include <map>
#include "CrossApp.h"
#include "CrossAppExt.h"

class ZPTNativeHelper
{
public:
    static void getDeviceInfo(CSJsonDictionary& dic);
    static void getPrivateInfo(CSJsonDictionary& dic);
    static void openUrl(const char* url);
    static void sendMail(const string &target, const string &title, CSJsonDictionary& extra, const string &format);
    static string getDeviceId();
    static string getAppVersion(bool replaceDot = false);
    static int getAppBuild();
    static string getStatusKey();
    static string getLanguage();
    static string getCountyCode();
    static void closeApp();
};

#endif /* defined(__AppGift__ZPTNativeHelper__) */
