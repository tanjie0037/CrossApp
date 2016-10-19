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
//#import "NativeXSDK.h"
//#import "FyberSDK.h"
#import "Supersonic/Supersonic.h"
//#import <AdscendMedia/AdscendMedia.h>
#import <Tapjoy/Tapjoy.h>
#import <Tapjoy/TJPlacement.h>

#include "SDKCommon.h"

USING_NS_CC;

//static const int NX_FREE = 0;
//static const int NX_READY = 1;
static const int TJ_FREE = 0;
static const int TJ_OFFER = 1;
static const int TJ_VIDEO = 2;

int AdHelper::_willShowTapjoy = TJ_FREE;
string AdHelper::K_NATIVEX_PLACEMENT_OFFER = "Store Open Offerwall";
string AdHelper::K_NATIVEX_PLACEMENT_VIDEO = "Game Launch Video";
string AdHelper::PLUGIN_PACKAGE = "com.zero.diaobaole";
string AdHelper::PLUGIN_ACTIVITY = "com.zero.diaobaole.MainActivity";
string AdHelper::PLUGIN_INTENT = "com.diao.diaobaole.OFFERWALL";
string AdHelper::PLUGIN_APK = "goldmine.apk";
string AdHelper::K_TAPJOY_PLACEMENT_OFFER = "offerwall_unit";
string AdHelper::K_TAPJOY_PLACEMENT_VIDEO = "video_unit";

static std::map<std::string, int> _nativeXStep;

bool AdHelper::_debug = false;
void AdHelper::setDebug(bool debug) {
    AdHelper::_debug = debug;
}

#pragma mark MySupersonicDelegate

@interface MySupersonicDelegate : NSObject <SupersonicLogDelegate, SupersonicOWDelegate, SupersonicRVDelegate> {}
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

//SupersonicRVDelegate func
- (void)supersonicRVInitSuccess
{
    CCLOG("supersonicRVInitSuccess");
}

- (void)supersonicRVInitFailedWithError:(NSError *)error
{
    CCLOG("supersonicRVInitFailedWithError");
    NSLog(@"error:%@", error);
}

- (void)supersonicRVAdAvailabilityChanged:(BOOL)hasAvailableAds
{
    CCLOG("supersonicRVAdAvailabilityChanged: %s", hasAvailableAds ? "true" : "no");
}

- (void)supersonicRVAdOpened
{
    CCLOG("supersonicRVAdOpened");
}

- (void)supersonicRVAdStarted
{
    CCLOG("supersonicRVAdStarted");
}

- (void)supersonicRVAdEnded
{
    CCLOG("supersonicRVAdEnded");
}

- (void)supersonicRVAdClosed
{
    CCLOG("supersonicRVAdClosed");
}

- (void)supersonicRVAdRewarded:(NSInteger)amount
{
    CCLOG("supersonicRVAdRewarded:%d", (int)amount);
}

- (void)supersonicRVAdFailedWithError:(NSError *)error
{
    CCLOG("supersonicRVAdFailedWithError");
    NSLog(@"error:%@", error);
}

@end

