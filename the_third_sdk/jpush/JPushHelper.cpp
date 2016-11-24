//
//  JPushHelper.cpp
//  AppGift
//
//  Created by tanjie on 15/7/29.
//
//

#include "JPushHelper.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <string>
#include "SDKCommon.h"

#define JPushHelperPath "com/zpt/libjpush/JPushHelper"

using namespace CSJson;

string JPushHelper::NOTI_PATH_SUFFIX = "ca_remote_notis.json";
string JPushHelper::NOTI_INDEX_FLAG_PATH = "ca_remote_noti_index.json";
string JPushHelper::NOTI_KEY_NOTI = "notis";
string JPushHelper::NOTI_KEY_INDEX = "index";
string JPushHelper::NOTI_KEY_CONTENT = "content";

void JPushHelper::initJPush(const std::string &uId, const std::set<std::string>& tags) {
    JniMethodInfo mi;
    
    JniHelper::getMethodInfo(mi
                             , "java/util/HashSet"
                             , "<init>"
                             , "(I)V");
    
    jobject javaSet = mi.env->NewObject(mi.classID, mi.methodID, tags.size());
    
    JniHelper::getMethodInfo(mi
                             , "java/util/HashSet"
                             , "add"
                             , "(Ljava/lang/Object;)Z");

    
    for (std::set<std::string>::iterator it = tags.begin(); it != tags.end(); it++) {
        std::string tag = *it;
        mi.env->CallBooleanMethod(javaSet, mi.methodID, mi.env->NewStringUTF(tag.c_str()));
    }
    
    if (!JniHelper::getStaticMethodInfo(mi, JPushHelperPath, "initUser", "(Ljava/lang/String;Ljava/util/HashSet;)V")) {
        assert(0);
        return;
    }
    
    jstring juId = mi.env->NewStringUTF(str_replace(uId.c_str(), "-", ""));
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, juId, javaSet);
    mi.env->DeleteLocalRef(mi.classID);
}

void JPushHelper::checkNewNotis() {
    string notiPath = CCFileUtils::sharedFileUtils()->getWritablePath() + string(NOTI_PATH_SUFFIX);
    string indexPath = CCFileUtils::sharedFileUtils()->getWritablePath() + string(NOTI_INDEX_FLAG_PATH);
    
    CCLog("---notiPath:[%s]", notiPath.c_str());
    
    if (CCFileUtils::sharedFileUtils()->isFileExist(notiPath)) {
        string content = CCFileUtils::sharedFileUtils()->getFileString(notiPath.c_str());
        CSJson::Value dict;
        CSJson::Reader cReader;
        
        int indexFlag = 0;
        if (CCFileUtils::sharedFileUtils()->isFileExist(indexPath)) {
            indexFlag = atoi(CCFileUtils::sharedFileUtils()->getFileString(indexPath.c_str()).c_str());
        }
        
        string indexContent = CCFileUtils::sharedFileUtils()->getFileString(notiPath.c_str());
        
        if (cReader.parse(content, dict, false)) {
            Value notis = dict[NOTI_KEY_NOTI];
            
            if (!notis.isArray()) {
                return;
            }
            
            // 遍历一遍把新增的noti创建并移动flag标记
            int i = notis.size() - 1;
            while (i > 0 && notis[i][NOTI_KEY_INDEX].asInt() < indexFlag) {
                // todo: 创建noti
                CCLog("---create noti: %d, %s", notis[i][NOTI_KEY_INDEX].asInt(), notis[i][NOTI_KEY_CONTENT].toStyledString().c_str());
                i--;
            }
            
            int lastIndex = notis[notis.size() - 1][NOTI_KEY_INDEX].asInt();
            if (notis.size() > 0 && lastIndex > indexFlag) {
                indexFlag = lastIndex;
                CCFileUtils::sharedFileUtils()->saveFile(indexPath, CCString::createWithFormat("%d", indexFlag)->getCString());
            }
        }
    }
}
#endif
