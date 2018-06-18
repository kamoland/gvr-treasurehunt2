/*
 */

#ifndef DDCONTROLLER_SRC_MAIN_JNI_DDCONTROLLER_H_  // NOLINT
#define DDCONTROLLER_SRC_MAIN_JNI_DDCONTROLLER_H_

#include <android/asset_manager.h>
#include <GLES2/gl2.h>
#include <jni.h>

#include <array>
#include <chrono>  // NOLINT
#include <memory>
#include <vector>

#include "vr/gvr/capi/include/gvr.h"
#include "vr/gvr/capi/include/gvr_controller.h"

class DaydreamController {
 public:
  DaydreamController(JNIEnv* env, jlong gvr_context_ptr);
  ~DaydreamController();
  void OnResume();
  void OnPause();
  void UpdateController();
  void GetControllerOrientation(float *quatAry);
  void GetControllerYawPitchRoll(float *yprAry);
  int32_t GetAllButtonsState();

 private:
  // Gvr API entry point.
  gvr_context* gvr_context_;
  std::unique_ptr<gvr::GvrApi> gvr_api_;
  bool gvr_api_initialized_;

  // Controller API entry point.
  std::unique_ptr<gvr::ControllerApi> controller_api_;

  // The last controller state (updated once per frame).
  gvr::ControllerState controller_state_;

  // Disallow copy and assign.
  DaydreamController(const DaydreamController& other) = delete;
  DaydreamController& operator=(const DaydreamController& other) = delete;
};

#endif  // DDCONTROLLER_SRC_MAIN_JNI_DDCONTROLLER_H_  // NOLINT
