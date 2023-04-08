# Requirements
1. You need ROOT installed on your computer, and `root-config` location should be in your PATH environment variable. You can check it by running `root-config` in your command line
2. The JDK introduces a bridge between the bytecode running in our JVM and the native code. So you need JNI header files accessible in the system default header locations or specify `JAVA_HOME` environment variable. You should be able to find `jni.h` and `jni_md.h` in your `$JAVA_HOME/include` location.
3. Clone the repo, run scons, run maven:
```
git clone git@github.com:drewkenjo/j2root.git
cd j2root
mvn package
scons build/native
scons
```
4. Set environment variables:
  * `source setup.sh` in bash
  * `source setup.csh` in csh
5. `run-groovy root.groovy`

# Wiki
https://github.com/drewkenjo/j2root/wiki

# Troubleshooting

## array length is not legal

If your tree contains more than 250 branches your code can fail with this error
<pre>
Caught: java.lang.IllegalArgumentException: array length is not legal:
java.lang.IllegalArgumentException: array length is not legal:
</pre>

The reason is because the length of the varargs is limited, and groovy fails when you pass too many arguments in `fill` method.
The solution is to use List in the `fill` method instead of varargs.

Instead of:

`tree.Fill(0,1,2,3,4,5,6)`

use:

`tree.Fill([0,1,2,3,4,5,6])`
