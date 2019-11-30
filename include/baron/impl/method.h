#pragma once

#include "baron/impl/jvm.h"

namespace Baron::Internal {
 class JMethodID : public FakeJni::JMethodID {
 public:
  using FakeJni::JMethodID::JMethodID;

  jvalue virtualInvoke(const JavaVM * vm, void * clazzOrObj, CX::va_list_t& list) const override;
  jvalue virtualInvoke(const JavaVM * vm, void * clazzOrObj, const jvalue * args) const override;
 };
}