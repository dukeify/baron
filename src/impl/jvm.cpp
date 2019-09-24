#include <iostream>

#include "baron/impl/jvm.h"

namespace Baron {
 //Jvm members
 Jvm::Jvm(FILE * log) : FakeJni::Jvm(log) {
  setInvokeInterface<Interface::InvokeInterface>();
  setNativeInterface<Interface::NativeInterface>();
  setJvmtiInterface<Interface::JvmtiInterfacee>();
  setJniEnv<Env::JniEnv>();
  setJvmtiEnv<Env::JvmtiEnv>();
 }
}