#include "jni.h"
#include <csignal>

#include <iostream>

#define _CHECK_JNI_VALUE(value) \
if (!value) {\
 printf("Received null JNI element!\n");\
 abort();\
}

void exampleNativeMethod() {
 printf("Eample Native Method!\n");
}

JNIEXPORT
jint JNI_OnLoad(JavaVM * vm, void *) {
 printf("EXAMPLE AGENT: JNI_OnLoad!\n");
 JNIEnv *env;
 vm->GetEnv((void **)&env, JNI_VERSION_1_8);
 auto clazz = env->FindClass("yo_this is not a valid class name_");
 _CHECK_JNI_VALUE(clazz)
 auto constructor = env->GetMethodID(clazz, "<init>", "()V");
 _CHECK_JNI_VALUE(constructor)
 jobject inst = env->NewObject(clazz, constructor);
 printf("EXAMPLE AGENT: Created instance: 0x%lx\n", (uintptr_t)inst);
 _CHECK_JNI_VALUE(inst)
 auto mid = env->GetMethodID(clazz, "exampleMethod", "()V");
 _CHECK_JNI_VALUE(mid)
 env->CallVoidMethod(inst, mid);
 auto fid = env->GetFieldID(clazz, "someField", "I");
 _CHECK_JNI_VALUE(fid)
 jint value = env->GetIntField(inst, fid);
 printf("EXAMPLE AGENT: Value: someField = %i\n", value);
 JNINativeMethod nm {
  "test",
  "()V",
  (decltype(nm.fnPtr))&exampleNativeMethod
 };
 env->RegisterNatives(clazz, &nm, 1);
 return JNI_VERSION_1_8;
}

JNIEXPORT
void JNI_OnUnload(JavaVM * vm, void *) {
 printf("JNI_OnUnload!\n");
}