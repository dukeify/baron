#pragma once

#include "baron/impl/jvm.h"

namespace Baron::Internal {
 class JClass : public FakeJni::JClass {
 public:
  using FakeJni::JClass::JClass;

  virtual ~JClass() = default;

  bool registerMethod(const FakeJni::JMethodID * mid, bool deallocate = true) const override;
//  bool unregisterMethod(const FakeJni::JMethodID * mid) const noexcept override;
  bool registerField(FakeJni::JFieldID * fid, bool deallocate = true) const noexcept override;
//  bool unregisterField(FakeJni::JFieldID * fid) const noexcept override;
  JObject * newInstance(const JavaVM * vm, const char * signature, CX::va_list_t& list) const override;
  JObject * newInstance(const JavaVM * vm, const char * signature, const jvalue * values) const override;
 };
}