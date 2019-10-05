#include "baron/impl/interface/invoke.h"

namespace Baron::Interface {
 jint InvokeInterface::getEnv(FakeJni::Jvm * vm, void ** penv, jint version) const {
  return FakeJni::InvokeInterface::getEnv(vm, penv, version);
 }

 jint InvokeInterface::destroyJavaVm(FakeJni::Jvm * vm) const {
  return FakeJni::InvokeInterface::destroyJavaVm(vm);
 }
}