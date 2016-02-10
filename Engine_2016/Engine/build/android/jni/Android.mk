LOCAL_PATH:= $(call my-dir)

ENGINE_PATH := $(LOCAL_PATH)../../../..
ENGINE_SRC_PATH := ../../../src

include $(CLEAR_VARS)

LOCAL_MODULE    := engine

LOCAL_CFLAGS := -DANDROID -frtti -fexceptions

LOCAL_C_INCLUDES := \
	$(ENGINE_PATH)/include \
	$(ENGINE_PATH)/include/Framework/OpenGLES \
	$(ENGINE_PATH)/include/Framework/Android 


LOCAL_SRC_FILES := \
	$(ENGINE_SRC_PATH)/lua/lapi.c \
	$(ENGINE_SRC_PATH)/lua/lauxlib.c \
	$(ENGINE_SRC_PATH)/lua/lbaselib.c \
	$(ENGINE_SRC_PATH)/lua/lbitlib.c \
	$(ENGINE_SRC_PATH)/lua/lcode.c \
	$(ENGINE_SRC_PATH)/lua/lcorolib.c \
	$(ENGINE_SRC_PATH)/lua/lctype.c \
	$(ENGINE_SRC_PATH)/lua/ldblib.c \
	$(ENGINE_SRC_PATH)/lua/ldebug.c \
	$(ENGINE_SRC_PATH)/lua/ldo.c \
	$(ENGINE_SRC_PATH)/lua/ldump.c \
	$(ENGINE_SRC_PATH)/lua/lfunc.c \
	$(ENGINE_SRC_PATH)/lua/lgc.c \
	$(ENGINE_SRC_PATH)/lua/linit.c \
	$(ENGINE_SRC_PATH)/lua/liolib.c \
	$(ENGINE_SRC_PATH)/lua/llex.c \
	$(ENGINE_SRC_PATH)/lua/lmathlib.c \
	$(ENGINE_SRC_PATH)/lua/lmem.c \
	$(ENGINE_SRC_PATH)/lua/loadlib.c \
	$(ENGINE_SRC_PATH)/lua/lobject.c \
	$(ENGINE_SRC_PATH)/lua/lopcodes.c \
	$(ENGINE_SRC_PATH)/lua/loslib.c \
	$(ENGINE_SRC_PATH)/lua/lparser.c \
	$(ENGINE_SRC_PATH)/lua/lstate.c \
	$(ENGINE_SRC_PATH)/lua/lstring.c \
	$(ENGINE_SRC_PATH)/lua/lstrlib.c \
	$(ENGINE_SRC_PATH)/lua/ltable.c \
	$(ENGINE_SRC_PATH)/lua/ltablib.c \
	$(ENGINE_SRC_PATH)/lua/ltm.c \
	$(ENGINE_SRC_PATH)/lua/lua.c \
	$(ENGINE_SRC_PATH)/lua/luac.c \
	$(ENGINE_SRC_PATH)/lua/lundump.c \
	$(ENGINE_SRC_PATH)/lua/lvm.c \
	$(ENGINE_SRC_PATH)/lua/lzio.c \
	$(ENGINE_SRC_PATH)/ticpp/ticpp.cpp \
	$(ENGINE_SRC_PATH)/ticpp/tinystr.cpp \
	$(ENGINE_SRC_PATH)/ticpp/tinyxml.cpp \
	$(ENGINE_SRC_PATH)/ticpp/tinyxmlerror.cpp \
	$(ENGINE_SRC_PATH)/ticpp/tinyxmlparser.cpp \
	$(ENGINE_SRC_PATH)/allocator.cpp \
	$(ENGINE_SRC_PATH)/assert.cpp \
	$(ENGINE_SRC_PATH)/fixed_pool.cpp \
	$(ENGINE_SRC_PATH)/hashtable.cpp \
	$(ENGINE_SRC_PATH)/FileStream.cpp \
	$(ENGINE_SRC_PATH)/MemoryManager.cpp \
	$(ENGINE_SRC_PATH)/PropertySet.cpp \
	$(ENGINE_SRC_PATH)/Object.cpp \
	$(ENGINE_SRC_PATH)/red_black_tree.cpp \
	$(ENGINE_SRC_PATH)/string.cpp \
	$(ENGINE_SRC_PATH)/float_util.cpp \
	$(ENGINE_SRC_PATH)/intersect_util.cpp \
	$(ENGINE_SRC_PATH)/mat4.cpp \
	$(ENGINE_SRC_PATH)/quat.cpp \
	$(ENGINE_SRC_PATH)/random.cpp \
	$(ENGINE_SRC_PATH)/random_util.cpp \
	$(ENGINE_SRC_PATH)/runtime_checks.cpp \
	$(ENGINE_SRC_PATH)/slmath_lua.cpp \
	$(ENGINE_SRC_PATH)/vec2.cpp \
	$(ENGINE_SRC_PATH)/vec3.cpp \
	$(ENGINE_SRC_PATH)/vec4.cpp \
	$(ENGINE_SRC_PATH)/log.cpp \
	$(ENGINE_SRC_PATH)/ElapsedTimer.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmFontGLES.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmMesh.cpp \
	$(ENGINE_SRC_PATH)/Texture.cpp \
	$(ENGINE_SRC_PATH)/Entity.cpp \
	$(ENGINE_SRC_PATH)/AnimationTimeline.cpp \
	$(ENGINE_SRC_PATH)/SpriteBatch.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmPackedResourceGLES.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmResourceGLES.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmShader.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmUserInterfaceGLES.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmUtilsGLES.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/es_util.cpp\
	$(ENGINE_SRC_PATH)/OpenGLES/FrmFont.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmInput.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmPackedResource.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmUserInterface.cpp \
	$(ENGINE_SRC_PATH)/OpenGLES/FrmUtils.cpp \
	$(ENGINE_SRC_PATH)/Android/es_util_android.cpp \
	$(ENGINE_SRC_PATH)/Android/input_android.cpp \
	$(ENGINE_SRC_PATH)/Android/FrmStdlib_Platform.cpp \
	$(ENGINE_SRC_PATH)/Android/FrmUtils_Platform.cpp \
	$(ENGINE_SRC_PATH)/Android/FrmNoise_Platform.cpp 

	
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)


$(call import-module,android/native_app_glue)



