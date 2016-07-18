#include "ShareSDKUtils.h"

#if 1
#define  LOG_TAG    "ShareSDKUtils"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...) 
#endif

#ifdef __cplusplus
extern "C" {
#endif

C2DXAuthResultEvent authCb;
C2DXGetUserInfoResultEvent infoCb;
C2DXShareResultEvent shareCb;
C2DXAddFriendResultEvent followFriendCb;
C2DXGetFriendsResultEvent getFriendsCb;


JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onJavaCallback
  (JNIEnv * env, jclass thiz, jstring resp) {
	const char* ccResp = env->GetStringUTFChars(resp, JNI_FALSE);
	//CCLog("ccResp = %s", ccResp);
	C2DXDictionary* dic = C2DXDictionary::create(ccResp);
	env->ReleaseStringUTFChars(resp, ccResp);
	int status = dic->getItemIntValue("status", 0); // Success = 1, Fail = 2, Cancel = 3
	int action = dic->getItemIntValue("action", 0); //  1 = ACTION_AUTHORIZING,  8 = ACTION_USER_INFOR,9 = ACTION_SHARE
	int platform = dic->getItemIntValue("platform", 0);
	C2DXDictionary* res = dic->getSubDictionary("res");
	int reqID = dic->getItemIntValue("reqID", 0);

	// TODO add codes here
	if(1 == status){
		callBackComplete(reqID, action, platform, res);
	}else if(2 == status){
		callBackError(reqID, action, platform, res);
	}else{
		callBackCancel(reqID, action, platform, res);
	}
	
	dic->release();
}

void callBackComplete(int reqID, int action, int platformId, C2DXDictionary* res){
	//CCLog("complete callback");
	if (action == 1 && NULL != authCb) { // 1 = ACTION_AUTHORIZING
		authCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, NULL);
	} else if (action == 8 && NULL != infoCb) { // 8 = ACTION_USER_INFOR 
		infoCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, res);
	} else if (action == 9 && NULL != shareCb) { // 9 = ACTION_SHARE
		shareCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, NULL);
	} else if (action == 6 && NULL != followFriendCb) { // 6 = FOLLOW_FRIEND
		followFriendCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, res);
	} else if (action == 2 && NULL != getFriendsCb){ // 2 = ACTION_GET_FRIEND_LIST
		getFriendsCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, res);
	}
}

void callBackError(int reqID, int action, int platformId, C2DXDictionary* res){
	if (action == 1 && NULL != authCb) { // 1 = ACTION_AUTHORIZING
		authCb(reqID, C2DXResponseStateFail, (C2DXPlatType) platformId, res);
	} else if (action == 8 && NULL != infoCb) { // 8 = ACTION_USER_INFOR 2 = ACTION_GET_FRIEND_LIST
		infoCb(reqID, C2DXResponseStateFail, (C2DXPlatType) platformId, res);
	} else if (action == 9 && NULL != shareCb) { // 9 = ACTION_SHARE
		shareCb(reqID, C2DXResponseStateFail, (C2DXPlatType) platformId, res);
	} else if (action == 6 && NULL != followFriendCb) { // 6 = FOLLOW_FRIEND
		followFriendCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, res);
	} else if (action == 2 && NULL != getFriendsCb){ // 2 = ACTION_GET_FRIEND_LIST
		getFriendsCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, res);
	}
}

void callBackCancel(int reqID, int action, int platformId, C2DXDictionary* res){
	if (action == 1 && NULL != authCb) { // 1 = ACTION_AUTHORIZING
		authCb(reqID, C2DXResponseStateCancel, (C2DXPlatType) platformId, NULL);
	} else if (action == 8 && NULL != infoCb) { // 8 = ACTION_USER_INFOR 2 = ACTION_GET_FRIEND_LIST
		infoCb(reqID, C2DXResponseStateCancel, (C2DXPlatType) platformId, NULL);
	} else if (action == 9 && NULL != shareCb) { // 9 = ACTION_SHARE
		shareCb(reqID, C2DXResponseStateCancel, (C2DXPlatType) platformId, NULL);
	} else if (action == 6 && NULL != followFriendCb) { // 6 = FOLLOW_FRIEND
		followFriendCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, NULL);
	} else if (action == 2 && NULL != getFriendsCb){ // 2 = ACTION_GET_FRIEND_LIST
		getFriendsCb(reqID, C2DXResponseStateSuccess, (C2DXPlatType) platformId, NULL);
	}
}

bool getMethod(JniMethodInfo &mi, const char *methodName, const char *paramCode) {
	return JniHelper::getStaticMethodInfo(mi, "cn/sharesdk/ShareSDKUtils", methodName, paramCode);
}

void releaseMethod(JniMethodInfo &mi) {
	if(mi.classID != NULL)
		mi.env->DeleteLocalRef(mi.classID);
}

