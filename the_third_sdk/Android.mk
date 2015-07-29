LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := The_third_sdk_static

LOCAL_MODULE_FILENAME := libthe_third_sdk

LOCAL_WHOLE_STATIC_LIBRARIES := CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_extension_static

LOCAL_SRC_FILES := share/ShareHelper.cpp \
				ads/AdHelper.cpp \
				umeng/UMHelper.cpp \
				common/ZPTNativeHelper.cpp \
				jpush/JPushHelper.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/share \
				$(LOCAL_PATH)/ads \
				$(LOCAL_PATH)/umeng \
				$(LOCAL_PATH)/common \
				$(LOCAL_PATH)/jpush

LOCAL_EXPORT_C_INCLUDES = $(LOCAL_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions
                    
include $(BUILD_STATIC_LIBRARY)

$(call import-module,extensions)
$(call import-module,CrossApp)
