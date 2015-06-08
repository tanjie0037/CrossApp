//
//  SDKCommon.h
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#ifndef AppGift_SDKCommon_h
#define AppGift_SDKCommon_h


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>

#ifndef utf8cstr
#define utf8cstr(nsstr) (nsstr ? [nsstr cStringUsingEncoding:NSUTF8StringEncoding] : "")
#define nsstr(cstr) (cstr ? [NSString stringWithUTF8String:cstr] : @"")
#endif


/*!
 * @brief 把格式化的JSON格式的字符串转换成字典
 * @param jsonString JSON格式的字符串
 * @return 返回字典
 */
static NSDictionary* dictionaryWithJsonString(NSString *jsonString) {
    if (jsonString == nil) {
        return nil;
    }
    
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err = nil;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                        options:NSJSONReadingMutableContainers
                                                          error:&err];
    if(err) {
        NSLog(@"json解析失败：%@",err);
        NSLog(@"jsonString: %@", jsonString);
        assert(0);
        return nil;
    }
    return dic;
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <android/log.h>

#ifndef LOG_TAG
#define  LOG_TAG    "jni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#endif

#endif  //CC_PLATFORM_ANDROID


#endif  //AppGift_SDKCommon_h

