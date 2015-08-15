# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in /Users/tanjie/Desktop/work/android/android-sdk/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}


-dontwarn retrofit.**
-dontwarn java.nio.file.*
-dontwarn org.codehaus.mojo.animal_sniffer.IgnoreJRERequirement

-keepattributes Signature
-keepattributes Exceptions

-keep class retrofit.** { *; }
-keep class com.squareup.okhttp.** { *; }
-keep class sun.misc.Unsafe { *; }
-keep class com.google.gson.stream.** { *; }
-keep class com.adscendmedia.sdk.rest.model.** { *; }
-keep class com.adscendmedia.sdk.rest.response.** { *; }
-keep interface com.squareup.okhttp.** { *; }