#include <iostream>

#include "baron/impl/jvm.h"
#include "baron/impl/class.h"
#include "baron/impl/interface/invoke.h"
#include "baron/impl/interface/native.h"
#include "baron/impl/interface/jvmti.h"
#include "baron/impl/env/jni.h"
#include "baron/impl/env/jvmti.h"
#include "baron/util/util.h"

#include <algorithm>
#include <stdexcept>

#ifdef BARON_DEBUG
#define DEBUG_DOUBLE_BLACKLIST \
fprintf(getLog(), "BARON WARNING: Encapsulating class is already blacklisted!\n");\
return;

#else
#define DEBUG_DOUBLE_BLACKLIST
#endif

namespace Baron {
 //Jvm members
 Jvm::Jvm(FILE * log) : FakeJni::Jvm(log) {
  setInvokeInterface<Interface::InvokeInterface>();
  setNativeInterface<Interface::NativeInterface>();
  setJvmtiInterface<Interface::JvmtiInterface>();
  setJniEnv<Env::JniEnv>();
  setJvmtiEnv<Env::JvmtiEnv>();
 }

 const FakeJni::JClass * Jvm::findClass(const char * name) const {
  using namespace FakeJni;
  auto& ref = const_cast<Jvm &>(*this);
  //Ignore class lookups for blacklisted classes
  if (isClassBlacklisted(name)) {
#ifdef BARON_DEBUG
   fprintf(getLog(), "BARON INFO: Ignored lookup request for blacklisted class '%s'\n", name);
#endif
   return nullptr;
  }
  auto clazz = FakeJni::Jvm::findClass(name);
  if (!clazz) {
   clazz = new Baron::Internal::JClass(name);
   ref.registerClass(clazz, true);
#ifdef BARON_DEBUG
   fprintf(getLog(), "BARON INFO: Fabricated class '%s' -> 0x%lx\n", name, (intptr_t)clazz);
#endif
  }
  return clazz;
 }

 //TODO print more information
 // - field and function access / invocation counts
 // - field and function access locations (with stack traces once libunwind is integrated)
 jint Jvm::destroy() {
  auto & classes = getClasses();
  const auto & log = getLog();
  if (classes.size() > 0) {
   fprintf(log, "BARON INFO: The following classes were registered during execution:\n");
   for (auto clazz : getClasses()) {
    fprintf(log, "\t'%s':\n", clazz->getName());
    auto mods = clazz->modifiers;
    fprintf(log, "\t - Modifiers: 0x%x [%s]\n", mods, deserializeClassModifiers(mods).c_str());
    fprintf(log, "\t - Parent: '%s'\n", clazz->parent.getName());
    fprintf(log, "\t - Fabricated: %s\n", (clazz->isArbitrary ? "yes" : "no"));
    auto & fields = clazz->getFields();
    fprintf(log, "\t - Fields: %u\n", fields.size());
    for (auto & field : fields) {
     fprintf(log, "\t\t'%s' -> '%s':\n", field->getName(), field->getSignature());
     mods = field->getModifiers();
     fprintf(log, "\t\t - Modifiers: 0x%x [%s]\n", mods, deserializeFieldModifiers(mods).c_str());
     fprintf(log, "\t\t - Fabricated: %s\n", (field->isArbitrary ? "yes" : "no"));
    }
    auto & methods = clazz->getMethods();
    fprintf(log, "\t - Methods: %u\n", methods.size());
    for (auto & method : methods) {
     fprintf(log, "\t\t'%s%s':\n", method->getName(), method->getSignature());
     mods = method->getModifiers();
     fprintf(log, "\t\t - Modifiers: 0x%x [%s]\n", mods, deserializeMethodModifiers(mods).c_str());
     fprintf(log, "\t\t - Fabricated: %s\n", (method->isArbitrary ? "yes" : "no"));
    }
   }
  } else {
   fprintf(log, "BARON INFO: No classes were registered during execution.\n");
  }
  return JNI_OK;
 }

 bool Jvm::isClassBlacklisted(const char * name) const {
  auto end = blacklistedClasses.end();
  return (std::find(blacklistedClasses.begin(), end, std::string(name)) != end);
 }

 bool Jvm::isMethodBlacklisted(const char * name, const char * sig, const char * clazz) const {
  auto identity = std::string(name) + sig;
  auto& ref = const_cast<Jvm &>(*this);
  //check global blacklist first
  auto& globalBlacklist = ref.blacklistedMethods[std::string("")];
  auto end = globalBlacklist.end();
  if (std::find(globalBlacklist.begin(), end, identity) != end) {
   return true;
  }
  //if the requested blacklist lookup wasn't global
  if (clazz[0]) {
   auto& classedBlacklist = ref.blacklistedMethods[std::string(clazz)];
   end = classedBlacklist.end();
   if (std::find(classedBlacklist.begin(), end, identity) != end) {
    return true;
   }
  }
  return false;
 }

 bool Jvm::isFieldBlacklisted(const char * name, const char * sig, const char * clazz) const {
  auto identity = std::string(name) + "::" + sig;
  auto& ref = const_cast<Jvm &>(*this);
  //check global blacklist first
  auto& globalBlacklist = ref.blacklistedFields[std::string("")];
  auto end = globalBlacklist.end();
  if (std::find(globalBlacklist.begin(), end, identity) != end) {
   return true;
  }
  //if the requested blacklist lookup wasn't global
  if (clazz[0]) {
   auto& classedBlacklist = ref.blacklistedFields[std::string(clazz)];
   end = classedBlacklist.end();
   if (std::find(classedBlacklist.begin(), end, identity) != end) {
    return true;
   }
  }
  return false;
 }

 void Jvm::blacklistClass(const char * name) {
  if (isClassBlacklisted(name)) {
   DEBUG_DOUBLE_BLACKLIST
  }
  blacklistedClasses.insert(name);
 }

 void Jvm::blacklistField(const char * name, const char * sig, const char * clazz) {
  if (isFieldBlacklisted(name, sig, clazz)) {
   DEBUG_DOUBLE_BLACKLIST
  }
  blacklistedFields[clazz].insert(std::string(name) + "::" + sig);
 }

 void Jvm::blacklistMethod(const char * name, const char * sig, const char * clazz) {
  if (isMethodBlacklisted(name, sig, clazz)) {
   DEBUG_DOUBLE_BLACKLIST
  }
  blacklistedMethods[clazz].insert(std::string(name) + sig);
 }

 jobject Jvm::fabricateInstance(jclass jclazz) const {
  auto& ref = const_cast<Jvm &>(*this);
  auto pinst = CX::union_cast<char **>(&lastInstance);
  *pinst += 1;
  auto inst = (jobject)*pinst;
  ref.fabricatedInstances.insert(inst);
  ref.fabricatedClassMappings[*jclazz].insert(inst);
  return inst;
 }

 jvalue Jvm::fabricateValue(jclass jclazz) const {
  jvalue value;
  FakeJni::JClass *clazz = *jclazz;
  if (clazz->isArbitrary) {
   value.l = fabricateInstance(jclazz);
  } else {
   if (!clazz->isPrimitive) {
    //TODO instance fabrication for non-arbitrary classes
    throw std::runtime_error("FATAL: Value fabrication for non-primitive types is not currently supported!");
   }
  }
  return value;
 }

 bool Jvm::isFabricated(jobject jobj) const noexcept {
  return fabricatedInstances.find(jobj) != fabricatedInstances.end();
 }
}