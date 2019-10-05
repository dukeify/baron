#pragma once

#include <fake-jni/jvm.h>

namespace Baron::Env {
 class JvmtiEnv : public FakeJni::JvmtiEnv {
 public:
  using FakeJni::JvmtiEnv::JvmtiEnv;
 };
}