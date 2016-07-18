LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := The_third_sdk_static

LOCAL_MODULE_FILENAME := libthe_third_sdk

LOCAL_WHOLE_STATIC_LIBRARIES := CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_extension_static

LOCAL_SRC_FILES := share/C2DXShareSDK/Android/ShareSDKUtils.cpp \
				share/C2DXShareSDK/C2DXShareSDK.cpp \
				ads/AdHelper.cpp \
				umeng/UMHelper.cpp \
				common/ZPTNativeHelper.cpp \
				jpush/JPushHelper.cpp \
				localNotification/ZPTLocalNotification.cpp \
				localNotification/android/ZPTLocalNotification_Android.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/share/C2DXShareSDK \
				$(LOCAL_PATH)/share/C2DXShareSDK/Android \
				$(LOCAL_PATH)/ads \
				$(LOCAL_PATH)/umeng \
				$(LOCAL_PATH)/common \
				$(LOCAL_PATH)/jpush \
				$(LOCAL_PATH)/localNotification \
				$(LOCAL_PATH)/localNotification/android \
				$(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES = $(LOCAL_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions
                    
include $(BUILD_STATIC_LIBRARY)

$(call import-module,extensions)
$(call import-module,CrossApp)
