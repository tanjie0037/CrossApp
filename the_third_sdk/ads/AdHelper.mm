//
//  AdHelper.mm
//  AppGift
//
//  Created by tanjie on 15/6/2.
//
//

#include "AdHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import "SponsorPaySDK.h"
#import "AppController.h"

USING_NS_CC;

void AdHelper::initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token) {
    switch (type) {
        case Fyber:
            [SponsorPaySDK startForAppId:[NSString stringWithUTF8String:appkey.c_str()]
                                  userId:[NSString stringWithUTF8String:uId.c_str()]
                           securityToken:[NSString stringWithUTF8String:token.c_str()]];
            break;
            
        default:
            break;
    }
}

void AdHelper::callOfferwall(AdHelper::AdType type) {
    switch (type) {
        case Fyber:
            [SponsorPaySDK showOfferWallWithParentViewController:(UIViewController*)[AppController getRootView] completion: ^(int status) {
                if (SPONSORPAY_ERR_NETWORK == status) {
                    CCLog("call offerwall failed.[%s, %s]", "Fyber", "SPONSORPAY_ERR_NETWORK");
                }
            }];
            break;
            
        default:
            break;
    }
}

void AdHelper::playVideo(AdHelper::AdType type) {
    //todo
}

#endif

