#include "baron/impl/interface/native.h"

#ifdef BARON_DEBUG
#define LOG_BLACKLIST_MATCH \
fprintf(\
 vm.getLog(),\
 "BARON INFO: Ignored blacklisted method lookup '%s#%s::%s'!\n",\
 className,\
 name,\
 sig\
);
#else
#define LOG_BLACKLIST_MATCH
#endif

#ifdef BARON_DEBUG
#define LOG_FABRICATED_METHOD \
fprintf(vm.getLog(), "BARON INFO: Fabricated method '%s%s' -> 0x%lx\n", name, sig, (intptr_t)mid);
#else
#define LOG_FABRICATED_CLASS
#endif

#define CHECK_BLACKLIST \
JClass * clazz = *jclazz;\
const auto className = clazz->getName();\
if (vm.isClassBlacklisted(className) || vm.isMethodBlacklisted(name, sig, className)) {\
 LOG_BLACKLIST_MATCH\
 return nullptr;\
}

//TODO
static void * fabricatedMethodCallback(JNIEnv * jenv, jobject jobj, ...) {
// using namespace FakeJni;
// auto env = (JniEnv *)jenv;
// auto& vm = env->getVM();
// const auto& log = vm.getLog();
// fprintf(log, "BARON INFO: Invoked ");
 return nullptr;
}

//TODO once fake-jni supports user-defined core classes, append the backtrace to the JMethodID for later debugging
namespace Baron::Interface {
 //TODO create backtrace of invocation
 jmethodID NativeInterface::getMethodID(jclass const jclazz, const char * name, const char * sig) const {
  using namespace FakeJni;
  CHECK_BLACKLIST
  auto mid = (JMethodID *)FakeJni::NativeInterface::getMethodID(jclazz, name, sig);
  if (!mid) {
   mid = new JMethodID(fabricatedMethodCallback, sig, name, JMethodID::PUBLIC);
   JClass * clazz = *jclazz;
   clazz->registerMethod(mid);
   LOG_FABRICATED_METHOD
  }
  return mid;
 }

 //TODO create backtrace of invocation
 jmethodID NativeInterface::getStaticMethodID(jclass jclazz, const char * name, const char * sig) const {
  using namespace FakeJni;
  CHECK_BLACKLIST
  auto mid = (JMethodID *)FakeJni::NativeInterface::getStaticMethodID(jclazz, name, sig);
  if (!mid) {
   mid = new JMethodID(fabricatedMethodCallback, sig, name, JMethodID::PUBLIC | JMethodID::STATIC);
   JClass * clazz = *jclazz;
   clazz->registerMethod(mid);
   LOG_FABRICATED_METHOD
  }
  return mid;
 }
}