#pragma mark MyNaviteXDeleagte
//@interface MyNaviteXDeleagte : NSObject <NativeXSDKDelegate, NativeXAdViewDelegate> {}
//@end
//
//@implementation MyNaviteXDeleagte
///** Called when the Offer Wall is successfully initialized. */
//- (void)nativeXSDKDidCreateSession {
//    CCLOG("nativeXSDKDidCreateSession");
//    
//    [[NativeXSDK sharedInstance] fetchAdWithCustomPlacement:nsstr(AdHelper::K_NATIVEX_PLACEMENT_OFFER.c_str()) delegate:self];
//    [[NativeXSDK sharedInstance] fetchAdWithCustomPlacement:nsstr(AdHelper::K_NATIVEX_PLACEMENT_VIDEO.c_str()) delegate:self];
//}
//
///** Called when there is an error trying to initialize the Offer Wall.
// *
// * @param   error
// *          reason why create session call failed
// */
//- (void)nativeXSDKDidFailToCreateSession: (NSError *) error
//{
//    CCLOG("nativeXSDKDidFailToCreateSession");
//    NSLog(@"error:%@", error);
//}
//
///** Called when the currency redemption is unsuccessfull.
// *
// * @param   error
// *          reason why redeem currency call failed
// */
//- (void)nativeXSDKDidRedeemWithError:(NSError *)error
//{
//    CCLOG("nativeXSDKDidRedeemWithError");
//    NSLog(@"error:%@", error);
//}
//
///** Called when adView is loaded and ready to be displayed
// * use this method to override when adView is displayed
// * If this delegate does not exist when caching an ad it will be shown immediately
// *
// * @param adView        the NativeX adView that has been loaded
// * @param placement     NSString representation of placement, used to distinquish which ad is loaded
// */
//- (void)nativeXAdView:(NativeXAdView *)adView didLoadWithPlacement:(NSString *)placement
//{
//    CCLOG("nativeXAdView:didLoadWithPlacement, placement:%s", utf8cstr(placement));
//    
//    NSString *kOffer = nsstr(AdHelper::K_NATIVEX_PLACEMENT_OFFER.c_str());
//    NSString *kVideo = nsstr(AdHelper::K_NATIVEX_PLACEMENT_VIDEO.c_str());
//    
//    if ([placement isEqual:kOffer]) {
//        if (_nativeXStep[AdHelper::K_NATIVEX_PLACEMENT_OFFER] == NX_READY) {
//            _nativeXStep[AdHelper::K_NATIVEX_PLACEMENT_OFFER] = NX_FREE;
//            [[NativeXSDK sharedInstance] showReadyAdWithCustomPlacement:kOffer];
//            
//        } else {
//            _nativeXStep[AdHelper::K_NATIVEX_PLACEMENT_OFFER] = NX_READY;
//        }
//        
//    } else if ([placement isEqual:kVideo]) {
//        if (_nativeXStep[AdHelper::K_NATIVEX_PLACEMENT_VIDEO] == NX_READY) {
//            _nativeXStep[AdHelper::K_NATIVEX_PLACEMENT_VIDEO] = NX_FREE;
//            [[NativeXSDK sharedInstance] showReadyAdWithCustomPlacement:kVideo];
//            
//        } else {
//            _nativeXStep[AdHelper::K_NATIVEX_PLACEMENT_VIDEO] = NX_READY;
//        }
//        
//    } else {
//        NSAssert(NO, @"unknown placement");
//    }
//}
//
///** called if no ad is available at this time
// *
// * @param adView        the NativeX adView that has NOT been loaded
// */
//- (void)nativeXAdViewNoAdContent:(NativeXAdView *)adView
//{
//    CCLOG("nativeXAdView:nativeXAdViewNoAdContent");
//}
//
///** Called when error loading an ad (was the SDK initialized correctly?)
// *
// * @param adView        the NativeX adView that has NOT been loaded because of an error
// * @param error         reason why ad failed to load
// */
//- (void)nativeXAdView:(NativeXAdView *)adView didFailWithError:(NSError *)error
//{
//    CCLOG("nativeXAdView:didFailWithError");
//    NSLog(@"error:%@", error);
//}
//
///** Called when ad content has expired for specific adView
// *
// * @param adView        the NativeX adView that has expired
// */
//- (void)nativeXAdViewDidExpire:(NativeXAdView *)adView
//{
//    CCLOG("nativeXAdView::nativeXAdViewDidExpire");
//}
//
//- (void) nativeXSDKDidRedeemWithRewardInfo:(NativeXRewardInfo *)rewardInfo
//{
//    // do it server side
//}
//
//@end
//
#pragma mark ADOffersViewControllerDelegate
//@interface MyADOffersViewControllerDelegate : NSObject<ADOffersViewControllerDelegate> {}
//@property (nonatomic, strong) ADOffersViewController* controller;
//@end

//@implementation MyADOffersViewControllerDelegate
//- (void)onCloseOffersVCPressed
//{
//    if (self.controller) {
//        [self.controller dismissViewControllerAnimated:YES completion:^(void){
//            [self.controller release];
//            self.controller = nil;
//        }];
//    }
//}
//@end

#pragma mark TJDelegate
@interface MyTJDelegate : NSObject<TJPlacementDelegate>
@property (strong, nonatomic) TJPlacement *tjPlacementOffer;
@property (strong, nonatomic) TJPlacement *tjPlacementVideo;
@end

@implementation MyTJDelegate
-(void)tjcConnectSuccess:(NSNotification*)notifyObj
{
    NSLog(@"Tapjoy connect Succeeded");
    
    [_tjPlacementOffer requestContent];
    [_tjPlacementVideo requestContent];
}

- (void)tjcConnectFail:(NSNotification*)notifyObj
{
    NSLog(@"Tapjoy connect Failed");
}

- (void)requestDidSucceed:(TJPlacement*)placement
{
    NSLog(@"Tapjoy request did succeed, contentIsAvailable:%d", placement.isContentAvailable);
}

- (void)contentIsReady:(TJPlacement*)placement
{
    NSLog(@"Tapjoy placement content is ready to display");
    
    if(AdHelper::_willShowTapjoy == TJ_OFFER) {
        [_tjPlacementOffer showContentWithViewController: (UIViewController*)[AppController getRootView]];
        AdHelper::_willShowTapjoy = TJ_FREE;
        
    } else if (AdHelper::_willShowTapjoy == TJ_VIDEO) {
        [_tjPlacementVideo showContentWithViewController: (UIViewController*)[AppController getRootView]];
        AdHelper::_willShowTapjoy = TJ_FREE;
    }
}

