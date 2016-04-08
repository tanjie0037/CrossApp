//
//  ADRewardedVideoViewController.h
//  AdscendMedia
//
//  Created by Tabish on 9/29/15.
//  Copyright © 2015 AdscendMedia. All rights reserved.
//

#import <UIKit/UIKit.h>


@protocol ADRewardedVideoViewControllerDelegate <NSObject>
- (void)onUserCredited;
- (void)onUserNotCredited;
- (void)onCloseVideoVCPressed;
@end

@interface ADRewardedVideoViewController : UIViewController

+ (ADRewardedVideoViewController*)newRewardedVideoForPublisherId:(NSString*)publisherId profileId:(NSString*)profileId subId1:(NSString*)subId1 delegate:(id<ADRewardedVideoViewControllerDelegate>)delegate;

@property (nonatomic, weak) id<ADRewardedVideoViewControllerDelegate> delegate; //required for receiving user credited call back

@end
