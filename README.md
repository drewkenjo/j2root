# Requirements
1. You need ROOT installed on your computer, and `root-config` location should be in your PATH environment variable. You can check it by running `root-config` in your command line
2. The JDK introduces a bridge between the bytecode running in our JVM and the native code. So you need JNI header files accessible in the system default header locations or specify `JAVA_HOME` environment variable. You should be able to find `jni.h` and `jni_md.h` in your `$JAVA_HOME/include` location.
