#pragma once

#include <fake-jni/jvm.h>

namespace Baron::Interface {
 class JvmtiInterface : public FakeJni::JvmtiInterface {
 public:
  using FakeJni::JvmtiInterface::JvmtiInterface;
 };
}