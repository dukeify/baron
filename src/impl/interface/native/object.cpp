#include "baron/impl/interface/native.h"

namespace Baron::Interface {
// jboolean NativeInterface::isSameObject(jobject obj1, jobject) const {
//  return
// }

 jobject NativeInterface::allocObject(jclass jclazz) const {

 }

 jobject NativeInterface::newObjectV(jclass, jmethodID, va_list) const {

 }

 jobject NativeInterface::newObjectA(jclass, jmethodID, const jvalue *) const {

 }

 jclass NativeInterface::getObjectClass(jobject) const {

 }

 jboolean NativeInterface::isInstanceOf(jobject jobj, jclass jclazz) const {

 }

 jclass NativeInterface::defineClass(const char *, jobject, const jbyte *, jsize) const {

 }

 jclass NativeInterface::findClass(const char *) const {

 }

 jclass NativeInterface::getSuperclass(jclass) const {

 }

 jboolean NativeInterface::isAssignableFrom(jclass, jclass) const {

 }

}