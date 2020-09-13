LOCAL_PATH := $(call my-dir)

#################################
# ARSA Framework include & lib folder
#################################
ARSA_INC_FOLDER = c:/arsa/include
ARSA_LIB_FOLDER = c:/arsa/lib

#################################
# current project source file
# this code must in project folder 
# because will add prefix laters
# if user don't need add prefix use MY_EXT_LOCAL_SRC_FILES instread (see below)
#################################
MY_LOCAL_SRC_FILES = ../../main.cpp \
	$(ARSA_INC_FOLDER)/SDL_android_main.c

#################################
# current project setting
# include, preprocessor
#################################
MY_INCLUDE = $(ARSA_INC_FOLDER) \
	$(ARSA_INC_FOLDER)/irrlicht \
	$(ARSA_INC_FOLDER)/cdx

MY_PREDEF = -O3 -DNDEBUG -fexpensive-optimizations -pipe -fno-exceptions -fno-rtti -fstrict-aliasing
MY_PREDEF += -D__ANDROID__ -D_IRR_STATIC_LIB_ -D_CRT_SECURE_NO_WARNINGS -D_IRR_COMPILE_WITH_SDL2_DEVICE_ -DHAVE_OPENGL -DPNG_NO_CONFIG_H  -DPNG_THREAD_UNSAFE_OK  -DPNG_NO_MMX_CODE  -DPNG_NO_MNG_FEATURES  -DHAVE_SYS_TYPES_H  -DHAVE_FCNTL_H  -DHAVE_ERRNO_H -DUNICODE -DGL_GLEXT_PROTOTYPES

#################################
# add prefix to c/c++ file
#################################
MY_OBJS = $(MY_EXT_LOCAL_SRC_FILES) $(MY_LOCAL_SRC_FILES) 

#################################
# arsa import static lib module
#################################
include $(CLEAR_VARS)
LOCAL_MODULE := Arsa
LOCAL_SRC_FILES := $(ARSA_LIB_FOLDER)/android/$(TARGET_ARCH_ABI)/libArsa.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(MY_INCLUDE)
LOCAL_CFLAGS += $(MY_PREDEF)
LOCAL_CXXFLAGS += -std=c++0x -Wno-switch -Wno-switch -Wno-parentheses-equality -Wno-c++11-extensions -Wno-ignored-attributes -Wno-format-security -Wno-implicit-exception-spec-mismatch -Wno-inconsistent-missing-override -Wno-extra-tokens -Wno-array-bounds -Wno-comment -Wno-tautological-constant-out-of-range-compare -Wno-tautological-compare -Wno-deprecated-register -Wno-logical-op-parentheses -Wno-writable-strings -Wno-unused-value -Wno-unused-variable -Wno-format -Wno-int-conversion -Wno-vexing-parse -Wno-unsequenced
LOCAL_SRC_FILES := $(MY_OBJS)

#################################
# current project module name
#################################
LOCAL_MODULE := arsa
LOCAL_STATIC_LIBRARIES := Arsa
LOCAL_LDLIBS := -lEGL -lGLESv1_CM -lGLESv2 -llog -landroid -lz  
include $(BUILD_SHARED_LIBRARY)

