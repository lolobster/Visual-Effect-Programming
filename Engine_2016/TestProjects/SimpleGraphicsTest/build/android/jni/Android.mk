# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#



LOCAL_PATH := $(call my-dir)

ENGINE_PATH := $(LOCAL_PATH)/../../../../../Engine

include $(CLEAR_VARS)

LOCAL_MODULE    := native-activity

LOCAL_CFLAGS := -DANDROID -frtti -fexceptions

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../../../Engine/include \
	$(LOCAL_PATH)/../../../../../Engine/include/Framework/Android \
	$(LOCAL_PATH)/../../../include
	
LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/../../../../src/SpriteBatchScene.cpp \
	$(LOCAL_PATH)/../../../../src/CubeReflectionsScene.cpp \
	$(LOCAL_PATH)/../../../../src/RenderTextureScene.cpp \
	$(LOCAL_PATH)/../../../../src/AnimationScene2.cpp \
	$(LOCAL_PATH)/../../../../src/AnimationScene.cpp \
	$(LOCAL_PATH)/../../../../src/LoadMeshScene.cpp \
	$(LOCAL_PATH)/../../../../src/MeshScene.cpp \
	$(LOCAL_PATH)/../../../../src/NormalMappedMeshScene.cpp \
	$(LOCAL_PATH)/../../../../src/SceneGraphScene.cpp \
	$(LOCAL_PATH)/../../../../src/TexturedMeshScene.cpp \
	$(LOCAL_PATH)/../../../../src/main.cpp \
	$(LOCAL_PATH)/../../../../src/scene.cpp \
	$(LOCAL_PATH)/../../../../src/teapot.cpp


	
EXT_LIBRARY_PATH := $(LOCAL_PATH)/../../../../../Engine/build/android/obj/local/armeabi



LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 

LOCAL_STATIC_LIBRARIES := android_native_app_glue

LOCAL_LDLIBS    += -L$(EXT_LIBRARY_PATH) -lengine

include $(BUILD_SHARED_LIBRARY)



$(call import-module,android/native_app_glue)


