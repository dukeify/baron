#pragma once

#include <fake-jni/jvm.h>

namespace Baron::Interface {
 class InvokeInterface : public FakeJni::InvokeInterface {
 public:
  using FakeJni::InvokeInterface::InvokeInterface;

  //impl/interface/invoke/misc.cpp
  virtual jint getEnv(FakeJni::Jvm *vm, void **penv, jint version) const override;
  virtual jint destroyJavaVm(FakeJni::Jvm *vm) const override;

  //impl/interface/invoke/thread.cpp
  virtual jint attachCurrentThread(FakeJni::Jvm *vm, void **penv, void *args) const override;
  virtual jint detachCurrentThread(FakeJni::Jvm *vm) const override;
  virtual jint attachCurrentThreadAsDaemon(FakeJni::Jvm *vm, void **penv, void *args) const override;
 };
}