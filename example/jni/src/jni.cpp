#include "jni.h"

#include <iostream>

JNIEXPORT
jint JNI_OnLoad(JavaVM * vm, void *) {
 printf("JNI_OnLoad!\n");
 return JNI_OK;
}

JNIEXPORT
void JNI_OnUnload(JavaVM * vm, void *) {
 printf("JNI_OnUnload!\n");
}