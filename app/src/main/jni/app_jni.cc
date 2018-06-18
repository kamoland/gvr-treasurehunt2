/*
 */

#include "app_jni.h"  // NOLINT

#include <memory>

#include "ddcontroller.h"  // NOLINT
#include "utils.h"  // NOLINT

namespace {
inline jlong jptr(DaydreamController* ptr) { return reinterpret_cast<intptr_t>(ptr); }

inline DaydreamController* ptr(jlong jptr) { return reinterpret_cast<DaydreamController*>(jptr); }

}  // namespace

NATIVE_METHOD(jlong, nativeOnCreate)
(JNIEnv* env, jobject obj, jlong gvr_context_ptr) {
  return jptr(new DaydreamController(env, gvr_context_ptr));
}

NATIVE_METHOD(void, nativeOnResume)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr) {
  ptr(controller_paint_jptr)->OnResume();
}

NATIVE_METHOD(void, nativeOnPause)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr) {
  ptr(controller_paint_jptr)->OnPause();
}

NATIVE_METHOD(void, nativeOnDestroy)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr) {
  delete ptr(controller_paint_jptr);
}

NATIVE_METHOD(void, nativeUpdateController)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr) {
  ptr(controller_paint_jptr)->UpdateController();
}

NATIVE_METHOD(void, nativeGetControllerOrientation)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr, jfloatArray quatAry) {
  jfloat *buf = env->GetFloatArrayElements(quatAry, 0);
  ptr(controller_paint_jptr)->GetControllerOrientation(buf);
  env->ReleaseFloatArrayElements(quatAry, buf, 0);
}

NATIVE_METHOD(void, nativeGetControllerYawPitchRoll)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr, jfloatArray yprAry) {
  jfloat *buf = env->GetFloatArrayElements(yprAry, 0);
  ptr(controller_paint_jptr)->GetControllerYawPitchRoll(buf);
  env->ReleaseFloatArrayElements(yprAry, buf, 0);
}

NATIVE_METHOD(jint, nativeGetAllButtonsState)
(JNIEnv* env, jobject obj, jlong controller_paint_jptr) {
  return ptr(controller_paint_jptr)->GetAllButtonsState();
}
