#include "baron/impl/interface/native.h"

namespace Baron::Interface {
 NativeInterface::NativeInterface(const FakeJni::Jvm& vm) :
  FakeJni::NativeInterface(vm),
  vm(dynamic_cast<Jvm&>(const_cast<FakeJni::Jvm &>(vm)))
 {}
}