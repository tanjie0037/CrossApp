//
//  ShareHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/5/28.
//
//

#include "ShareHelper.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

void ShareHelper::open(const std::string& appKey, bool useAppTrusteeship) {
    NSString *appKeyStr = [NSString stringWithCString:appKey.c_str() encoding:NSUTF8StringEncoding];
    [ShareSDK registerApp:appKeyStr useAppTrusteeship:useAppTrusteeship];
}

void ShareHelper::close() {
    //todo
}

void ShareHelper::setPlatformConfig(C2DXPlatType platType, CSJsonDictionary& configInfo) {
    NSDictionary* dic = nil;
    NSMutableDictionary *configDict = nil;
    
    if (!configInfo.isEmpty()) {
        dic = dictionaryWithJsonString([NSString stringWithUTF8String:configInfo.getDescription().c_str()]);
        configDict = [NSMutableDictionary dictionaryWithDictionary:dic];
        
        switch (platType) {
            case ShareTypeWeixiSession:
            case ShareTypeYiXinSession:
                [configDict setObject:[NSNumber numberWithInt:0] forKey:@"scene"];
                break;
            case ShareTypeWeixiTimeline:
            case ShareTypeYiXinTimeline:
                [configDict setObject:[NSNumber numberWithInt:1] forKey:@"scene"];
                break;
            case ShareTypeWeixiFav:
                [configDict setObject:[NSNumber numberWithInt:2] forKey:@"scene"];
                break;
            default:
                break;
        }
    }
    
    [ShareSDK connectPlatformWithType:(ShareType)platType
                             platform:nil
                              appInfo:configDict];
}

void ShareHelper::authorize(C2DXPlatType platType, C2DXAuthResultEvent callback) {
    [ShareSDK authWithType:(ShareType)platType
                   options:nil
                    result:^(SSAuthState state, id<ICMErrorInfo> error) {
     
     CSJsonDictionary errorInfo;
     
     if (error)
     {
     errorInfo.insertItem("error_code", (int)[error errorCode]);
     errorInfo.insertItem("error_msg", [[error errorDescription] UTF8String]);
     }
     
     if (callback)
     {
     callback ((C2DXResponseState)state, platType, errorInfo);
     }
     
     }];
    
}

void ShareHelper::cancelAuthorize(C2DXPlatType platType) {
    [ShareSDK cancelAuthWithType:(ShareType)platType];
}

bool ShareHelper::hasAutorized(C2DXPlatType platType) {
    return [ShareSDK hasAuthorizedWithType:(ShareType)platType] ? true : false;
}

void ShareHelper::getUserInfo(C2DXPlatType platType, C2DXGetUserInfoResultEvent callback) {
    [ShareSDK getUserInfoWithType:(ShareType)platType
                      authOptions:nil
                           result:^(BOOL result, id<ISSPlatformUser> userInfo, id<ICMErrorInfo> error) {
     
     CSJsonDictionary userInfoDict;
     CSJsonDictionary errorInfo;
     
     NSError *parseError = nil;
     
     if (result) {
     NSData* jsonData = [NSJSONSerialization dataWithJSONObject:[userInfo sourceData] options:NSJSONWritingPrettyPrinted error:&parseError];
     
     if (parseError) {
     assert(0);
     NSLog(@"parseError:%@", parseError);
     return;
     } else {
     NSString* str = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
     userInfoDict.initWithDescription(utf8cstr(str));
     [str release];
     }
     }
     
     if (error)
     {
     NSInteger errCode = [error errorCode];
     NSString *errDesc = [error errorDescription];
     
     errorInfo.insertItem("error_code", (int)errCode);
     if (errDesc)
     {
     errorInfo.insertItem("error_msg", [errDesc UTF8String]);
     }
     
     }
     
     if (callback)
     {
     callback (result ? C2DXResponseStateSuccess : C2DXResponseStateFail, platType, userInfoDict, errorInfo);
     }
     
     }];
}

void ShareHelper::shareContent(cn::sharesdk::C2DXPlatType platType, CSJsonDictionary &content, cn::sharesdk::C2DXShareResultEvent callback) {
    id<ISSContent> publishContent = convertPublishContent(content);
    [ShareSDK shareContent:publishContent
                      type:(ShareType)platType
               authOptions:nil
              shareOptions:nil
             statusBarTips:YES
                    result:^(ShareType type, SSResponseState state, id<ISSPlatformShareInfo> statusInfo, id<ICMErrorInfo> error, BOOL end) {
     
     CSJsonDictionary shareInfo;
     CSJsonDictionary errorInfo;
     
     NSError *parseError = nil;
     
     if (state == SSResponseStateSuccess) {
     NSData* jsonData = [NSJSONSerialization dataWithJSONObject:[statusInfo sourceData] options:NSJSONWritingPrettyPrinted error:&parseError];
     
     if (parseError) {
     assert(0);
     NSLog(@"parseError:%@", parseError);
     return;
     } else {
     NSString* str = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
     shareInfo.initWithDescription(utf8cstr(str));
     [str release];
     }
     }
     
     if (error) {
     NSInteger errCode = [error errorCode];
     NSString *errDesc = [error errorDescription];
     
     errorInfo.insertItem("error_code", (int)errCode);
     if (errDesc) {
     errorInfo.insertItem("error_msg", utf8cstr(errDesc));
     }
     }
     
     if (callback) {
     callback ((C2DXResponseState)state, (C2DXPlatType)type, shareInfo, errorInfo);
     }
     
     }];
}

void ShareHelper::oneKeyShareContent(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback) {
    //todo
}

void ShareHelper::showShareMenu(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback) {
    //todo
}

void ShareHelper::showShareMenu(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, CrossApp::CCPoint pt, cn::sharesdk::C2DXMenuArrowDirection direction, cn::sharesdk::C2DXShareResultEvent callback) {
    //todo
}

void ShareHelper::showShareView(cn::sharesdk::C2DXPlatType platType, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback) {
    id<ISSContent> publishContent = convertPublishContent(content);
    
    [ShareSDK showShareViewWithType:(ShareType)platType
                          container:nil
                            content:publishContent
                      statusBarTips:YES
                        authOptions:nil
                       shareOptions:nil
                             result:^(ShareType type, SSResponseState state, id<ISSPlatformShareInfo> statusInfo, id<ICMErrorInfo> error, BOOL end) {
     
     CSJsonDictionary shareInfo;
     CSJsonDictionary errorInfo;
     
     NSError *parseError = nil;
     
     if (state == SSResponseStateSuccess) {
     NSData* jsonData = [NSJSONSerialization dataWithJSONObject:[statusInfo sourceData] options:NSJSONWritingPrettyPrinted error:&parseError];
     
     if (parseError) {
     assert(0);
     NSLog(@"parseError:%@", parseError);
     } else {
     NSString* str = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
     shareInfo.initWithDescription(utf8cstr(str));
     [str release];
     }
     }
     
     if (error) {
     NSInteger errCode = [error errorCode];
     NSString *errDesc = [error errorDescription];
     
     errorInfo.insertItem("error_code", (int)errCode);
     if (errDesc) {
     errorInfo.insertItem("error_msg", utf8cstr(errDesc));
     }
     }
     
     if (callback) {
     callback ((C2DXResponseState)state, (C2DXPlatType)type, shareInfo, errorInfo);
     }
     
     
     }];
}

bool ShareHelper::isClientInstalled(cn::sharesdk::C2DXPlatType platType) {
    id <ISSPlatformApp> app = [ShareSDK getClientWithType:(ShareType)platType];
    return  [app isClientInstalled] ? true : false ;
}


#endif