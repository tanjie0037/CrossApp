APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCROSSAPP_DEBUG=0 -std=c++11 -fsigned-char

APP_PLATFORM := android-9
APP_ABI := armeabi