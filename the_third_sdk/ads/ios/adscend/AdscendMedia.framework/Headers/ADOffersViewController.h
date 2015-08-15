//
//  ADOffersViewController.h
//  SampleApp
//
//  Created by Tabish on 4/20/15.
//  Copyright (c) 2015 AdscendMedia. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
    AdGender_Male,
    AdGender_Female,
    
    AdGender_Ignore
}AdGender;

typedef enum {
    AdAgeRange_1835,    //18 - 35 years
    AdAgeRange_3545,    //35 - 45 years
    AdAgeRange_4555,    //45 - 55 years
    AdAgeRange_5565,    //55 - 65 years
    AdAgeRange_65,      //65 and above
    
    AdAgeRange_Ignore
}AdAgeRange;

@protocol ADOffersViewControllerDelegate <NSObject>
- (void)onCloseOffersVCPressed;
@end


@interface ADOffersViewController : UIViewController

//the only way you should access the offer wall
+ (ADOffersViewController*)newOffersWallForPublisherId:(NSString*)publisherId adwallId:(NSString*)adwallId subId1:(NSString*)subId1 delegate:(id<ADOffersViewControllerDelegate>)delegate;

@property (nonatomic, weak) id<ADOffersViewControllerDelegate> delegate; //required for receiving close button call back

//optional properties
@property (nonatomic) AdGender gender;
@property (nonatomic) AdAgeRange ageRange;
@property (nonatomic, strong) NSString* subid2;
@property (nonatomic, strong) NSString* subid3;
@property (nonatomic, strong) NSString* subid4;

@end