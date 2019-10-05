#pragma once

#include <fake-jni/jvm.h>

#include <map>
//#include <vector>
#include <set>
#include <utility>

namespace Baron {
 class Jvm : public FakeJni::Jvm {
 public:
  using class_property_t = std::set<std::pair<std::string, std::string>>;

  std::set<std::string> blacklistedClasses;
  std::map<std::string, class_property_t> blacklistedFields;
  std::map<std::string, class_property_t> blacklistedMethods;

  explicit Jvm(FILE * log = stdout);

  //FakeJni::Jvm overrides
  virtual const FakeJni::JClass * findClass(const char * name) const override;
  virtual void destroy() override;

  //baron specific
  //Blacklist class from fabrication
  virtual void blacklistClass(const char * name);
  //Blacklist field in 'clazz' from fabrication
  virtual void blacklistField(const char * clazz, const char * name, const char * signature);
  //Globally blacklist field from fabrication
  virtual void blacklistField(const char * name, const char * signature);
  //Blacklist method in 'clazz' from fabrication
  virtual void blacklistMethod(const char * clazz, const char * name, const char * signature);
  //Globally blacklist method from fabrication
  virtual void blacklistMethod(const char * name, const char * signature);
 };
}