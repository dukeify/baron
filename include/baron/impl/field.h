#pragma once

#include <cstring>

#include "baron/impl/jvm.h"

namespace Baron::Internal {
 class JFieldID : public FakeJni::JFieldID {
 public:
  using FakeJni::JFieldID::JFieldID;

  jvalue get(const JavaVM * vm, FakeJni::JObject * obj) const override;
  void set(const JavaVM * vm, FakeJni::JObject * obj, void * value) const override;

 private:
  const FakeJni::JClass * resolveType(const Jvm * vm) const {
   const auto signature = getSignature();
   for (auto& clz : vm->getClasses()) {
    if (!strcmp(signature, clz->getName())) {
     return clz;
    }
   }
   return nullptr;
  }
 };
}