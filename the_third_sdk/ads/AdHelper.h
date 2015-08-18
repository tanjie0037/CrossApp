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
        AdFyber = 0,
        AdSupersonic = 1,
        AdNativeX = 2,
        AdAdxmi = 3,
        AdTapjoy = 4,
        AdAdscend = 5,
        AdSuperrewards = 6
    };
    
    static std::map<std::string, AdType> AD_TYPE;
    
    static std::map<std::string, AdType> createMap() {
        std::map<std::string, AdType> map;
        map["fyber"] = AdFyber;
        map["supersonic"] = AdSupersonic;
        map["nativeX"] = AdNativeX;
        map["adxmi"] = AdAdxmi;
        map["tapjoy"] = AdTapjoy;
        map["adscend"] = AdAdscend;
        map["superrewards"] = AdSuperrewards;
        return map;
    }
    
    static std::string K_NATIVEX_PLACEMENT_OFFER;
    static std::string K_NATIVEX_PLACEMENT_VIDEO;
    
public:
    static void initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token);
    static void callOfferwall(AdType type);
    static void playVideo(AdType type);
};

#endif
