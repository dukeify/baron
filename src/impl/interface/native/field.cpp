#include "baron/impl/interface/native.h"

#ifdef BARON_DEBUG
#define LOG_BLACKLIST_MATCH \
fprintf(\
 vm.getLog(),\
 "BARON INFO: Ignored blacklisted field lookup '%s#%s::%s'!\n",\
 className,\
 name,\
 sig\
);
#else
#define LOG_BLACKLIST_MATCH
#endif

#ifdef BARON_DEBUG
#define LOG_FABRICATED_FIELD \
fprintf(\
 vm.getLog(),\
 "BARON INFO: Fabricated field %s::%s -> 0x%lx\n",\
 name,\
 sig,\
 (intptr_t)fid\
);
#else
#define LOG_FABRICATED_FIELD
#endif

#define CHECK_BLACKLIST \
JClass * clazz = *jclazz;\
const auto className = clazz->getName();\
if (vm.isClassBlacklisted(className) || vm.isFieldBlacklisted(name, sig, className)) {\
 LOG_BLACKLIST_MATCH\
 return nullptr;\
}

//TODO
static void * fabricatedGetCallback(void * inst) {
 return nullptr;
}

//TODO
static void fabricatedSetCallback(void * inst, void * value) {

}

//TODO once fake-jni supports user-defined core classes, append the backtrace to the JFieldID for later debugging
namespace Baron::Interface {
 //TODO create backtrace of invocation
 jfieldID NativeInterface::getFieldID(jclass jclazz, const char * name, const char * sig) const {
  using namespace FakeJni;
  CHECK_BLACKLIST
  auto fid = (JFieldID *)FakeJni::NativeInterface::getFieldID(jclazz, name, sig);
  if (!fid) {
   fid = new JFieldID(fabricatedGetCallback, fabricatedSetCallback, name, sig, JFieldID::PUBLIC);
   JClass * clazz = *jclazz;
   clazz->registerField(fid);
   LOG_FABRICATED_FIELD
  }
  return fid;
 }

 //TODO create backtrace of invocation
 jfieldID NativeInterface::getStaticFieldID(jclass jclazz, const char * name, const char * sig) const {
  using namespace FakeJni;
  CHECK_BLACKLIST
  auto fid = (JFieldID *)FakeJni::NativeInterface::getStaticFieldID(jclazz, name, sig);
  if (!fid) {
   fid = new JFieldID(fabricatedGetCallback, fabricatedSetCallback, name, sig, JFieldID::PUBLIC | JFieldID::STATIC);
   JClass * clazz = *jclazz;
   clazz->registerField(fid);
   LOG_FABRICATED_FIELD
  }
  return fid;
 }
}