#include "ShareSDKUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

C2DXAuthResultEvent authCb;
C2DXGetUserInfoResultEvent infoCb;
C2DXShareResultEvent shareCb;

JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onJavaCallback(JNIEnv * env, jclass thiz, jstring resp) {
	CCJSONConverter* json = CCJSONConverter::sharedConverter();
	const char* ccResp = env->GetStringUTFChars(resp, JNI_FALSE);
	CCLOG("ccResp = %s", ccResp);
	CCDictionary* dic = json->dictionaryFrom(ccResp);
	env->ReleaseStringUTFChars(resp, ccResp);
	CCNumber* status = dynamic_cast<CCNumber*>(dic->objectForKey("status")); // Success = 1, Fail = 2, Cancel = 3
	CCNumber* action = dynamic_cast<CCNumber*>(dic->objectForKey("action")); //  1 = ACTION_AUTHORIZING,  8 = ACTION_USER_INFOR,9 = ACTION_SHARE
	CCNumber* platform = dynamic_cast<CCNumber*>(dic->objectForKey("platform"));
	CCDictionary* res = (CCDictionary*) dic->objectForKey("res");

	// TODO add codes here
	if (!status || !action || !platform) {
		dic->release();
		assert(0);
		return;
	}

	int statusInt = status->getIntValue();
	int actionInt = action->getIntValue();
	int platformInt = platform->getIntValue();

	if (1 == statusInt) {
		callBackComplete(actionInt, platformInt, res);
	} else if (2 == statusInt) {
		callBackError(actionInt, platformInt, res);
	} else {
		callBackCancel(actionInt, platformInt, res);
	}

	dic->release();
}

void callBackComplete(int action, int platformId, CCDictionary* res) {
	if (action == 1 && NULL != authCb) { // 1 = ACTION_AUTHORIZING
		authCb(C2DXResponseStateSuccess, (C2DXPlatType) platformId, NULL);
	} else if (action == 8 && NULL != infoCb) { // 8 = ACTION_USER_INFOR
		infoCb(C2DXResponseStateSuccess, (C2DXPlatType) platformId, res, NULL);
	} else if (action == 9 && NULL != shareCb) { // 9 = ACTION_SHARE
		shareCb(C2DXResponseStateSuccess, (C2DXPlatType) platformId, res, NULL);
	}
}

void callBackError(int action, int platformId, CCDictionary* res) {
	if (action == 1 && NULL != authCb) { // 1 = ACTION_AUTHORIZING
		authCb(C2DXResponseStateFail, (C2DXPlatType) platformId, NULL);
	} else if (action == 8 && NULL != infoCb) { // 8 = ACTION_USER_INFOR
		infoCb(C2DXResponseStateFail, (C2DXPlatType) platformId, res, NULL);
	} else if (action == 9 && NULL != shareCb) { // 9 = ACTION_SHARE
		shareCb(C2DXResponseStateFail, (C2DXPlatType) platformId, res, NULL);
	}
}

void callBackCancel(int action, int platformId, CCDictionary* res) {
	if (action == 1 && NULL != authCb) { // 1 = ACTION_AUTHORIZING
		authCb(C2DXResponseStateCancel, (C2DXPlatType) platformId, NULL);
	} else if (action == 8 && NULL != infoCb) { // 8 = ACTION_USER_INFOR
		infoCb(C2DXResponseStateCancel, (C2DXPlatType) platformId, res, NULL);
	} else if (action == 9 && NULL != shareCb) { // 9 = ACTION_SHARE
		shareCb(C2DXResponseStateCancel, (C2DXPlatType) platformId, res, NULL);
	}
}

bool initShareSDK(const char* appKey, bool useAppTrusteeship) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "initSDK", "(Ljava/lang/String;Z)V");
	if (!isHave) {
		return false;
	}

	jstring appKeyStr = mi.env->NewStringUTF(appKey);
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, appKeyStr,
			useAppTrusteeship);
	releaseMethod(mi);
	return true;
}

bool getMethod(JniMethodInfo &mi, const char *methodName,
		const char *paramCode) {
	return JniHelper::getStaticMethodInfo(mi, "cn/sharesdk/ShareSDKUtils",
			methodName, paramCode);
}

void releaseMethod(JniMethodInfo &mi) {
	mi.env->DeleteLocalRef(mi.classID);
}

bool stopSDK() {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "stopSDK", "()V");
	if (!isHave) {
		return false;
	}

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
	releaseMethod(mi);
	return true;
}

bool setPlatformDevInfo(int platformId, CCDictionary *info) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "setPlatformConfig", "(ILjava/lang/String;)V");
	if (!isHave) {
		return false;
	}

	CCJSONConverter* json = CCJSONConverter::sharedConverter();
	const char* ccInfo = json->strFrom(info);
	jstring jInfo = mi.env->NewStringUTF(ccInfo);
	// free(ccInfo);

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId, jInfo);
	releaseMethod(mi);
	return true;
}

bool doAuthorize(int platformId, C2DXAuthResultEvent callback) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "authorize", "(I)V");
	if (!isHave) {
		return false;
	}

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId);
	releaseMethod(mi);
	authCb = callback;
	return true;
}

bool removeAccount(int platformId) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "removeAccount", "(I)V");
	if (!isHave) {
		return false;
	}

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId);
	releaseMethod(mi);
	return true;
}

bool isValid(int platformId) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "isValid", "(I)Z");
	if (!isHave) {
		return false;
	}

	jboolean valid = mi.env->CallStaticBooleanMethod(mi.classID, mi.methodID,
			platformId);
	releaseMethod(mi);
	return valid == JNI_TRUE;
}

bool showUser(int platformId, C2DXGetUserInfoResultEvent callback) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "showUser", "(I)V");
	if (!isHave) {
		return false;
	}
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId);
	releaseMethod(mi);
	infoCb = callback;
	return true;
}

bool doShare(int platformId, CCDictionary *content,
		C2DXShareResultEvent callback) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "share", "(ILjava/lang/String;)V");
	if (!isHave) {
		return false;
	}

	CCJSONConverter* json = CCJSONConverter::sharedConverter();
	const char* ccContent = json->strFrom(content);
	jstring jContent = mi.env->NewStringUTF(ccContent);
	// free(ccContent);

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId, jContent);
	releaseMethod(mi);
	shareCb = callback;
	return true;
}

bool multiShare(CCArray *platTypes, CCDictionary *content,
		C2DXShareResultEvent callback) {
	int index = 0;
	int count = platTypes->count();
	while (index < count) {
		CCNumber* item = dynamic_cast<CCNumber*>(platTypes->objectAtIndex(index));
		int platformId = item->getIntValue();
		doShare(platformId, content, callback);
		index++;
	}
	return true;
}

bool onekeyShare(int platformId, CCDictionary *content,
		C2DXShareResultEvent callback) {
	JniMethodInfo mi;
	if (platformId > 0) {
		bool isHave = getMethod(mi, "onekeyShare", "(ILjava/lang/String;)V");
		if (!isHave) {
			return false;
		}
	} else {
		bool isHave = getMethod(mi, "onekeyShare", "(Ljava/lang/String;)V");
		if (!isHave) {
			return false;
		}
	}

	CCJSONConverter* json = CCJSONConverter::sharedConverter();
	const char* ccContent = json->strFrom(content);
	jstring jContent = mi.env->NewStringUTF(ccContent);
	// free(ccContent);

	if (platformId > 0) {
		mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId,
				jContent);
	} else {
		mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, jContent);
	}
	releaseMethod(mi);

	shareCb = callback;
	return true;
}

#ifdef __cplusplus
}
#endif