#include <iostream>

#include <baron/baron.h>

using namespace Baron;

int main(int argc, char **argv) {
 Jvm vm;

 vm.attachLibrary("./libjni.so");

 vm.removeLibrary("./libjni.so");

 vm.destroy();

 return 0;
}