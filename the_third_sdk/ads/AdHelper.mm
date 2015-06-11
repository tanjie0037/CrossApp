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
#import "NativeXSDK.h"
//#import "SponsorPaySDK.h"
#import "Supersonic/Supersonic.h"
#include "SDKCommon.h"

USING_NS_CC;

@interface MySupersonicDelegate : NSObject <SupersonicLogDelegate, SupersonicOWDelegate> {}
@end

@implementation MySupersonicDelegate
- (void)sendLog:(NSString *)log level:(LogLevel)level tag:(LogTag)tag
{
    CCLOG("MySupersonicDelegate:[tag:%d][msg:%s]", tag, [log UTF8String]);
}

- (void)supersonicOWInitSuccess
{
    CCLOG("supersonicOWInitSuccess");
}

- (void)supersonicOWShowSuccess
{
    CCLOG("supersonicOWShowSuccess");
}

- (void)supersonicOWInitFailedWithError:(NSError *)error
{
    CCLOG("supersonicOWInitFailedWithError");
    NSLog(@"error:%@", error);
}

- (void)supersonicOWShowFailedWithError:(NSError *)error
{
    CCLOG("supersonicOWShowFailedWithError");
    NSLog(@"error:%@", error);
}

- (void)supersonicOWAdClosed
{
    CCLOG("supersonicOWAdClosed");
}

- (BOOL)supersonicOWDidReceiveCredit:(NSDictionary *)creditInfo
{
    //we confirm it at server side
    return YES;
}

- (void)supersonicOWFailGettingCreditWithError:(NSError *)error
{
    CCLOG("supersonicOWFailGettingCreditWithError");
    NSLog(@"error:%@", error);
}
@end

@interface MyNaviteXDeleagte : NSObject <NativeXSDKDelegate, NativeXAdViewDelegate> {}
@end

@implementation MyNaviteXDeleagte
/** Called when the Offer Wall is successfully initialized. */
- (void)nativeXSDKDidCreateSession {
    CCLOG("nativeXSDKDidCreateSession");
}

/** Called when there is an error trying to initialize the Offer Wall.
 *
 * @param   error
 *          reason why create session call failed
 */
- (void)nativeXSDKDidFailToCreateSession: (NSError *) error {
    CCLOG("nativeXSDKDidFailToCreateSession");
    NSLog(@"error:%@", error);
}

/** Called when the currency redemption is unsuccessfull.
 *
 * @param   error
 *          reason why redeem currency call failed
 */
- (void)nativeXSDKDidRedeemWithError:(NSError *)error {
    CCLOG("nativeXSDKDidRedeemWithError");
    NSLog(@"error:%@", error);
}

/** Called when adView is loaded and ready to be displayed
 * use this method to override when adView is displayed
 * If this delegate does not exist when caching an ad it will be shown immediately
 *
 * @param adView        the NativeX adView that has been loaded
 * @param placement     NSString representation of placement, used to distinquish which ad is loaded
 */
- (void)nativeXAdView:(NativeXAdView *)adView didLoadWithPlacement:(NSString *)placement {
    CCLOG("nativeXAdView:didLoadWithPlacement, placement:%s", utf8cstr(placement));
    [[NativeXSDK sharedInstance] showReadyAdWithPlacement:kAdPlacementStoreOpen];
}

/** called if no ad is available at this time
 *
 * @param adView        the NativeX adView that has NOT been loaded
 */
- (void)nativeXAdViewNoAdContent:(NativeXAdView *)adView {
    CCLOG("nativeXAdView:nativeXAdViewNoAdContent");
}

/** Called when error loading an ad (was the SDK initialized correctly?)
 *
 * @param adView        the NativeX adView that has NOT been loaded because of an error
 * @param error         reason why ad failed to load
 */
- (void)nativeXAdView:(NativeXAdView *)adView didFailWithError:(NSError *)error {
    CCLOG("nativeXAdView:didFailWithError");
    NSLog(@"error:%@", error);
}

/** Called when ad content has expired for specific adView
 *
 * @param adView        the NativeX adView that has expired
 */
- (void)nativeXAdViewDidExpire:(NativeXAdView *)adView {
    CCLOG("nativeXAdView::nativeXAdViewDidExpire");
}

- (void) nativeXSDKDidRedeemWithRewardInfo:(NativeXRewardInfo *)rewardInfo {
    // Add code to handle the reward info and credit your user here.
    int totalRewardAmount = 0;
    for (NativeXReward *reward in rewardInfo.rewards) {
        NSLog(@"Reward: rewardName:%@ rewardId:%@ amount:%@", reward.rewardName, reward.rewardId, reward.amount);
        // grab the amount and add it to total
        totalRewardAmount += [reward.amount intValue];
    }
}

@end

static MySupersonicDelegate *_mySupersonicDelegate = [[MySupersonicDelegate alloc] init];
static MyNaviteXDeleagte *_myNaviteXDeleagte = [[MyNaviteXDeleagte alloc] init];

void AdHelper::initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token) {
    switch (type) {
        case AdFyber:
//            [SponsorPaySDK startForAppId:[NSString stringWithUTF8String:appkey.c_str()]
//                                  userId:[NSString stringWithUTF8String:uId.c_str()]
//                           securityToken:[NSString stringWithUTF8String:token.c_str()]];
            break;
            
        case AdSupersonic:
            //uId: 1 to 64 characters
            [Supersonic sharedInstance];
            [[Supersonic sharedInstance] setOWDelegate:_mySupersonicDelegate];
            [[Supersonic sharedInstance] setLogDelegate:_mySupersonicDelegate];
            [[Supersonic sharedInstance] initOWWithAppKey:[NSString stringWithUTF8String:appkey.c_str()]
                                               withUserId:[NSString stringWithUTF8String:uId.c_str()]];
            break;
         case AdNativeX:
            [[NativeXSDK sharedInstance] setDelegate:_myNaviteXDeleagte];
            [[NativeXSDK sharedInstance] createSessionWithAppId:nsstr(appkey.c_str())
                                             andPublisherUserId:nsstr(uId.c_str())];
        default:
            break;
    }
}

void AdHelper::callOfferwall(AdHelper::AdType type) {
    switch (type) {
        case AdFyber:
//            [SponsorPaySDK showOfferWallWithParentViewController:(UIViewController*)[AppController getRootView] completion: ^(int status) {
//                if (SPONSORPAY_ERR_NETWORK == status) {
//                    CCLOG("call offerwall failed.[%s, %s]", "Fyber", "SPONSORPAY_ERR_NETWORK");
//                }
//            }];
            break;
            
        case AdSupersonic:
            [[Supersonic sharedInstance] showOW];
            break;
        case AdNativeX:
            [[NativeXSDK sharedInstance] fetchAdWithPlacement:kAdPlacementStoreOpen delegate:_myNaviteXDeleagte];
        default:
            break;
    }
}

void AdHelper::playVideo(AdHelper::AdType type) {
    //todo
}

#endif

