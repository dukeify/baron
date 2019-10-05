#include "baron/impl/interface/invoke.h"

namespace Baron::Interface {
 jint InvokeInterface::attachCurrentThread(FakeJni::Jvm * vm, void ** penv, void * args) const {
  return FakeJni::InvokeInterface::attachCurrentThread(vm, penv, args);
 }

 jint InvokeInterface::detachCurrentThread(FakeJni::Jvm * vm) const {
  return FakeJni::InvokeInterface::detachCurrentThread(vm);
 }

 jint InvokeInterface::attachCurrentThreadAsDaemon(FakeJni::Jvm * vm, void ** penv, void * args) const {
  return FakeJni::InvokeInterface::attachCurrentThreadAsDaemon(vm, penv, args);
 }
}