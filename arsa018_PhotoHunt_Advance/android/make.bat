@echo off
ECHO Building by ARSA Androidc... bin/arsa018_PhotoHunt_Advance.apk
set JAVA_HOME=C:\Program Files (x86)/Java/jdk1.8.0_102
set ANDROID_HOME=C:\Users\Wing64/Local Settings/Application Data/Android/android-sdk
set PATH=%PATH%C:\Program Files (x86)/Java/jdk1.8.0_102/bin
IF /I NOT %1_ EQU apk_ (
C:/android-ndk-r12b/ndk-build %1
 )
IF /I %1_ EQU apk_ (
IF EXIST "bin" (rmdir /s /q bin)
IF EXIST "gen" (rmdir /s /q gen)
"C:/apache-ant-1.9.7/bin/ant" release
"C:\Program Files (x86)/Java/jdk1.8.0_102/bin/jarsigner" -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore arsa.keystore -storepass 123456 bin/arsa018_PhotoHunt_Advance-release-unsigned.apk arsa
"C:\Program Files (x86)/Java/jdk1.8.0_102/bin/jarsigner" -verify bin/arsa018_PhotoHunt_Advance-release-unsigned.apk
zipalign -v 4 bin/arsa018_PhotoHunt_Advance-release-unsigned.apk bin/arsa018_PhotoHunt_Advance.apk
ECHO Checking device...
"C:\Users\Wing64/Local Settings/Application Data/Android/android-sdk/platform-tools/adb.exe" devices
ECHO Installing... bin/arsa018_PhotoHunt_Advance.apk
"C:\Users\Wing64/Local Settings/Application Data/Android/android-sdk/platform-tools/adb.exe" install -r -d bin/arsa018_PhotoHunt_Advance.apk
"C:\Users\Wing64/Local Settings/Application Data/Android/android-sdk/platform-tools/adb.exe" shell am start arsa.framework.arsa018_PhotoHunt_Advance/arsa.framework.arsa018_PhotoHunt_Advance.arsa100
"C:\Users\Wing64/Local Settings/Application Data/Android/android-sdk/platform-tools/adb.exe" kill-server
 )
