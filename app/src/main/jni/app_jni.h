/*
 */

#ifndef DDCONTROLLER_APP_SRC_MAIN_JNI_APP_JNI_H_  // NOLINT
#define DDCONTROLLER_APP_SRC_MAIN_JNI_APP_JNI_H_

#include <jni.h>
#include <stdint.h>

#define NATIVE_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL                 \
      Java_com_kamoland_util_DaydreamController_##method_name

extern "C" {

NATIVE_METHOD(jlong, nativeOnCreate)
(JNIEnv* env, jobject obj, jlong gvrContextPtr);
NATIVE_METHOD(void, nativeOnResume)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr);
NATIVE_METHOD(void, nativeOnPause)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr);
NATIVE_METHOD(void, nativeOnDestroy)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr);
NATIVE_METHOD(void, nativeGetControllerOrientation)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr, jfloatArray quatAry);
NATIVE_METHOD(void, nativeUpdateController)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr);
NATIVE_METHOD(void, nativeGetControllerYawPitchRoll)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr, jfloatArray ypr);
NATIVE_METHOD(jint, nativeGetAllButtonsState)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr);

}

#endif  // DDCONTROLLER_APP_SRC_MAIN_JNI_APP_JNI_H_  // NOLINT
