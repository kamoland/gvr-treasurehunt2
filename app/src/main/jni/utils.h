/*
 * Copyright 2017 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DDCONTROLLER_SRC_MAIN_JNI_UTILS_H_  // NOLINT
#define DDCONTROLLER_SRC_MAIN_JNI_UTILS_H_

#include <android/log.h>
#include <jni.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef __ANDROID__
  #define LOG_TAG "DdController"
  #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
  #define LOGW(...) \
      __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
  #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
  #define LOGD(...)
  #define LOGW(...)
  #define LOGE(...)
#endif  // #ifdef __ANDROID__

#define CHECK(condition) if (!(condition)) { \
        LOGE("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
        abort(); }
#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_NE(a, b) CHECK((a) != (b))


#endif  // DDCONTROLLER_SRC_MAIN_JNI_UTILS_H_  // NOLINT
