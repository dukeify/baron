#include "baron/impl/interface/native.h"

#include <algorithm>

namespace Baron::Interface {
 jobject NativeInterface::allocObject(jclass jclazz) const {
  return vm.fabricateInstance(jclazz);
 }

 jobject NativeInterface::newObjectV(jclass jclazz, jmethodID jmid, va_list list) const {
  using namespace FakeJni;
  auto mid = (JMethodID *)jmid;
  if (mid->isArbitrary) {
   return vm.fabricateInstance(jclazz);
  }
  return FakeJni::NativeInterface::newObjectV(jclazz, jmid, list);
 }

 jobject NativeInterface::newObjectA(jclass jclazz, jmethodID jmid, const jvalue * values) const {
  using namespace FakeJni;
  auto mid = (JMethodID *)jmid;
  if (mid->isArbitrary) {
   return vm.fabricateInstance(jclazz);
  }
  return FakeJni::NativeInterface::newObjectA(jclazz, jmid, values);
 }

 jclass NativeInterface::getObjectClass(jobject jobj) const {
  auto & fabInstances = vm.fabricatedInstances;
  auto end = fabInstances.end();
  if (std::find(fabInstances.begin(), end, jobj) == end) {
   for (auto &[clazz, instances] : vm.fabricatedClassMappings) {
    end = instances.end();
    if (std::find(instances.begin(), end, jobj) != end) {
     return *clazz;
    }
   }
   //TODO this should never happen
//   throw std::runtime_error("FATAL: ");
   return nullptr;
  }
  return FakeJni::NativeInterface::getObjectClass(jobj);
 }

 jboolean NativeInterface::isInstanceOf(jobject jobj, jclass jclazz) const {
  using namespace FakeJni;
  JClass * clazz = *jclazz;
  if (clazz->isArbitrary) {
   auto& instances = vm.fabricatedClassMappings[clazz];
   auto end = instances.end();
   return (jboolean)(std::find(instances.begin(), end, jobj) != end);
  }
  return FakeJni::NativeInterface::isSameObject(jobj, jclazz);
 }
}