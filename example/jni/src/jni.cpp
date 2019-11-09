#include "jni.h"

#include <iostream>

JNIEXPORT
jint JNI_OnLoad(JavaVM * vm, void *) {
 printf("JNI_OnLoad!\n");
 JNIEnv *env;
 vm->GetEnv((void **)&env, JNI_VERSION_1_8);
 auto clazz = env->FindClass("yo_this is not a valid class name_");
 auto mid = env->GetMethodID(clazz, "exampleMethod", "()V");
 auto fid = env->GetFieldID(clazz, "someField", "I");
 return JNI_VERSION_1_8;
}

JNIEXPORT
void JNI_OnUnload(JavaVM * vm, void *) {
 printf("JNI_OnUnload!\n");
}