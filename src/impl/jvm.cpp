#include <iostream>

#include "baron/impl/jvm.h"
#include "baron/impl/interface/invoke.h"
#include "baron/impl/interface/native.h"
#include "baron/impl/interface/jvmti.h"
#include "baron/impl/env/jni.h"
#include "baron/impl/env/jvmti.h"

#include <algorithm>

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

 //TODO print all leased properties
 void Jvm::destroy() {
  auto& classes = getClasses();
  const auto len = classes.getSize();
  for () {

  }
 }

 void Jvm::blacklistClass(const char * name) {
  blacklistedClasses.insert(name);
 }

 void Jvm::blacklistField(const char * clazz, const char * name, const char * signature) {
  if (std::find(blacklistedClasses.begin(), blacklistedClasses.end(), clazz) != blacklistedClasses.end()) {
#ifdef BARON_DEBUG
   fprintf(getLog(), "BARON WARNING: Encapsulating class is already blacklisted!\n");
#endif
  } else {
   blacklistedFields[clazz].insert({name, signature});
  }
 }

 void Jvm::blacklistField(const char * name, const char * signature) {
  blacklistedFields[nullptr].insert({name, signature});
 }

 void Jvm::blacklistMethod(const char * clazz, const char * name, const char * signature) {
  if (std::find(blacklistedClasses.begin(), blacklistedClasses.end(), clazz) != blacklistedClasses.end()) {
#ifdef BARON_DEBUG
   fprintf(getLog(), "BARON WARNING: Encapsulating class is already blacklsited!\n");
#endif
  } else {
   blacklistedMethods[clazz].insert({name, signature});
  }
 }

 void Jvm::blacklistMethod(const char * name, const char * signature) {
  blacklistedFields[nullptr].insert({name, signature});
 }
}