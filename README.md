Vibe
====

Vibe contains libraries with features missing from Qt.

Some of the classes here are Maui and Hawaii specific, however some others are not
and the plan is to push what can be used by other projects to Qt.

A number of classes was forked or taken verbatim from kdelibs (frameworks branch),
our changes will be sent to the KDE Project which in turns will probably
push some of their work to the Qt Project as well.

Building
=======

Assuming you are in the source directory, just create a build directory
and run cmake:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii ..

To do a debug build the last command can be:

    cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii -DCMAKE_BUILD_TYPE=Debug ..

To do a release build instead it can be:

    cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii -DCMAKE_BUILD_TYPE=Release ..

If not passed, the CMAKE_INSTALL_PREFIX parameter defaults to /usr/local.
You have to specify a path that fits your needs, /opt/hawaii is just an example.

Package maintainers would pass *-DCMAKE_INSTALL_PREFIX=/usr*.

The CMAKE_BUILD_TYPE parameter allows the following values:

    Debug: debug build
    Release: release build
    RelWithDebInfo: release build with debugging information

## Installation

It's really easy, it's just a matter of typing:

    make install

from the build directory.
