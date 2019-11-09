#include "baron/util/util.h"

#define _MODIFIER_CASE(clazz, modifier) \
if ((modifiers & clazz::modifier) == clazz::modifier) {\
 smods.append(#modifier);\
}

#include <fake-jni/jvm.h>

namespace Baron {
 std::string deserializeClassModifiers(uint32_t modifiers) {
  std::string smods;
  _MODIFIER_CASE(FakeJni::JClass, PUBLIC)
  _MODIFIER_CASE(FakeJni::JClass, PRIVATE)
  _MODIFIER_CASE(FakeJni::JClass, PROTECTED)
  _MODIFIER_CASE(FakeJni::JClass, STATIC)
  _MODIFIER_CASE(FakeJni::JClass, FINAL)
  _MODIFIER_CASE(FakeJni::JClass, INTERFACE)
  _MODIFIER_CASE(FakeJni::JClass, ABSTRACT)
  _MODIFIER_CASE(FakeJni::JClass, SYNTHETIC)
  _MODIFIER_CASE(FakeJni::JClass, ANNOTATION)
  _MODIFIER_CASE(FakeJni::JClass, ENUM)
  return smods;
 }

 std::string deserializeFieldModifiers(uint32_t modifiers) {
  std::string smods;
  _MODIFIER_CASE(FakeJni::JMethodID, PUBLIC)
  _MODIFIER_CASE(FakeJni::JMethodID, PRIVATE)
  _MODIFIER_CASE(FakeJni::JMethodID, PROTECTED)
  _MODIFIER_CASE(FakeJni::JMethodID, STATIC)
  _MODIFIER_CASE(FakeJni::JMethodID, FINAL)
  _MODIFIER_CASE(FakeJni::JMethodID, SYNCHRONIZED)
  _MODIFIER_CASE(FakeJni::JMethodID, BRIDGE)
  _MODIFIER_CASE(FakeJni::JMethodID, VARARGS)
  _MODIFIER_CASE(FakeJni::JMethodID, NATIVE)
  _MODIFIER_CASE(FakeJni::JMethodID, ABSTRACT)
  _MODIFIER_CASE(FakeJni::JMethodID, STRICT)
  _MODIFIER_CASE(FakeJni::JMethodID, SYNTHETIC)
  return smods;
 }

 std::string deserializeMethodModifiers(uint32_t modifiers) {
  std::string smods;
  _MODIFIER_CASE(FakeJni::JFieldID, PUBLIC)
  _MODIFIER_CASE(FakeJni::JFieldID, PRIVATE)
  _MODIFIER_CASE(FakeJni::JFieldID, PROTECTED)
  _MODIFIER_CASE(FakeJni::JFieldID, STATIC)
  _MODIFIER_CASE(FakeJni::JFieldID, FINAL)
  _MODIFIER_CASE(FakeJni::JFieldID, VOLATILE)
  _MODIFIER_CASE(FakeJni::JFieldID, TRANSIENT)
  _MODIFIER_CASE(FakeJni::JFieldID, SYNTHETIC)
  _MODIFIER_CASE(FakeJni::JFieldID, ENUM)
  return smods;
 }
}