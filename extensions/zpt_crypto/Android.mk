LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := Zpt_crypto_static
LOCAL_MODULE_FILENAME := libzptcrypto

LOCAL_SRC_FILES := \
CACrypto.cpp \
base64/libbase64.c \
xxtea/xxtea.c \
#md5/md5.c

LOCAL_EXPORT_C_INCLUDES := \
$(LOCAL_PATH) \
$(LOCAL_PATH)/xxtea \
$(LOCAL_PATH)/md5 \
$(LOCAL_PATH)/base64

LOCAL_CFLAGS := -fexceptions

LOCAL_WHOLE_STATIC_LIBRARIES := CrossApp_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,CrossApp)
