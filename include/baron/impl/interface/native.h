#pragma once

#include <fake-jni/jvm.h>

namespace Baron::Interface {
 class NativeInterface : public FakeJni::NativeInterface {
 public:
  using FakeJni::NativeInterface::NativeInterface;

  //inpl/interface/native/object.cpp
//  virtual jboolean isSameObject(jobject, jobject) const override;
  virtual jobject allocObject(jclass) const override;
  virtual jobject newObjectV(jclass, jmethodID, va_list) const override;
  virtual jobject newObjectA(jclass, jmethodID, const jvalue *) const override;
  virtual jclass getObjectClass(jobject) const override;
  virtual jboolean isInstanceOf(jobject, jclass) const override;
  virtual jclass defineClass(const char *, jobject, const jbyte *, jsize) const override;
  virtual jclass findClass(const char *) const override;
  virtual jclass getSuperclass(jclass) const override;
  virtual jboolean isAssignableFrom(jclass, jclass) const override;
 };
}