- (void)requestDidFail:(TJPlacement*)placement error:(NSError *)error
{
    NSLog(@"Tapjoy request failed with error: %@", [error localizedDescription]);
}

- (void)contentDidAppear:(TJPlacement*)placement
{
    NSLog(@"Content did appear for %@ placement", [placement placementName]);
}

- (void)contentDidDisappear:(TJPlacement*)placement
{
    NSLog(@"Content did disappear for %@ placement", [placement placementName]);
}

@end

#pragma mark AdHelper
static MySupersonicDelegate *_mySupersonicDelegate = [[MySupersonicDelegate alloc] init];
//static MyNaviteXDeleagte *_myNaviteXDeleagte = [[MyNaviteXDeleagte alloc] init];
//static MyADOffersViewControllerDelegate *_myADOffersViewControllerDelegate = [[MyADOffersViewControllerDelegate alloc] init];
static MyTJDelegate *_myTJDelegate = [[MyTJDelegate alloc] init];

//AdFyber = 0,
//AdSupersonic = 1,
//AdNativeX = 2,
//AdAdxmi = 3,
//AdTapjoy = 4

std::map<std::string, AdHelper::AdType> AdHelper::AD_TYPE = AdHelper::createMap();

//ADOffersViewController* _offersVC = nil;
string _uId;
string _appkey;
string _token;

void AdHelper::initAdInGoldMine(const CSJson::Value &params) {}

void AdHelper::initAd(AdType type, const std::string &uId, const std::string &appkey, const std::string &token)
{
    switch (type) {
        case AdFyber:{
//            FYBSDKOptions *options = [FYBSDKOptions optionsWithAppId:nsstr(appkey.c_str()) userId:nsstr(uId.c_str()) securityToken:nsstr(token.c_str())];
//            [FyberSDK startWithOptions:options];
            break;
        }

        case AdSupersonic:
            //uId: 1 to 64 characters
            [Supersonic sharedInstance];
            [[Supersonic sharedInstance] setOWDelegate:_mySupersonicDelegate];
            [[Supersonic sharedInstance] setLogDelegate:_mySupersonicDelegate];
            [[Supersonic sharedInstance] setRVDelegate:_mySupersonicDelegate];
            [[Supersonic sharedInstance] initOWWithAppKey:[NSString stringWithUTF8String:appkey.c_str()]
                                               withUserId:[NSString stringWithUTF8String:uId.c_str()]];
            
            [[Supersonic sharedInstance] initRVWithAppKey:[NSString stringWithUTF8String:appkey.c_str()]
                                               withUserId:[NSString stringWithUTF8String:uId.c_str()]];
            break;
            
        case AdNativeX:
//            _nativeXStep[K_NATIVEX_PLACEMENT_OFFER] = NX_FREE;
//            _nativeXStep[K_NATIVEX_PLACEMENT_VIDEO] = NX_FREE;
//        
//            [[NativeXSDK sharedInstance] setDelegate:_myNaviteXDeleagte];
//            [[NativeXSDK sharedInstance] createSessionWithAppId:nsstr(appkey.c_str())
//                                             andPublisherUserId:nsstr(uId.c_str())];
            break;
        case AdAdscend:
            _uId = uId;
            _appkey = appkey;
            _token = token;
            break;
        case AdTapjoy:
            //Turn on Tapjoy debug mode
            [Tapjoy setDebugEnabled: _debug ? YES : NO]; //Do not set this for any version of the app released to an app store
            
            //Set up success and failure notifications
            [[NSNotificationCenter defaultCenter] addObserver:_myTJDelegate
                                                     selector:@selector(tjcConnectSuccess:)
                                                         name:TJC_CONNECT_SUCCESS
                                                       object:nil];
            
            [[NSNotificationCenter defaultCenter] addObserver:_myTJDelegate
                                                     selector:@selector(tjcConnectFail:)
                                                         name:TJC_CONNECT_FAILED
                                                       object:nil];
            
            //Tapjoy connect call
            [Tapjoy connect: nsstr(appkey.c_str())];
            [Tapjoy setUserID: nsstr(uId.c_str())];
            
            _myTJDelegate.tjPlacementOffer = [TJPlacement placementWithName:nsstr(K_TAPJOY_PLACEMENT_OFFER.c_str()) delegate:_myTJDelegate ];
            _myTJDelegate.tjPlacementVideo = [TJPlacement placementWithName:nsstr(K_TAPJOY_PLACEMENT_VIDEO.c_str()) delegate:_myTJDelegate ];
            break;
        default:
            break;
    }
}

