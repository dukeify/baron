#include "baron/impl/method.h"

#define VIRTUAL_INVOKE(arg) \
FakeJni::JClass * clazz = nullptr;\
for (auto &[clz, instances] : jvm->fabricatedClassMappings) {\
 if (instances.find((jobject)clazzOrObj) != instances.end()) {\
  clazz = clz;\
  break;\
 }\
}\
if (!clazz) {\
 throw std::runtime_error("FATAL: Could not find class mapping for fabricated object!");\
}\
return findVirtualMatch(clazz)->directInvoke<jvalue>(vm, clazzOrObj, arg);

namespace Baron::Internal {
 jvalue JMethodID::virtualInvoke(const JavaVM * vm, void * clazzOrObj, CX::va_list_t &list) const {
  auto jvm = (Baron::Jvm *)vm;
  if (jvm->isFabricated((jobject)clazzOrObj)) {
   VIRTUAL_INVOKE(list)
  }
  return FakeJni::JMethodID::virtualInvoke(vm, clazzOrObj, list);
 }

 jvalue JMethodID::virtualInvoke(const JavaVM * vm, void * clazzOrObj, const jvalue * args) const {
  auto jvm = (Baron::Jvm *)vm;
  if (jvm->isFabricated((jobject)clazzOrObj)) {
   VIRTUAL_INVOKE(args)
  }
  return FakeJni::JMethodID::virtualInvoke(vm, clazzOrObj, args);
 }
}