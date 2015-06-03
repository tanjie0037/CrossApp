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
#import "AppController.h"

#import "SponsorPaySDK.h"
#import "Supersonic/Supersonic.h"

USING_NS_CC;

@interface MySupersonicDelegate : NSObject <SupersonicLogDelegate, SupersonicOWDelegate> {}
@end

@implementation MySupersonicDelegate
- (void)sendLog:(NSString *)log level:(LogLevel)level tag:(LogTag)tag
{
    CCLog("MySupersonicDelegate:[tag:%d][msg:%s]", tag, [log UTF8String]);
}

- (void)supersonicOWInitSuccess
{
    CCLog("supersonicOWInitSuccess");
}

- (void)supersonicOWShowSuccess
{
    CCLog("supersonicOWShowSuccess");
}

- (void)supersonicOWInitFailedWithError:(NSError *)error
{
    CCLog("supersonicOWInitFailedWithError");
    NSLog(@"error:%@", error);
}

- (void)supersonicOWShowFailedWithError:(NSError *)error
{
    CCLog("supersonicOWShowFailedWithError");
    NSLog(@"error:%@", error);
}

- (void)supersonicOWAdClosed
{
    CCLog("supersonicOWAdClosed");
}

- (BOOL)supersonicOWDidReceiveCredit:(NSDictionary *)creditInfo
{
    //we confirm it at server side
    return YES;
}

- (void)supersonicOWFailGettingCreditWithError:(NSError *)error
{
    CCLog("supersonicOWFailGettingCreditWithError");
    NSLog(@"error:%@", error);
}
@end

static MySupersonicDelegate* _mySupersonicDelegate = [[MySupersonicDelegate alloc] init];

void AdHelper::initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token) {
    switch (type) {
        case AdFyber:
            [SponsorPaySDK startForAppId:[NSString stringWithUTF8String:appkey.c_str()]
                                  userId:[NSString stringWithUTF8String:uId.c_str()]
                           securityToken:[NSString stringWithUTF8String:token.c_str()]];
            break;
            
        case AdSupersonic:
            //uId: 1 to 64 characters
            [Supersonic sharedInstance];
            [[Supersonic sharedInstance] setOWDelegate:_mySupersonicDelegate];
            [[Supersonic sharedInstance] initOWWithAppKey:[NSString stringWithUTF8String:appkey.c_str()]
                                               withUserId:[NSString stringWithUTF8String:uId.c_str()]];
            [[Supersonic sharedInstance] setLogDelegate:_mySupersonicDelegate];
            break;
        default:
            break;
    }
}

void AdHelper::callOfferwall(AdHelper::AdType type) {
    switch (type) {
        case AdFyber:
            [SponsorPaySDK showOfferWallWithParentViewController:(UIViewController*)[AppController getRootView] completion: ^(int status) {
                if (SPONSORPAY_ERR_NETWORK == status) {
                    CCLog("call offerwall failed.[%s, %s]", "Fyber", "SPONSORPAY_ERR_NETWORK");
                }
            }];
            break;
            
        case AdSupersonic:
            [[Supersonic sharedInstance] showOW];
            break;
            
        default:
            break;
    }
}

void AdHelper::playVideo(AdHelper::AdType type) {
    //todo
}

#endif