void AdHelper::callOfferwall(AdHelper::AdType type, bool inGoldMine)
{
    switch (type) {
        case AdFyber: {
            // Create an instance of the FYBOfferWallViewController
//            FYBOfferWallViewController *offerWallViewController = [[FYBOfferWallViewController alloc] init];
            
            // Show a close button while the Offer Wall is loading
//            offerWallViewController.showCloseButtonOnLoad = YES;
            
            // Dismiss the Offer Wall when the user leaves your application
//            offerWallViewController.shouldDismissOnRedirect = YES;
            
            // Show the Offer Wall
//            [offerWallViewController presentFromViewController:(UIViewController*)[AppController getRootView] animated:YES completion:^{
//                
//                // Code executed when the Offer Wall is presented
//                NSLog(@"Offer was presented");
//                
//            } dismiss:^(NSError *error){
//                
//                // Code executed when the Offer Wall is dismissed
//                NSLog(@"Offer is dismissed: %@", error);
//            }];   
            break;
        }
        case AdSupersonic:
            [[Supersonic sharedInstance] showOW];
            break;
        case AdNativeX: {
//            _nativeXStep[K_NATIVEX_PLACEMENT_OFFER] = NX_READY;
//        
//            if ([[NativeXSDK sharedInstance] isAdReadyWithCustomPlacement:nsstr(K_NATIVEX_PLACEMENT_OFFER.c_str())]) {
//                _nativeXStep[K_NATIVEX_PLACEMENT_OFFER] = NX_FREE;
//                [[NativeXSDK sharedInstance] showReadyAdWithCustomPlacement:nsstr(K_NATIVEX_PLACEMENT_OFFER.c_str())];
//                
//            } else {
//                [[NativeXSDK sharedInstance] fetchAdWithCustomPlacement: nsstr(K_NATIVEX_PLACEMENT_OFFER.c_str()) delegate: _myNaviteXDeleagte];
//            }
            break;
        }
        case AdAdxmi:
            break;
        case AdAdscend: {
//            _offersVC = [ADOffersViewController newOffersWallForPublisherId:nsstr(_appkey.c_str()) adwallId:nsstr(_token.c_str()) subId1:nsstr(_uId.c_str()) delegate:_myADOffersViewControllerDelegate];
//            _myADOffersViewControllerDelegate.controller = _offersVC;
//            [(UIViewController*)[AppController getRootView] presentViewController:_offersVC animated:YES completion:nil];
            break;
        }
        case AdTapjoy: {
            AdHelper::_willShowTapjoy = TJ_OFFER;
            
            if(_myTJDelegate.tjPlacementOffer.isContentReady) {
                [_myTJDelegate.tjPlacementOffer showContentWithViewController: (UIViewController*)[AppController getRootView]];
                AdHelper::_willShowTapjoy = TJ_FREE;
            }
            else {
                [_myTJDelegate.tjPlacementOffer requestContent];
                //handle situation where there is no content to show, or it has not yet downloaded.
                NSLog(@"Tapjoy offerwall is not ready");
            }
            break;
        }
        default:
            break;
    }
}

void AdHelper::playVideo(AdHelper::AdType type, bool inGoldMine)
{
    switch (type) {
        case AdFyber:
            break;
            
        case AdSupersonic: {
            if ([[Supersonic sharedInstance] isAdAvailable]) {
                CCLOG("Supersonic AdAvailable");
                [[Supersonic sharedInstance] showRV];
            }
            break;
        }
        case AdNativeX: {
//            _nativeXStep[K_NATIVEX_PLACEMENT_VIDEO] = NX_READY;
//            
//            if ([[NativeXSDK sharedInstance] isAdReadyWithCustomPlacement:nsstr(K_NATIVEX_PLACEMENT_VIDEO.c_str())]) {
//                _nativeXStep[K_NATIVEX_PLACEMENT_VIDEO] = NX_FREE;
//                [[NativeXSDK sharedInstance] showReadyAdWithCustomPlacement:nsstr(K_NATIVEX_PLACEMENT_VIDEO.c_str())];
//                
//            } else {
//                [[NativeXSDK sharedInstance] fetchAdWithCustomPlacement: nsstr(K_NATIVEX_PLACEMENT_VIDEO.c_str()) delegate: _myNaviteXDeleagte];
//            }
            break;
        }
        case AdTapjoy: {
            AdHelper::_willShowTapjoy = TJ_VIDEO;
            
            if(_myTJDelegate.tjPlacementVideo.isContentReady) {
                [_myTJDelegate.tjPlacementVideo showContentWithViewController: (UIViewController*)[AppController getRootView]];
                AdHelper::_willShowTapjoy = TJ_FREE;
            }
            else {
                [_myTJDelegate.tjPlacementVideo requestContent];
                //handle situation where there is no content to show, or it has not yet downloaded.
                NSLog(@"Tapjoy video is not ready");
            }
            break;
        }
        default:
            break;
    }
}

#endif

