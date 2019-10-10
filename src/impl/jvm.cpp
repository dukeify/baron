#include <iostream>

#include "baron/impl/jvm.h"
#include "baron/impl/interface/invoke.h"
#include "baron/impl/interface/native.h"
#include "baron/impl/interface/jvmti.h"
#include "baron/impl/env/jni.h"
#include "baron/impl/env/jvmti.h"

#include <algorithm>

#ifdef BARON_DEBUG
#define DEBUG_DOUBLE_BLACKLIST \
else {\
 fprintf(getLog(), "BARON WARNING: Encapsulating class is already blacklisted!\n");\
}
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
  if (std::find(blacklistedClasses.begin(), blacklistedClasses.end(), name) != blacklistedClasses.end()) {
   fprintf(getLog(), "BARON INFO: Ignored lookup request for blacklisted class '%s'\n", name);
   return nullptr;
  }
  auto clazz = FakeJni::Jvm::findClass(name);
  if (!clazz) {
   clazz = new JClass(name);
   ref.registerClass(clazz);
   fprintf(getLog(), "BARON INFO: Fabricated class '%s' -> 0x%lx\n", name, (intptr_t)clazz);
  }
  return clazz;
 }

 //TODO print more information
 // - field and function access / invocation counts
 // - field and function access locations (with stack traces once libunwind is integrated)
 // - serialize field and function modifiers to their respective names and print them as a list
 void Jvm::destroy() {
  auto & classes = getClasses();
  const auto & log = getLog();
  if (classes.size() > 0) {
   fprintf(log, "BARON INFO: The following classes were registered during execution:\n");
   for (auto clazz : getClasses()) {
    fprintf(log, "\t'%s':\n", clazz->getName());
    fprintf(log, "\t - Modifiers: 0x%x\n", clazz->modifiers);
    fprintf(log, "\t - Parent: '%s'\n", clazz->parent.getName());
    fprintf(log, "\t - Fabricated: %s\n", (clazz->isArbitrary ? "yes" : "no"));
    auto & fields = clazz->getFields();
    fprintf(log, "\t - Fields: %u\n", fields.size());
    for (auto & field : fields) {
     fprintf(log, "\t\t'%s' -> '%s':\n", field->getName(), field->getSignature());
     fprintf(log, "\t\t - Modifiers: 0x%x\n", field->getModifiers());
     fprintf(log, "\t\t - Fabricated: %s\n", (field->isArbitrary ? "yes" : "no"));
    }
    auto & methods = clazz->getMethods();
    for (auto & method : methods) {
     fprintf(log, "\t\t'%s%s':\n", method->getName(), method->getSignature());
     fprintf(log, "\t\t - Modifiers: 0x%x\n", method->getModifiers());
     fprintf(log, "\t\t - Fabricated: %s\n", (method->isArbitrary ? "yes" : "no"));
    }
   }
  } else {
   fprintf(log, "BARON INFO: No classes were registered during execution.\n");
  }
 }

 bool Jvm::isClassBlacklisted(const char * name) {
  auto end = blacklistedClasses.end();
  return (std::find(blacklistedClasses.begin(), end, std::string(name)) != end);
 }

 bool Jvm::isMethodBlacklisted(const char * name, const char * sig, const char * clazz) {
  auto identity = std::string(name) + sig;
  //check global blacklist first
  auto& globalBlacklist = blacklistedMethods[std::string("")];
  auto end = globalBlacklist.end();
  if (std::find(globalBlacklist.begin(), end, identity) != end) {
   return true;
  }
  //if the requested blacklist lookup wasn't global
  if (clazz[0]) {
   auto& classedBlacklist = blacklistedMethods[std::string(clazz)];
   end = classedBlacklist.end();
   if (std::find(classedBlacklist.begin(), end, identity) != end) {
    return true;
   }
  }
  return false;
 }

 bool Jvm::isFieldBlacklisted(const char * name, const char * sig, const char * clazz) {
  auto identity = std::string(name) + "::" + sig;
  //check global blacklist first
  auto& globalBlacklist = blacklistedFields[std::string("")];
  auto end = globalBlacklist.end();
  if (std::find(globalBlacklist.begin(), end, identity) != end) {
   return true;
  }
  //if the requested blacklist lookup wasn't global
  if (clazz[0]) {
   auto& classedBlacklist = blacklistedFields[std::string(clazz)];
   end = classedBlacklist.end();
   if (std::find(classedBlacklist.begin(), end, identity) != end) {
    return true;
   }
  }
  return false;
 }

 void Jvm::blacklistClass(const char * name) {
  if (isClassBlacklisted(name)) {
   blacklistedClasses.insert(name);
  }
  DEBUG_DOUBLE_BLACKLIST
 }

 void Jvm::blacklistField(const char * name, const char * sig, const char * clazz) {
  if (isFieldBlacklisted(name, sig, clazz)) {
   blacklistedFields[clazz].insert(std::string(name) + "::" + sig);
  }
  DEBUG_DOUBLE_BLACKLIST
 }

 void Jvm::blacklistMethod(const char * name, const char * sig, const char * clazz) {
  if (isMethodBlacklisted(name, sig, clazz)) {
   blacklistedMethods[clazz].insert(std::string(name) + sig);
  }
  DEBUG_DOUBLE_BLACKLIST
 }

 jobject Jvm::fabricateInstance(jclass jclazz) {
  auto pinst = CX::union_cast<char **>(&lastInstance)();
  *pinst += 1;
  auto inst = (jobject)*pinst;
  fabricatedInstances.insert(inst);
  fabricatedClassMappings[*jclazz].insert(inst);
  return inst;
 }
}