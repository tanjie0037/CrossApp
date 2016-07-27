//
//  C2DXiOSShareSDK.cpp
//  HelloWorldDemo
//
//  Created by 刘 靖煌 on 15-11-27.
//  Copyright © 2015年 mob.com. All rights reserved.
//

#include "C2DXiOSShareSDK.h"

#import <ShareSDK/ShareSDK.h>
#import <ShareSDKConnector/ShareSDKConnector.h>
#import <ShareSDKUI/ShareSDKUI.h>
#import <ShareSDKExtension/ShareSDK+Extension.h>
#import <ShareSDKExtension/SSEShareHelper.h>
#import <ShareSDK/SSDKFriendsPaging.h>
#import <MOBFoundation/MOBFRegex.h>
#import <ShareSDK/NSMutableDictionary+SSDKShare.h>

#import <ShareSDK/ShareSDK+Base.h>
#include "SDKCommon.h"

//#define IMPORT_SINA_WEIBO_LIB               //导入新浪微博库，如果不需要新浪微博客户端分享可以注释此行
//#define IMPORT_QZONE_QQ_LIB                 //导入腾讯开发平台库，如果不需要QQ空间分享、SSO或者QQ好友分享可以注释此行
//#define IMPORT_RENREN_LIB                   //导入人人库，如果不需要人人SSO，可以注释此行
//#define IMPORT_WECHAT_LIB                   //导入微信库，如果不需要微信分享可以注释此行
//#define IMPORT_ALIPAY_LIB                   //导入支付宝分享库，如果不需要支付宝分享可以注释此行
#define IMPORT_KAKAO_LIB                    //导入Kakao库，如果不需要Kakao分享可以注释此行

#ifdef IMPORT_SINA_WEIBO_LIB
#import "WeiboSDK.h"
#endif

#ifdef IMPORT_QZONE_QQ_LIB
#import <TencentOpenAPI/TencentOAuth.h>
#import <TencentOpenAPI/QQApiInterface.h>
#endif

#ifdef IMPORT_RENREN_LIB
#import <RennSDK/RennSDK.h>
#endif

#ifdef IMPORT_WECHAT_LIB
#import "WXApi.h"
#endif

#ifdef IMPORT_ALIPAY_LIB
#import "APOpenAPI.h"
#endif

#ifdef IMPORT_KAKAO_LIB
#import <KakaoOpenSDK/KakaoOpenSDK.h>
#endif

static UIView *_refView = nil;

using namespace cn::sharesdk;

#pragma mark - tools

/**
 *	@brief	转换NSDictionary为C2DXDictionary类型
 *
 *	@param 	dict 	字典
 *
 *	@return	字典类型
 */
C2DXDictionary* convertNSDictToCCDict(NSDictionary *dict);

/**
 *	@brief	转换C2DXDictionary为NSDictionary类型
 *
 *	@param 	dict 	字典
 *
 *	@return	字典类型
 */
NSMutableDictionary* convertC2DXDictionaryToNSDictionary(C2DXDictionary *Dictionary);

C2DXDictionary* convertNSDictToCCDict(NSDictionary *dict)
{
    NSError *parseError = nil;
    
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:0 error:&parseError];
    
    if (parseError) {
        return NULL;
    }
    
    C2DXDictionary* cDict = C2DXDictionary::create(utf8cstr([[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding]));
    return cDict;
}

