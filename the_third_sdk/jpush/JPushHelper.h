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
public:
   static void initJPush(const std::string &uId, const std::set<std::string>& tags);
};

#endif /* defined(__AppGift__JPushHelper__) */
