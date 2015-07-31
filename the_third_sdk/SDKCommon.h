//
//  SDKCommon.h
//  AppGift
//
//  Created by tanjie on 15/6/5.
//
//

#ifndef AppGift_SDKCommon_h
#define AppGift_SDKCommon_h

//static int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
//{
//    int StringLen;
//    char caNewString[128];
//    char* FindPos;
//    FindPos =(char *)strstr(sSrc, sMatchStr);
//    if( (!FindPos) || (!sMatchStr) )
//        return -1;
//    
//    while( FindPos )
//    {
//        memset(caNewString, 0, sizeof(caNewString));
//        StringLen = FindPos - sSrc;
//        strncpy(caNewString, sSrc, StringLen);
//        strcat(caNewString, sReplaceStr);
//        strcat(caNewString, FindPos + strlen(sMatchStr));
//        strcpy(sSrc, caNewString);
//        
//        FindPos =(char *)strstr(sSrc, sMatchStr);
//    }
//    free(FindPos);
//    return 0;
//}

#include <string.h>
#include <stdlib.h>

static char *str_copy(const char *str) {
    int len = strlen(str) + 1;
    char *buf = (char *)malloc(len);
    if (NULL == buf) return NULL;
    memcpy(buf, str, len);
    return buf;
}

static char* str_replace(const char *str, const char *sub, const char *replace) {
    char *pos = (char *) str;
    char *find = strstr(str, sub);
        
    if (!find) return str_copy(str);
    int size = 128;
    
    char *result = (char *) malloc(size);
    if (NULL == result) return NULL;
    memset(result, '\0', size);
    char *current;
    
    while ((current = strstr(pos, sub))) {
        int len = current - pos;
        strncat(result, pos, len);
        strncat(result, replace, strlen(replace));
        pos = current + strlen(sub);
    }
    
    if (pos != (str + strlen(str))) {
        strncat(result, pos, (str - pos));
    }
    
    return result;
}

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

/*
    NSDictionary to jsonString
 */
static NSString *dict2jsonString(NSDictionary *object) {
    NSString *jsonString = nil;
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:object
                                                       options:NSJSONWritingPrettyPrinted // Pass 0 if you don't care about the readability of the generated string
                                                         error:&error];
    if (! jsonData) {
        NSLog(@"Got an error: %@", error);
    } else {
        jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    }
    return jsonString;
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <android/log.h>

#ifndef LOG_TAG
#define  LOG_TAG    "jni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#endif

#endif  //CC_PLATFORM_ANDROID


#endif  //AppGift_SDKCommon_h