bool registerAppAndSetPlatformConfigJNI(const char* appKey, C2DXDictionary *platformInfos) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "initSDKAndSetPlatfromConfig", "(Ljava/lang/String;Ljava/lang/String;)V");
	if (!isHave) {
		return false;
	}
	
	jstring jInfo = NULL;
	jstring jAppKey = NULL;
	if (platformInfos != NULL) {
		const char* ccInfo = platformInfos->getDescription().c_str();
		jInfo = mi.env->NewStringUTF(ccInfo);
	}
	jAppKey = mi.env->NewStringUTF(appKey);

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, jAppKey, jInfo);
	releaseMethod(mi);
	return true;
}

bool authorizeJNI(int reqID, int platformId, C2DXAuthResultEvent callback) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "authorize", "(II)V");
	if (!isHave) {
		return false;
	}

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, reqID, platformId);
	releaseMethod(mi);
	authCb = callback;
	return true;
}

bool cancelAuthorizeJNI(int platformId) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "removeAccount", "(I)V");
	if (!isHave) {
		return false;
	}

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, platformId);
	releaseMethod(mi);
	return true;
}

bool isAuthorizedValidJNI(int platformId) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "isAuthValid", "(I)Z");
	if (!isHave) {
		return false;
	}

	jboolean valid = mi.env->CallStaticBooleanMethod(mi.classID, mi.methodID, platformId);
	releaseMethod(mi);
	return valid == JNI_TRUE;
}

bool isClientValidJNI(int platformId) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "isClientValid", "(I)Z");
	if (!isHave) {
		return false;
	}

	jboolean valid = mi.env->CallStaticBooleanMethod(mi.classID, mi.methodID, platformId);
	releaseMethod(mi);
	return valid == JNI_TRUE;
}

bool getUserInfoJNI(int reqID, int platformId, C2DXGetUserInfoResultEvent callback){
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "showUser", "(II)V");
	if (!isHave) {
		return false;
	}
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, reqID, platformId);
	releaseMethod(mi);
	infoCb = callback;
	return true;
}

bool addFriendJNI(int reqID, int platformId, const char* account, C2DXAddFriendResultEvent callback){
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "followFriend", "(IILjava/lang/String;)V");
	if(!isHave){
		return false;
	}
	jstring jContent = mi.env->NewStringUTF(account);
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, reqID, platformId, jContent);
	releaseMethod(mi);
	followFriendCb = callback;

	return true;
}

C2DXDictionary* getAuthInfoJNI(int platformId){
	JniMethodInfo mi;
	C2DXDictionary* dic;
	bool isHave = getMethod(mi, "getAuthInfo", "(I)Ljava/lang/String;");
	jstring userInfo = (jstring) mi.env->CallStaticObjectMethod(mi.classID, mi.methodID, platformId);
	const char* ccResp = mi.env->GetStringUTFChars(userInfo, JNI_FALSE);
	CCLog("userInfo = %s", ccResp);
	dic = C2DXDictionary::create(ccResp);
	releaseMethod(mi);

	return dic;
}

bool shareContentJNI(int reqID, const C2DXPlatforms& platTypes, C2DXDictionary *content, C2DXShareResultEvent callback){
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "shareContent", "(IILjava/lang/String;)V");
	if (!isHave) {
		return false;
	}

	const char* ccContent = content->getDescription().c_str();
	jstring jContent = mi.env->NewStringUTF(ccContent);
	// free(ccContent);

	for (int i = 0; i < platTypes.size(); ++i)
	{
		mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, reqID, platTypes[i], jContent);
	}

	releaseMethod(mi);
	shareCb = callback;
	return true;
}

bool getFriendListJNI(int reqID, int platformId, int count, int page, C2DXGetFriendsResultEvent callback){
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "getFriendList", "(IIII)V");
	if (!isHave) {
		return false;
	}
	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, reqID, platformId, count, page);
	releaseMethod(mi);
	infoCb = callback;
	return true;
}

bool onekeyShareJNI(int reqID, int platformId, C2DXDictionary *content, C2DXShareResultEvent callback) {
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "onekeyShare", "(IILjava/lang/String;)V");
	if (!isHave) {
		return false;
	}

	const char* ccContent = content->getDescription().c_str();
	jstring jContent = mi.env->NewStringUTF(ccContent);

	mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, reqID, platformId, jContent);
	releaseMethod(mi);
	
	shareCb = callback;
	return true;
}

void toastShowJNI(const char *msg) {
    JniMethodInfo mi;
    bool isHave = getMethod(mi, "toast", "(Ljava/lang/String;)V");
	if (!isHave) {
		return;
	}
	
    jstring jContent = mi.env->NewStringUTF(msg);

    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, jContent);
    releaseMethod(mi);

    return;
}

#ifdef __cplusplus
}
#endif
