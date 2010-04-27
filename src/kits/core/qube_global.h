#ifndef QUBE_GLOBAL_H
#define QUBE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QUBE_LIBRARY)
#  define QUBESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QUBESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QUBE_GLOBAL_H