NSMutableDictionary * convertC2DXDictionaryToNSDictionary(C2DXDictionary * Dictionary)
{
    string dicStr = Dictionary->getDescription();
    
    if (dicStr.length() == 0) {
        NSLog(@"json解析失败");
        return nil;
    }
    
    NSData *jsonData = [nsstr(dicStr.c_str()) dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err = nil;
    NSMutableDictionary *nsDict = [NSJSONSerialization JSONObjectWithData:jsonData
                                                        options:NSJSONReadingMutableContainers
                                                          error:&err];
    if(err) {
        NSLog(@"json解析失败：%@",err);
        return nil;
    }
    
    return nsDict;
}

#pragma mark - ShareSDK 接口
#pragma mark 注册

void C2DXiOSShareSDK::registerAppAndSetPlatformConfig(const char *appKey, C2DXDictionary *configInfo)
{
    NSString* appKeyStr = [NSString stringWithCString:appKey
                                             encoding:NSUTF8StringEncoding];
    NSMutableDictionary* platformsDic = [NSMutableDictionary dictionary];
    
    if(configInfo)
    {
        platformsDic = convertC2DXDictionaryToNSDictionary(configInfo);
    }
    
    NSArray *platforms = [platformsDic allKeys];
    NSMutableArray *activePlatforms = [NSMutableArray array];
    
    for (id platformStr in platforms)
    {
        if ([platformStr isKindOfClass:[NSString class]])
        {
            [activePlatforms addObject:@([platformStr integerValue])];
        }
    }
    
    [ShareSDK registerApp:appKeyStr
          activePlatforms:activePlatforms
                 onImport:^(SSDKPlatformType platformType)
                 {
                     switch (platformType)
                     {
#ifdef IMPORT_SINA_WEIBO_LIB
                         case SSDKPlatformTypeSinaWeibo:
                             [ShareSDKConnector connectWeibo:[WeiboSDK class]];
                             break;
#endif
                             
#ifdef IMPORT_QZONE_QQ_LIB
                         case SSDKPlatformTypeQQ:
                             [ShareSDKConnector connectQQ:[QQApiInterface class]
                                        tencentOAuthClass:[TencentOAuth class]];
                             break;
#endif
                             
#ifdef IMPORT_RENREN_LIB
                         case SSDKPlatformTypeRenren:
                             [ShareSDKConnector connectRenren:[RennClient class]];
                             break;
#endif
                             
#ifdef IMPORT_WECHAT_LIB
                         case SSDKPlatformTypeWechat:
                             [ShareSDKConnector connectWeChat:[WXApi class]];
                             break;
#endif
                             
#ifdef IMPORT_ALIPAY_LIB
                         case SSDKPlatformTypeAliPaySocial:
                             [ShareSDKConnector connectAliPaySocial:[APOpenAPI class]];
                             break;
#endif
                             
#ifdef IMPORT_KAKAO_LIB
                         case SSDKPlatformTypeKakao:
                             [ShareSDKConnector connectKaKao:[KOSession class]];
                             break;
#endif
             
                         default:
                         break;
                     }
                 }
    onConfiguration:^(SSDKPlatformType platformType, NSMutableDictionary *appInfo)
    {
        NSMutableDictionary * dict = [platformsDic objectForKey:[NSString stringWithFormat:@"%zi",platformType]];
        
        [appInfo addEntriesFromDictionary:dict];
        [appInfo setObject:@"both" forKey:@"auth"];
        
    }];
}

id convertPublishContent(C2DXDictionary *content)
{
    NSString *text = nil;
    NSString *title = nil;
    NSString *url = nil;
    SSDKImage *thumbImg = nil;
    SSDKImage *image = nil;
    
    NSString *musicFileUrl = nil;
    NSString *extInfo = nil;
    NSData *fileData = nil;
    NSData *emoData = nil;
    SSDKContentType contentType = SSDKContentTypeAuto;
    C2DXContentType temContentType = C2DXContentTypeText;
    
    NSMutableDictionary *shareContentPara = [NSMutableDictionary dictionary];
    
    if (content)
    {
        //text
        const char* textStr = content->getItemCStringValue("text");
        if (textStr) {
            text = nsstr(textStr);
        }
        
        //title
        const char* titleStr = content->getItemCStringValue("title");
        if (titleStr > 0) {
            title = nsstr(titleStr);
        }
        
        //url
        const char* urlStr = content->getItemCStringValue("url");
        if (urlStr) {
            url = nsstr(urlStr);
        }
        
        //thumbImg
        const char* thumbImagePath = content->getItemCStringValue("thumbImg");
        if (thumbImagePath)
        {
            NSString *imgPath = nsstr(thumbImagePath);
            
            if ([MOBFRegex isMatchedByRegex:@"\\w://.*"
                                    options:MOBFRegexOptionsNoOptions
                                    inRange:NSMakeRange(0, 10)
                                 withString:imgPath])
            {
                thumbImg = [[SSDKImage alloc]initWithURL:[NSURL URLWithString:imgPath]];
            }else
            {
                thumbImg = [[SSDKImage alloc] initWithImage:[UIImage imageNamed:imgPath]
                                                     format:SSDKImageFormatJpeg settings:nil];
            }
        }
        
        //image
        const char *imagePath = content->getItemCStringValue("image");
        if (imagePath)
        {
            NSString *imgPath = nsstr(imagePath);
            
            if ([MOBFRegex isMatchedByRegex:@"\\w://.*"
                                    options:MOBFRegexOptionsNoOptions
                                    inRange:NSMakeRange(0, 10)
                                 withString:imgPath])
            {
                image = [[SSDKImage alloc]initWithURL:[NSURL URLWithString:imgPath]];
            }else
            {
                image = [[SSDKImage alloc] initWithImage:[UIImage imageNamed:imgPath]
                                                     format:SSDKImageFormatJpeg settings:nil];
            }
        }
        
        //Music url
        const char *musicUrl = content->getItemCStringValue("musicFileUrl");
        if (musicUrl)
        {
            musicFileUrl = nsstr(musicUrl);
        }
        
        //extInfo
        const char *extInfoUrl = content->getItemCStringValue("extInfo");
        if (extInfoUrl)
        {
            extInfo = nsstr(extInfoUrl);
        }
        
        //fileData
        const char *fileDataStr = content->getItemCStringValue("fileData");
        
        if (fileDataStr)
        {
            fileData = [NSData dataWithContentsOfFile: nsstr(fileDataStr)];
        }
        
        //emoticonData
        const char *emoticonDataStr = content->getItemCStringValue("emoticonData");
        
        if (emoticonDataStr)
        {
            emoData = [NSData dataWithContentsOfFile: nsstr(emoticonDataStr)];
        }
        
        //type
        int typeValue = content->getItemIntValue("type", C2DXContentTypeAuto);
        if (typeValue)
        {
            temContentType = (C2DXContentType)typeValue;
            
            switch (temContentType)
            {
                case C2DXContentTypeText:
                    contentType = (SSDKContentType)SSDKContentTypeText;
                    break;
                case C2DXContentTypeImage:
                case C2DXContentTypeEmoji:
                    contentType = (SSDKContentType)SSDKContentTypeImage;
                    break;
                case C2DXContentTypeWebPage:
                    contentType = (SSDKContentType)SSDKContentTypeWebPage;
                    break;
                case C2DXContentTypeMusic:
                    contentType = (SSDKContentType)SSDKContentTypeAudio;
                    break;
                case C2DXContentTypeVideo:
                    contentType = (SSDKContentType)SSDKContentTypeVideo;
                    break;
                case C2DXContentTypeApp:
                    contentType = (SSDKContentType)SSDKContentTypeApp;
                    break;
                case C2DXContentTypeAuto:
                case C2DXContentTypeFile:
                    contentType = (SSDKContentType)SSDKContentTypeAuto;
                default:
                    break;
            }
        }
        
        [shareContentPara SSDKSetupShareParamsByText:text
                                              images:image
                                                 url:[NSURL URLWithString:url]
                                               title:title
                                                type:contentType];
        
        return shareContentPara;
    }
    
    return NULL;
}

#pragma mark 分享平台授权
void C2DXiOSShareSDK::authorize(int reqID, C2DXPlatType platType, C2DXAuthResultEvent callback)
{
    [ShareSDK authorize:(SSDKPlatformType)platType
               settings:nil
         onStateChanged:^(SSDKResponseState state, SSDKUser *user, NSError *error) {

             C2DXDictionary *userInfoDict = C2DXDictionary::create();
             switch (state)
             {
                 case SSDKResponseStateSuccess:
                     if (user)
                     {
                         userInfoDict = convertNSDictToCCDict([user rawData]);
                     }
                     
                     break;
                 case SSDKResponseStateFail:
                     
                     if (error)
                     {
                         NSInteger errCode = [error code];
                         NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                         
                         userInfoDict->insertItem("error_code", (int)errCode);
                         
                         if (errDesc)
                         {
                             userInfoDict->insertItem("error_msg", utf8cstr(errDesc));
                         }
                     }
                     break;
                     
                 default:
                     break;
             }
             
             if (callback)
             {
                 callback(reqID,(C2DXResponseState)state,platType,userInfoDict);
             }
    }];
}

#pragma mark 取消授权
void C2DXiOSShareSDK::cancelAuthorize(C2DXPlatType platType)
{
    [ShareSDK cancelAuthorize:(SSDKPlatformType)platType];
}

#pragma mark 用户是否授权
bool C2DXiOSShareSDK::hasAutorized(C2DXPlatType platType)
{
    return [ShareSDK hasAuthorized:(SSDKPlatformType)platType] ? true :false;
}

bool C2DXiOSShareSDK::isClientInstalled(C2DXPlatType platType)
{
    return [ShareSDK isClientInstalled:(SSDKPlatformType)platType];
}

#pragma mark 获取授权用户信息
void C2DXiOSShareSDK::getUserInfo(int reqID,C2DXPlatType platType, C2DXGetUserInfoResultEvent callback)
{
    [ShareSDK getUserInfo:(SSDKPlatformType)platType
           onStateChanged:^(SSDKResponseState state, SSDKUser *user, NSError *error)
    {
        C2DXDictionary *userInfoDict = C2DXDictionary::create();
        switch (state)
        {
            case SSDKResponseStateSuccess:
                userInfoDict = convertNSDictToCCDict([user rawData]);
                if ([[user credential] rawData])
                {
                    userInfoDict->insertItem("credential", convertNSDictToCCDict([[user credential] rawData]));
                }
                
                break;
            case SSDKResponseStateFail:
                
                if (error)
                {
                    NSInteger errCode = [error code];
                    NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                    
                    userInfoDict->insertItem("error_code", (int)errCode);
                    
                    if (errDesc)
                    {
                        userInfoDict->insertItem("error_msg", utf8cstr(errDesc));
                    }
                }
                break;
                
            default:
                break;
        }
        
        if(callback)
        {
             callback(reqID,(C2DXResponseState)state,platType,userInfoDict);
        }
    }];
}

#pragma mark 简单分享
void C2DXiOSShareSDK::shareContent(int reqID,C2DXPlatType platType, C2DXDictionary *content, C2DXShareResultEvent callback)
{
    NSMutableDictionary *parameters = convertPublishContent(content);
    [ShareSDK share:(SSDKPlatformType)platType
         parameters:parameters
     onStateChanged:^(SSDKResponseState state, NSDictionary *userData, SSDKContentEntity *contentEntity, NSError *error) {

         C2DXDictionary *userInfoDict = C2DXDictionary::create();
         switch (state)
         {
             case SSDKResponseStateSuccess:
                 userInfoDict = convertNSDictToCCDict(userData);
                 break;
             case SSDKResponseStateFail:
                 
                 if (error)
                 {
                     NSInteger errCode = [error code];
                     NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                     
                     userInfoDict->insertItem("error_code", (int)errCode);
                     
                     if (errDesc)
                     {
                         userInfoDict->insertItem("error_msg", utf8cstr(errDesc));
                     }
                 }
                 break;
                 
             default:
                 break;
         }
         
         if (callback)
         {
             callback(reqID,(C2DXResponseState)state,(C2DXPlatType)platType,userInfoDict);
         }
     }];
}

#pragma mark 一键分享
void C2DXiOSShareSDK::oneKeyShareContent(int reqID, const C2DXPlatforms& platTypes, C2DXDictionary *content, C2DXShareResultEvent callback)
{
    NSMutableArray *shareList = nil;
    if (platTypes.size() > 0)
    {
        shareList = [NSMutableArray array];
        for (int i = 0; i != platTypes.size(); i++)
        {
            [shareList addObject:[NSNumber numberWithInteger: platTypes[i]]];
        }
    }
    
    NSMutableDictionary *shareParams;
    //构造分享参数
    shareParams = convertPublishContent(content);
    [SSEShareHelper oneKeyShare:shareList
                     parameters:shareParams
                 onStateChanged:^(SSDKPlatformType platformType,
                                  SSDKResponseState state,
                                  NSDictionary *userData,
                                  SSDKContentEntity *contentEntity,
                                  NSError *error,
                                  BOOL end)
     {
         C2DXDictionary *userInfoDict = C2DXDictionary::create();
         
         switch (state)
         {
             case SSDKResponseStateSuccess:
                 userInfoDict = convertNSDictToCCDict(userData);
                 break;
             case SSDKResponseStateFail:
                 
                 if (error)
                 {
                     NSInteger errCode = [error code];
                     NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                     
                     userInfoDict->insertItem("error_code", (int)errCode);
                     
                     if (errDesc)
                     {
                         userInfoDict->insertItem("error_msg", utf8cstr(errDesc));
                     }
                 }
                 break;
                 
             default:
                 break;
         }
         
         if (callback)
         {
             callback(reqID,(C2DXResponseState)state,(C2DXPlatType)platformType,userInfoDict);
         }
    }];
}

#pragma mark 弹出分享菜单进行分享
void C2DXiOSShareSDK::showShareMenu(int reqID,const C2DXPlatforms& platTypes, C2DXDictionary *content, C2DXPoint pt, C2DXShareResultEvent callback)
{
    NSMutableArray *shareList = nil;
    if (platTypes.size() > 0)
    {
        shareList = [NSMutableArray array];
        for (int i = 0; i != platTypes.size(); i++)
        {
            [shareList addObject:[NSNumber numberWithInteger: platTypes[i]]];
        }
    }
    
    //设置iPad菜单位置
    //    pt = CCDirector::sharedDirector() -> convertToUI(pt);
    if (!_refView)
    {
        _refView = [[UIView alloc] initWithFrame:CGRectMake(pt.x, pt.y, 1, 1)];
    }
    _refView.frame = CGRectMake(pt.x, pt.y, 1, 1);
    [[UIApplication sharedApplication].keyWindow.rootViewController.view addSubview:_refView];
    
    NSMutableDictionary *shareParams;
    //构造分享参数
    shareParams = convertPublishContent(content);
    
    [ShareSDK showShareActionSheet:_refView
                             items:shareList
                       shareParams:shareParams
               onShareStateChanged:^(SSDKResponseState state,
                                     SSDKPlatformType platformType,
                                     NSDictionary *userData,
                                     SSDKContentEntity *contentEntity,
                                     NSError *error,
                                     BOOL end)
     {
         C2DXDictionary *userInfoDict = C2DXDictionary::create();
         
         switch (state)
         {
             case SSDKResponseStateSuccess:
                 userInfoDict = convertNSDictToCCDict(userData);
                 break;
             case SSDKResponseStateFail:
                 
                 if (error)
                 {
                     NSInteger errCode = [error code];
                     NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                     
                     userInfoDict->insertItem("error_code", (int)errCode);
                     
                     if (errDesc)
                     {
                         userInfoDict->insertItem("error_msg", utf8cstr(errDesc));
                     }
                 }
                 break;
             default:
                 break;
         }
        
         if (callback)
         {
             callback(reqID,(C2DXResponseState)state,(C2DXPlatType)platformType,userInfoDict);
         }
         
         if (_refView)
         {
             //移除视图
             [_refView removeFromSuperview];
         }
     }];
}

#pragma mark 弹出分享编辑框
void C2DXiOSShareSDK::showShareEditView(int reqID,C2DXPlatType platType, C2DXDictionary *content, C2DXShareResultEvent callback)
{
    NSMutableDictionary *shareParams;
    //构造分享参数
    shareParams = convertPublishContent(content);
    [ShareSDK showShareEditor:(SSDKPlatformType)platType
           otherPlatformTypes:nil
                  shareParams:shareParams
          onShareStateChanged:^(SSDKResponseState state,
                                SSDKPlatformType platformType,
                                NSDictionary *userData,
                                SSDKContentEntity *contentEntity,
                                NSError *error,
                                BOOL end)
    {
        C2DXDictionary *userInfoDict = C2DXDictionary::create();
        
        switch (state)
        {
            case SSDKResponseStateSuccess:
                userInfoDict = convertNSDictToCCDict(userData);
                break;
            case SSDKResponseStateFail:
                if (error)
                {
                    NSInteger errCode = [error code];
                    NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                    
                    userInfoDict->insertItem("error_code", (int)errCode);
                    
                    if (errDesc)
                    {
                        userInfoDict->insertItem("error_msg", utf8cstr(errDesc));
                    }
                }
                break;
            default:
                break;
        }
        
        if (callback)
        {
            callback(reqID,(C2DXResponseState)state,(C2DXPlatType)platformType,userInfoDict);
        }
    }];
}

void C2DXiOSShareSDK::getFriendList(int reqID,C2DXPlatType platType,int count, int page, C2DXAddFriendResultEvent callback)
{
    [ShareSDK getFriends:(SSDKPlatformType)platType
                  cursor:page
                    size:count
          onStateChanged:^(SSDKResponseState state, SSDKFriendsPaging *paging, NSError *error) {

              C2DXDictionary *userInfoDict = C2DXDictionary::create();
              NSMutableDictionary *useDic = [NSMutableDictionary dictionary];
              
              switch (state)
              {
                  case SSDKResponseStateSuccess:
                      if (paging.users)
                      {
                          [useDic setObject:paging.users forKey:@"data"];
                      }
                      break;
                  case SSDKResponseStateFail:
                      if (error)
                      {
                          NSInteger errCode = [error code];
                          NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                          
                          if (@(errCode))
                          {
                              [useDic setObject:@(errCode) forKey:@"error_code"];
                          }
                          
                          if (errDesc)
                          {
                              [useDic setObject:errDesc forKey:@"error_msg"];
                          }
                      }
                      break;
                  default:
                      break;
              }
              
              userInfoDict = convertNSDictToCCDict(useDic);
              
              if (callback)
              {
                  callback(reqID,(C2DXResponseState)state,(C2DXPlatType)platType,userInfoDict);
              }
          }];
}

void C2DXiOSShareSDK::addFriend(int reqID,C2DXPlatType platType,const char* account ,C2DXGetFriendsResultEvent callback)
{
    SSDKPlatformType type = (SSDKPlatformType)platType;
    SSDKUser *user = [[SSDKUser alloc] init];
    
    if (type == SSDKPlatformTypeTencentWeibo)
    {
        user.nickname = nsstr(account);
    }
    else
    {
        user.nickname = nsstr(account);
    }
    
    [ShareSDK addFriend:(SSDKPlatformType)platType
                   user:user
         onStateChanged:^(SSDKResponseState state, SSDKUser *user, NSError *error) {

             C2DXDictionary *userInfoDict = C2DXDictionary::create();;
             NSMutableDictionary *temDic = [NSMutableDictionary dictionary];
             
             if (user)
             {
                 [temDic setObject:user forKey:@"data"];
             }
             
             if (error)
             {
                 NSInteger errCode = [error code];
                 NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                 
                 [temDic setObject:@(errCode) forKey:@"error_code"];
                 [temDic setObject:errDesc forKey:@"error_msg"];
             }
             userInfoDict = convertNSDictToCCDict(temDic);
             
             if (callback)
             {
                 callback(reqID,(C2DXResponseState)state,(C2DXPlatType)platType,userInfoDict);
             }
         }];
}

void C2DXiOSShareSDK::alertLog(const char *msg)
{
//    NSString *message = convertC2DXStringToNSString((C2DXString *)msg);
//    
//    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@""
//                                                        message:message
//                                                       delegate:nil
//                                              cancelButtonTitle:@"OK"
//                                              otherButtonTitles:nil, nil];
//    [alertView show];
}

