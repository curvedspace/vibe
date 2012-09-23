General Information
===================

Vibe contains libraries with features missing from Qt.

Some of the classes here are Maui and Hawaii specific, however some others are not
and the plan is to push what can be used by other projects to Qt.

A number of classes was forked or taken verbatim from kdelibs (frameworks branch),
and will probably be pushed to Qt by KDE developers.

Bulding
=======

To build the code create a separate build/ directory and run cmake from there,
for example:

```sh
mkdir build
cd build
cmake ..
```

The default prefix is /usr/local, in order to install on a different path, for
example /system, run cmake as reported below:

$ cmake -DCMAKE_INSTALL_PREFIX=/system ..
