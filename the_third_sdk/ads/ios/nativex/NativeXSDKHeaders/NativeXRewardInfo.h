//
//  NativeXRewardInfo.h
//  NativeXSDK
//
//  Created by Joel Braun on 2015.02.09.
//
//

#import <Foundation/Foundation.h>
#include "NativeXReward.h"

@interface NativeXRewardInfo : NSObject

/*
 * Array of NativeXRewards detailing exact rewards to be given to the user
 */
@property (nonatomic, readonly) NSArray* rewards;

/**
 *  Create CurrencyInfo object using API JSON response
 *
 *  @param APIResult    NSDictionary of API results
 *
 *  @return an object version of json response
 */
-(id)initWithRedeemBalancesResult:(NSDictionary *)APIResult;

/**
 *  Calling this method will display a native iOS alert view on success
 */
-(void)showRedeemAlert;

/**
 *  converts object to dictionary (to prep for JSON)
 *
 *  @return NSDictionary version of object
 */
-(id)proxyForJson;

@end
