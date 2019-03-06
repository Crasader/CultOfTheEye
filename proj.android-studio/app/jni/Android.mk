LOCAL_PATH := $(call my-dir)
LOCAL_SHORT_COMMANDS=true

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
				   $(LOCAL_PATH)/../../../Classes/Audio.cpp \
				   $(LOCAL_PATH)/../../../Classes/BallistaController.cpp \
				   $(LOCAL_PATH)/../../../Classes/CollisionHandler.cpp \
				   $(LOCAL_PATH)/../../../Classes/ControlsScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/Core.cpp \
				   $(LOCAL_PATH)/../../../Classes/CreditsScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/Debris.cpp \
				   $(LOCAL_PATH)/../../../Classes/EndScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/GameScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/GameScene_Loading.cpp \
				   $(LOCAL_PATH)/../../../Classes/GameScene_UserInput.cpp \
				   $(LOCAL_PATH)/../../../Classes/Gem.cpp \
				   $(LOCAL_PATH)/../../../Classes/Helpers.cpp \
				   $(LOCAL_PATH)/../../../Classes/Monster.cpp \
				   $(LOCAL_PATH)/../../../Classes/MonsterBody.cpp \
				   $(LOCAL_PATH)/../../../Classes/MonsterHead.cpp \
				   $(LOCAL_PATH)/../../../Classes/MonsterManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/PhysicsShapeCache.cpp \
				   $(LOCAL_PATH)/../../../Classes/Projectile.cpp \
				   $(LOCAL_PATH)/../../../Classes/ResourceManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/RNG.cpp \
				   $(LOCAL_PATH)/../../../Classes/Tentacle.cpp \
				   $(LOCAL_PATH)/../../../Classes/TitleScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/Tower.cpp \
				   $(LOCAL_PATH)/../../../Classes/TowerManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/UIButton.cpp \
				   $(LOCAL_PATH)/../../../Classes/UITower.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
