//
//  ShareHelper.mm
//  AppGift
//
//  Created by tanjie on 15/5/28.
//
//

#include "ShareHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <ShareSDK/ShareSDK.h>
#import <AGCommon/CMRegexKitLite.h>
#import <FacebookConnection/ISSFacebookApp.h>
//#import "WXApi.h"
//#import "WeiboApi.h"
//#import <TencentOpenAPI/QQApiInterface.h>
//#import <TencentOpenAPI/TencentOAuth.h>

#include "SDKCommon.h"

//static UIView *_refView = nil;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;
using namespace cn::sharesdk;

/**
 *	@brief	转换分享内容
 *
 *	@param 	content 	内容字典结构
 *
 *	@return	分享内容对象
 */
id<ISSContent> convertPublishContent(CSJsonDictionary& content) {
    NSString *message = nil;
    id<ISSCAttachment> image = nil;
    NSString *title = nil;
    NSString *url = nil;
    NSString *desc = nil;
    SSPublishContentMediaType type = SSPublishContentMediaTypeText;
    
    CCLOG("content:[%s]", content.getStyledDescription().c_str());
    
    if (!content.isEmpty()) {
        message = [NSString stringWithUTF8String:content.getItemStringValue(Share_content).c_str()];
    
        NSString *imagePath = [NSString stringWithUTF8String:content.getItemStringValue(Share_image).c_str()];
        if ([imagePath isMatchedByRegex:@"\\w://.*"]) {
            image = [ShareSDK imageWithUrl:imagePath];
        } else {
            image = [ShareSDK imageWithPath:imagePath];
        }
    
        title = [NSString stringWithUTF8String:content.getItemStringValue(Share_title).c_str()];
        
        url = [NSString stringWithUTF8String:content.getItemStringValue(Share_url).c_str()];
        desc = [NSString stringWithUTF8String:content.getItemStringValue(Share_desc).c_str()];
        type = (SSPublishContentMediaType)(content.getItemIntValue(Share_type, 0));
    }
    
    id<ISSContent> contentObj =  [ShareSDK content:message
                                    defaultContent:message
                                             image:image
                                             title:title
                                               url:url
                                       description:desc
                                         mediaType:type];
    
    if (!content.isEmpty()) {
        NSString *siteUrlStr = [NSString stringWithUTF8String:content.getItemStringValue(Share_siteUrl).c_str()];
        NSString *siteStr = [NSString stringWithUTF8String:content.getItemStringValue(Share_site).c_str()];
        
        if ((siteUrlStr && siteUrlStr.length > 0) || (siteStr && siteStr.length > 0)) {
            [contentObj addQQSpaceUnitWithTitle:INHERIT_VALUE
                                            url:INHERIT_VALUE
                                           site:siteStr
                                        fromUrl:siteUrlStr
                                        comment:INHERIT_VALUE
                                        summary:INHERIT_VALUE
                                          image:INHERIT_VALUE
                                           type:INHERIT_VALUE
                                        playUrl:INHERIT_VALUE
                                           nswb:INHERIT_VALUE];
        }
        
        NSString *extInfoStr = [NSString stringWithUTF8String:content.getItemStringValue(Share_extInfo).c_str()];
        NSString *musicUrlStr = [NSString stringWithUTF8String:content.getItemStringValue(Share_musicUrl).c_str()];

        if (extInfoStr || musicUrlStr) {
            [contentObj addWeixinSessionUnitWithType:INHERIT_VALUE
                                             content:INHERIT_VALUE
                                               title:INHERIT_VALUE
                                                 url:INHERIT_VALUE
                                               image:INHERIT_VALUE
                                        musicFileUrl:musicUrlStr
                                             extInfo:extInfoStr
                                            fileData:INHERIT_VALUE
                                        emoticonData:INHERIT_VALUE];
            
            [contentObj addWeixinTimelineUnitWithType:INHERIT_VALUE
                                              content:INHERIT_VALUE
                                                title:INHERIT_VALUE
                                                  url:INHERIT_VALUE
                                                image:INHERIT_VALUE
                                         musicFileUrl:musicUrlStr
                                              extInfo:extInfoStr
                                             fileData:INHERIT_VALUE
                                         emoticonData:INHERIT_VALUE];
        }
    }
    
    return contentObj;
}

void ShareHelper::open(const std::string& appKey, bool useAppTrusteeship) {
    NSString *appKeyStr = [NSString stringWithCString:appKey.c_str() encoding:NSUTF8StringEncoding];
    [ShareSDK registerApp:appKeyStr useAppTrusteeship:useAppTrusteeship];
    //激活SSO
    [ShareSDK ssoEnabled:YES];
}

bool ShareHelper::close() {
    return true;
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

//bool ShareHelper::isClientInstalled(cn::sharesdk::C2DXPlatType platType) {
//    id <ISSPlatformApp> app = [ShareSDK getClientWithType:(ShareType)platType];
//    return  [app isClientInstalled] ? true : false ;
//}

void ShareHelper::initIOS()
{
    //开启Facebook网页授权开关(optional). todo: fb审核需要关闭
    id<ISSFacebookApp> facebookApp =(id<ISSFacebookApp>)[ShareSDK getClientWithType:ShareTypeFacebook];
    [facebookApp setIsAllowWebAuthorize:YES];
    
//    //导入微信类型
//    [ShareSDK importWeChatClass:[WXApi class]];
//    
//    //导入腾讯微博类型
//    [ShareSDK importTencentWeiboClass:[WeiboApi class]];
//    
//    //导入QQ类型
//    [ShareSDK importQQClass:[QQApiInterface class] tencentOAuthCls:[TencentOAuth class]];
}

#endif