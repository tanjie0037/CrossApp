//
//  ShareHelper.h
//  AppGift
//
//  Created by tanjie on 15/5/28.
//
//

#ifndef __AppGift__ShareHelper__
#define __AppGift__ShareHelper__

#include <stdio.h>
#include <string>
#include <vector>
#include "CrossApp.h"
#include "CrossAppExt.h"
#include "ShareSDKTypeDef.h"

class ShareHelper
{
public:
    
    /**
     *	@brief	初始化ShareSDK
     *
     *	@param 	appKey 	应用Key
     *	@param 	useAppTrusteeship 	是否使用应用信息托管
     */
    static void open(const std::string& appKey, bool useAppTrusteeship);
    
    /**
     *	@brief	关闭ShareSDK
     */
    static bool close();
    
    /**
     *	@brief	设置平台配置信息
     *
     *	@param 	platType 	平台类型
     *	@param 	configInfo 	配置信息
     */
    static void setPlatformConfig(cn::sharesdk::C2DXPlatType platType, CSJsonDictionary& configInfo);
    
    /**
     *	@brief	用户授权
     *
     *	@param 	platType 	平台类型
     *	@param 	callback 	回调方法
     */
    static void authorize(cn::sharesdk::C2DXPlatType platType, cn::sharesdk::C2DXAuthResultEvent callback);
    
    /**
     *	@brief	取消用户授权
     *
     *	@param 	platType 	平台类型
     */
    static void cancelAuthorize(cn::sharesdk::C2DXPlatType platType);
    
    /**
     *	@brief	用户是否授权
     *
     *	@param 	platType 	平台类型
     *
     *	@return	true 已授权， false 尚未授权
     */
    static bool hasAutorized(cn::sharesdk::C2DXPlatType platType);
    
    /**
     *	@brief	获取用户信息
     *
     *	@param 	platType 	平台类型
     *	@param 	callback 	回调方法
     */
    static void getUserInfo(cn::sharesdk::C2DXPlatType platType, cn::sharesdk::C2DXGetUserInfoResultEvent callback);
    
    /**
     *	@brief	分享内容
     *
     *	@param 	platType 	平台类型
     *	@param 	content 	分享内容
     *	@param 	callback 	回调方法
     */
    static void shareContent(cn::sharesdk::C2DXPlatType platType, CSJsonDictionary &content, cn::sharesdk::C2DXShareResultEvent callback);
    
    /**
     *	@brief	一键分享内容
     *
     *	@param 	platTypes 	平台类型列表
     *	@param 	content 	分享内容
     *	@param 	callback 	回调方法
     */
    static void oneKeyShareContent(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback);
    
    /**
     *	@brief	显示分享菜单
     *
     *	@param 	platTypes 	平台类型列表
     *	@param 	content 	分享内容
     *	@param 	callback 	回调方法
     */
    static void showShareMenu(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback);
    
    /**
     *	@brief	显示分享菜单
     *
     *	@param 	platTypes 	平台类型列表
     *	@param 	content 	分享内容
     *	@param 	pt          弹出分享菜单参位置，仅用于设置iPad分享菜单弹出
     *  @param  direction   弹出分享菜单指向，仅用于设置iPad分享菜单弹出
     *	@param 	callback 	回调方法
     */
    static void showShareMenu(const std::vector<cn::sharesdk::C2DXPlatType>& platTypes, CSJsonDictionary& content, CrossApp::CCPoint pt, cn::sharesdk::C2DXMenuArrowDirection direction, cn::sharesdk::C2DXShareResultEvent callback);
    
    
    /**
     *	@brief	显示分享视图
     *
     *	@param 	platType 	平台类型
     *	@param 	content 	分享内容
     *	@param 	callback 	回调方法
     */
    static void showShareView(cn::sharesdk::C2DXPlatType platType, CSJsonDictionary& content, cn::sharesdk::C2DXShareResultEvent callback);
    
    /**
     *	@brief	检测是否安装客户端
     *
     *	@param 	platType 	平台类型
     *
     *	@return true 安装有客户端, false 没有安装客户端
     */
//    static bool isClientInstalled(cn::sharesdk::C2DXPlatType platType);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static void initIOS();
#endif
};

#endif /* defined(__AppGift__ShareHelper__) */
