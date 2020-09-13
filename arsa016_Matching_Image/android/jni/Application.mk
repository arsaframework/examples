
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
# APP_STL := stlport_static 

# all
# armeabi
# armeabi-v7a
# mip
# x86
APP_ABI := armeabi-v7a

# use this to select gcc instead of clang
#NDK_TOOLCHAIN_VERSION := 4.9
# OR use this to select the latest clang version:
NDK_TOOLCHAIN_VERSION := clang

# !!!MUST equal android:minSdkVersion in AndroidManifest.xml!!!
#APP_PLATFORM:=android-19