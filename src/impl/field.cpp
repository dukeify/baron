#include "baron/impl/field.h"

namespace Baron::Internal {
 jvalue JFieldID::get(const JavaVM * vm, FakeJni::JObject * obj) const {
  jobject jobj = *obj;
  auto jvm = (Baron::Jvm *)vm;
  if (jvm->isFabricated(jobj)) {
   auto clazz = resolveType(jvm);
   if (!clazz) {
    throw std::runtime_error("FATAL: Tried to fabricate a value for an unregistered type!");
   }
   auto pair = std::pair{jobj, this};
   jvalue value;
   bool found = false;
   for (auto &[k, v]: jvm->fabricatedValues) {
    if (k.first == pair.first && k.second == pair.second) {
     found = true;
     break;
    }
   }
   if (found) {
    value = jvm->fabricatedValues[pair];
   } else {
    value = jvm->fabricateValue(*clazz);
    jvm->fabricatedValues[pair] = value;
   }
   return value;
  }
  return FakeJni::JFieldID::get(vm, obj);
 }

 void JFieldID::set(const JavaVM * vm, FakeJni::JObject * obj, void * value) const {
  jobject jobj = *obj;
  auto jvm = (Baron::Jvm *)vm;
  if (jvm->isFabricated(jobj)) {
   auto clazz = resolveType(jvm);
   if (!clazz) {
    throw std::runtime_error("FATAL: Tried to fabricate a value for an unregistered type!");
   }
   auto pair = std::pair{jobj, this};
   jvm->fabricatedValues[pair] = jvm->fabricateValue(*clazz);
  }
  FakeJni::JFieldID::set(vm, obj, value);
 }
}