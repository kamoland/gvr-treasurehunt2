/*
 */

#include "ddcontroller.h"  // NOLINT

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>
#include <string>

#include "utils.h"  // NOLINT

namespace {

// Prediction time to use when estimating head pose.
static const int64_t kPredictionTimeWithoutVsyncNanos = 50000000;  // 50ms

}  // namespace

DaydreamController::DaydreamController(JNIEnv* env, jlong gvr_context_ptr)
    :  // This is the GVR context pointer obtained from Java:
      gvr_context_(reinterpret_cast<gvr_context*>(gvr_context_ptr)),
      // Wrap the gvr_context* into a GvrApi C++ object for convenience:
      gvr_api_(gvr::GvrApi::WrapNonOwned(gvr_context_)),
      gvr_api_initialized_(false)
{
  LOGD("DaydreamController initialized.");
}

DaydreamController::~DaydreamController() {
  LOGD("DaydreamController shutdown.");
}

void DaydreamController::OnResume() {
  LOGD("DaydreamController::OnResume");
  LOGD("Initializing ControllerApi.");
  controller_api_.reset(new gvr::ControllerApi);
  CHECK(controller_api_);

  int32_t options = gvr::ControllerApi::DefaultOptions();
  options |= GVR_CONTROLLER_ENABLE_GYRO;
  options |= GVR_CONTROLLER_ENABLE_ACCEL;
  // ARMモデルを許可する
  options |= GVR_CONTROLLER_ENABLE_ARM_MODEL;

  CHECK(controller_api_->Init(options, gvr_context_));

  if (controller_api_) {
      controller_api_->Resume();
  }

  gvr_api_initialized_ = true;

  LOGD("Initialized ControllerApi.");
}

void DaydreamController::OnPause() {
  LOGD("DaydreamController::OnPause");
  if (gvr_api_initialized_) gvr_api_->PauseTracking();
  if (controller_api_) controller_api_->Pause();
}

void DaydreamController::UpdateController() {
  if (!gvr_api_initialized_) {
    return;
  }
  gvr::ClockTimePoint pred_time = gvr::GvrApi::GetTimePointNow();
  pred_time.monotonic_system_time_nanos += kPredictionTimeWithoutVsyncNanos;

  gvr::Mat4f head_view = gvr_api_->GetHeadSpaceFromStartSpaceTransform(pred_time);

  // ARMモデルを適用する
  controller_api_->ApplyArmModel(GVR_CONTROLLER_RIGHT_HANDED, GVR_ARM_MODEL_FOLLOW_GAZE, head_view);

  controller_state_.Update(*controller_api_);
}

void DaydreamController::GetControllerOrientation(float *quatAry) {
  gvr::ControllerQuat q = controller_state_.GetOrientation();
  quatAry[0] = q.qx;
  quatAry[1] = q.qy;
  quatAry[2] = q.qz;
  quatAry[3] = q.qw;
}

void DaydreamController::GetControllerYawPitchRoll(float *yprAry) {
  gvr::ControllerQuat q = controller_state_.GetOrientation();

  float sp = -2.0f * (q.qy * q.qz - q.qw * q.qx);

  // ジンバルロックをチェックする
  if (fabs(sp) > 0.9999f) {
    // 真上か真下を向いている
    // ピッチ
    yprAry[1] = 3.1415727f / 2 * sp;
    // ヘディングを計算し，バンクを0に設定する
    yprAry[0] = atan2(-q.qx * q.qz + q.qw * q.qy, 0.5f - q.qy * q.qy - q.qz * q.qz);
    yprAry[2] = 0.0f;
  } else {
    // 角度を計算する
    // ピッチ
    yprAry[1] = asin(sp);
    // ヘディング
    yprAry[0] = atan2(q.qx * q.qz + q.qw * q.qy, 0.5f - q.qx * q.qx - q.qy * q.qy);
    // バンク
    yprAry[2] = atan2(q.qx * q.qy + q.qw * q.qz, 0.5f - q.qx * q.qx - q.qz * q.qz);
  }
}

int32_t DaydreamController::GetAllButtonsState() {
  return controller_state_.GetButtonState(GVR_CONTROLLER_BUTTON_CLICK)
        | (controller_state_.GetButtonState(GVR_CONTROLLER_BUTTON_HOME) << 2)
          | (controller_state_.GetButtonState(GVR_CONTROLLER_BUTTON_APP) << 3)
        ;
}
