//
//  AdHelper.h
//  AppGift
//
//  Created by tanjie on 15/6/2.
//
//

#ifndef AppGift_AdHelper_h
#define AppGift_AdHelper_h

#include "CrossApp.h"

class AdHelper {
public:
    enum AdType {
        Fyber = 0
    };
    
public:
    static void initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token);
    static void callOfferwall(AdType type);
    static void playVideo(AdType type);
};

#endif
