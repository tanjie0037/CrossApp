LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := The_third_sdk_static

LOCAL_MODULE_FILENAME := libthe_third_sdk

LOCAL_SRC_FILES := share/C2DXShareSDK/C2DXShareSDK.cpp \
share/C2DXShareSDK/Android/ShareSDKUtils.cpp \
share/C2DXShareSDK/Android/JSON/CCJSONConverter.cpp \
share/C2DXShareSDK/Android/JSON/cJSON/cJSON.c

LOCAL_WHOLE_STATIC_LIBRARIES := CrossApp_static

LOCAL_C_INCLUDES := $(LOCAL_PATH)/share/C2DXShareSDK \
                           $(LOCAL_PATH)/share/C2DXShareSDK/Android \
                           $(LOCAL_PATH)/share/C2DXShareSDK/Android/JSON \
                           $(LOCAL_PATH)/share/C2DXShareSDK/Android/JSON/cJSON

LOCAL_EXPORT_C_INCLUDES = $(LOCAL_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions
                    
include $(BUILD_STATIC_LIBRARY)

$(call import-module,CrossApp)
