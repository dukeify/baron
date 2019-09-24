#pragma once

#include <fake-jni/jvm.h>

namespace Baron {
 namespace Interface {
  class InvokeInterface : public FakeJni::InvokeInterface {
  public:
   using FakeJni::InvokeInterface::InvokeInterface;
  };

  class NativeInterface : public FakeJni::NativeInterface {
  public:
   using FakeJni::NativeInterface::NativeInterface;
  };

  class JvmtiInterfacee : public FakeJni::JvmtiInterface {
  public:
   using FakeJni::JvmtiInterface::JvmtiInterface;
  };
 }

 namespace Env {
  class JniEnv : public FakeJni::JniEnv {
  public:
   using FakeJni::JniEnv::JniEnv;
  };

  class JvmtiEnv : public FakeJni::JvmtiEnv {
  public:
   using FakeJni::JvmtiEnv::JvmtiEnv;
  };
 }

 class Jvm : public FakeJni::Jvm {
 public:
  explicit Jvm(FILE * log = stdout);
 };
}