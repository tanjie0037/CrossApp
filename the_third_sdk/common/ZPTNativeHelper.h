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
    static void openUrl(const char* url);
    static void sendMail(const char *target, const char *title, CSJsonDictionary& extra);
};

#endif /* defined(__AppGift__ZPTNativeHelper__) */
