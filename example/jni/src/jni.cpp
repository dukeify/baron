#include "jni.h"
#include <csignal>

#include <iostream>

#define _CHECK_JNI_VALUE(value) \
if (!value) {\
 printf("Received null JNI element!\n");\
 abort();\
}

JNIEXPORT
jint JNI_OnLoad(JavaVM * vm, void *) {
 printf("JNI_OnLoad!\n");
 JNIEnv *env;
 vm->GetEnv((void **)&env, JNI_VERSION_1_8);
 auto clazz = env->FindClass("yo_this is not a valid class name_");
 _CHECK_JNI_VALUE(clazz)
 auto mid = env->GetMethodID(clazz, "exampleMethod", "()V");
 _CHECK_JNI_VALUE(mid)
 auto fid = env->GetFieldID(clazz, "someField", "I");
 _CHECK_JNI_VALUE(fid)
 return JNI_VERSION_1_8;
}

JNIEXPORT
void JNI_OnUnload(JavaVM * vm, void *) {
 printf("JNI_OnUnload!\n");
}