#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QUBE_LIBRARY)
#  define QUBESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QUBESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GLOBAL_H
