# baron

## What is baron?
`baron` is a highly streamlined, cross-platform, reverse engineering framework for JNI modules and JVMTI agents. It 
automates the process of discovering the API of a [blackbox module](https://en.wikipedia.org/wiki/Black_box) by probing 
it through the JNI. In doing so, `baron` can discover the complete set of Java classes, class properties, JNI native 
methods, and many other properties, required by a given module. 

Once the module's API has been discovered, the user is free to implement any required Java class functionality in 
native code, through `fake-jni`. Any extraneous classes, methods or other properties, will be automatically stubbed by
`baron` so that the total implementation overhead of a purely native JNI module, or JVMTI agent, includes only the
definition of missing functionality. 

## How does it work?
`baron` is built on top of [`fake-jni`](https://github.com/dukeify/fake-jni), a framework for running JNI modules and 
JVMTI agents without the overhead of an accompanying JVM instance. See 
[How does it work?](https://github.com/dukeify/fake-jni#how-does-it-work?).

## How do I use it?
The API is identical to that of `fake-jni`, save for a few additions to configure probing behaviour. Any API details not
documented by `baron` will be documented in `fake-jni`.

To discover potential entry points into an agent or module, begin by simply attaching it to a vm instance and printing
the vm statistics:
```cpp
#include <baron/baron.h>

using namespace FakeJni;

int main() {
 Baron::Jvm vm;

 //attach your library or executable to the vm
 //all relevant JNI registrations / acquisitions during JNI_OnLoad will be logged 
 vm.attachLibrary("path/to/my/module/or/agent/library.so");
 
 //detach your library or executable from the vm
 vm.detachLibrary("path/to/my/module/or/agent/library.so");
 
 //get vm statistics including registrations, acquistions and more 
 vm.printStatistics();
 
 //discard any remaining vm state (this will detach all agents and modules as well)
 //this is optional, if you plan on reusing the vm with a new set of runtime properties,
 //then you should probably discard the previous state. Otherwise, it will be done 
 //automatically when the vm is destructed
 vm.destroy();
 
 return 0;
}
``` 

Once you know your entry point(s), all that's left is to invoke them and let the vm and module handle the rest.

```cpp
#include <iostream>

#include <baron/baron.h>

using namespace FakeJni;

int main() {
 Baron::Jvm vm;
 
 vm.attachLibrary("path/to/my/module/or/agent/library.so");
 
 //find some method registered during JNI_OnLoad to call in main
 JClass *encapsulatingClass = nullptr;
 JMethodID *entryPoint = nullptr;
 for (auto& clazz : vm.getClasses()) {
  if (strcmp(clazz->getName(), "com/example/SomeClass") == 0) {
   encapsulatingClass = clazz;
   entryPoint = clazz->getMethod("(IID)I", "someMethod");
   break;
  }
 }
 
 //check that we found the method
 if (!entryPoint) {
  throw std::runtime_error("Could not find entry point!");
 }

 JInt result;

 //note: your module or agent may register a Java entry point during any of the 
 //JNI_OnLoad, Agent_OnLoad or Agent_OnAttach, phases. If this occurs, then you
 //do not need to register your own entry point, and can instead simply invoke
 //`Jvm::start()`.
 //create the Java entry point in native code
 createMainMethod(vm, [&](JArray<JString *> * args) {
  result = entryPoint->invoke(vm, encapsulatingClass, (JInt)3, (JInt)2, (JDouble)3.14);
 });
 
 //start vm execution (blocks until execution is complete)
 //all relevant JNI and JVM registrations / acquistions during execution will be logged 
 vm.start();
 
 //do something with our result
 std::cout << "Result: " << result << std::endl;
 
 //cleanup and print statistics
 vm.detachLibrary("path/to/my/module/or/agent/library.so");
 vm.printStatistics();
 vm.destroy();
 
 return 0;
}
```

## What properties are logged while probing?
**Note:** Not all of these properties are currently implemented. See 
[#2](https://github.com/dukeify/baron/issues/2) for more details.

- Registered classes
  - Class parent
  - Fabrication flag
  - Total Instance count at destruction
  - Fabricated and constructed instance counts
  - Serialized modifiers
  - Registered fields
    - Name and signature
    - Fabrication flag
    - Serialized modifiers
    - Access count
    - Default fabricated value
  - Registered functions
    - Name and signature
    - Fabrication flag
    - Serialized modifiers
    - Invocation count

## Supported Targets
`baron` supports all targets that `fake-jni` supports. See 
[Supported targets](https://github.com/dukeify/fake-jni#supported-targets). 

## When can I use it?
Once the [0.1.0 milestone](https://github.com/dukeify/baron/milestone/1) is reached, the JNI portion of `baron` will be 
complete. At this point, most users will be able to use `baron` for their needs. JVMTI agent support will take longer as
`fake-jni` has not fully implemented the JVMTI yet. See `baron`'s 
[0.2.0 milestone](https://github.com/dukeify/baron/milestone/2) and `fake-jni`'s
[0.5.0 milestone](https://github.com/dukeify/fake-jni/milestone/3).

## Licensing
The JNI headers are licensed under the GPL, with the "classpath" exception, meaning that we are free to use and 
implement the JNI headers in `baron`, however, any changes made to JNI headers will be made under the GPL, in complete 
compliance with the licensing. The rest of `baron`, including implementations of other GPL licensed headers, are 
licensed under the GPLv3 license.

`baron` is fully complaint with the [`fake-jni` licensing](https://github.com/dukeify/fake-jni#licensing).
