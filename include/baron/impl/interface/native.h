#pragma once

#include <fake-jni/jvm.h>
#include <cx/vararg.h>

#include "baron/impl/jvm.h"

namespace Baron::Interface {
 class NativeInterface : public FakeJni::NativeInterface {
 public:
  Jvm& vm;

  NativeInterface(const FakeJni::Jvm& vm);

  //inpl/interface/native/object.cpp
  jobject allocObject(jclass) const override;
  jobject newObjectV(jclass, jmethodID, CX::va_list_t&) const override;
  jobject newObjectA(jclass, jmethodID, const jvalue *) const override;
  jclass getObjectClass(jobject) const override;
  jboolean isInstanceOf(jobject, jclass) const override;
  //Currently unsupported by fake-jni
//  jclass defineClass(const char *, jobject, const jbyte *, jsize) const override;
  //Handled by baron jvm implementation
//  jclass findClass(const char *) const override;

  //inpl/interface/native/method.cpp
  jmethodID getMethodID(jclass, const char *, const char *) const override;
  jmethodID getStaticMethodID(jclass, const char *, const char *) const override;

  //inpl/interface/native/field.cpp
  jfieldID getFieldID(jclass, const char *, const char *) const override;
  jfieldID getStaticFieldID(jclass, const char *, const char *) const override;
 };
}