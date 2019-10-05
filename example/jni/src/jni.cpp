#include "jni.h"

#include <iostream>

JNIEXPORT
jint JNI_OnLoad(JavaVM * vm, void *) {
 printf("JNI_OnLoad!\n");
 return JNI_VERSION_1_8;
}

JNIEXPORT
void JNI_OnUnload(JavaVM * vm, void *) {
 printf("JNI_OnUnload!\n");
}