/****************************************************************************
 * This file is part of Qube.
 *
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Qube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Qube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Qube.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef QUBE_GLOBAL_H
#define QUBE_GLOBAL_H

/*
 * The following code is based on kde4libs/kdecore/kernel/kglobal.h
 *
 * Copyright (C) 1999 Sirtaj Singh Kanq <taj@kde.org>
 * Copyright (C) 2007 Matthias Kretz <kretz@kde.org>
 */

namespace Qube
{
    namespace Core
    {
/// @cond InternalDocs

        /**
         * @internal
         */
        typedef void (*QubeCleanUpFunction)();

        /**
         * @internal
         *
         * Helper class for QUBE_GLOBAL_STATIC to clean up the object on library unload or application
         * shutdown.
         */
        class CleanUpGlobalStatic
        {
        public:
            QubeCleanUpFunction func;

            inline ~CleanUpGlobalStatic() {
                func();
            }
        };
    }
}

#ifdef Q_CC_MSVC
/**
 * @internal
 *
 * MSVC seems to give anonymous structs the same name which fails at link time. So instead we name
 * the struct and hope that by adding the line number to the name it's unique enough to never clash.
 */
# define QUBE_GLOBAL_STATIC_STRUCT_NAME(NAME) _q_##NAME##__LINE__
#else
/**
 * @internal
 *
 * Make the struct of the QUBE_GLOBAL_STATIC anonymous.
 */
# define QUBE_GLOBAL_STATIC_STRUCT_NAME(NAME)
#endif

/// @endcond

/**
 * This macro makes it easy to use non-POD types as global statics.
 * The object is created on first use and creation is threadsafe.
 *
 * The object is destructed on library unload or application exit.
 * Be careful with calling other objects in the destructor of the class
 * as you have to be sure that they (or objects they depend on) are not already destructed.
 *
 * @param TYPE The type of the global static object. Do not add a *.
 * @param NAME The name of the function to get a pointer to the global static object.
 *
 * If you have code that might be called after the global object has been destroyed you can check
 * for that using the isDestroyed() function.
 *
 * If needed (If the destructor of the global object calls other functions that depend on other
 * global statics (e.g. KConfig::sync) your destructor has to be called before those global statics
 * are destroyed. A Qt post routine does that.) you can also install a post routine (qAddPostRoutine) to clean up the object
 * using the destroy() method. If you registered a post routine and the object is destroyed because
 * of a lib unload you have to call qRemovePostRoutine!
 *
 * Example:
 * @code
 * class A {
 * public:
 *     ~A();
 *     ...
 * };
 *
 * QUBE_GLOBAL_STATIC(A, globalA)
 * // The above creates a new globally static variable named 'globalA' which you
 * // can use as a pointer to an instance of A.
 *
 * void doSomething()
 * {
 *     //  The first time you access globalA a new instance of A will be created automatically.
 *     A *a = globalA;
 *     ...
 * }
 *
 * void doSomethingElse()
 * {
 *     if (globalA.isDestroyed()) {
 *         return;
 *     }
 *     A *a = globalA;
 *     ...
 * }
 *
 * void installPostRoutine()
 * {
 *     // A post routine can be used to delete the object when QCoreApplication destructs,
 *     // not adding such a post routine will delete the object normally at program unload
 *     qAddPostRoutine(globalA.destroy);
 * }
 *
 * A::~A()
 * {
 *     // When you install a post routine you have to remove the post routine from the destructor of
 *     // the class used as global static!
 *     qRemovePostRoutine(globalA.destroy);
 * }
 * @endcode
 *
 * A common case for the need of deletion on lib unload/app shutdown are Singleton classes. Here's
 * an example how to do it:
 * @code
 * class MySingletonPrivate;
 * class MySingleton
 * {
 * friend class MySingletonPrivate;
 * public:
 *     static MySingleton *self();
 *     QString someFunction();
 *
 * private:
 *     MySingleton();
 *     ~MySingleton();
 * };
 * @endcode
 * in the .cpp file:
 * @code
 * // This class will be instantiated and referenced as a singleton in this example
 * class MySingletonPrivate
 * {
 * public:
 *     QString foo;
 *     MySingleton instance;
 * };
 *
 * QUBE_GLOBAL_STATIC(MySingletonPrivate, mySingletonPrivate)
 *
 * MySingleton *MySingleton::self()
 * {
 *     // returns the singleton; automatically creates a new instance if that has not happened yet.
 *     return &mySingletonPrivate->instance;
 * }
 * QString MySingleton::someFunction()
 * {
 *     // Refencing the singleton directly is possible for your convenience
 *     return mySingletonPrivate->foo;
 * }
 * @endcode
 *
 * Instead of the above you can use also the following pattern (ignore the name of the namespace):
 * @code
 * namespace MySingleton
 * {
 *     QString someFunction();
 * }
 * @endcode
 * in the .cpp file:
 * @code
 * class MySingletonPrivate
 * {
 * public:
 *     QString foo;
 * };
 *
 * QUBE_GLOBAL_STATIC(MySingletonPrivate, mySingletonPrivate)
 *
 * QString MySingleton::someFunction()
 * {
 *     return mySingletonPrivate->foo;
 * }
 * @endcode
 *
 * Now code that wants to call someFunction() doesn't have to do
 * @code
 * MySingleton::self()->someFunction();
 * @endcode
 * anymore but instead:
 * @code
 * MySingleton::someFunction();
 * @endcode
 *
 * @ingroup Macros
 */
#define QUBE_GLOBAL_STATIC(TYPE, NAME) QUBE_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ())

/**
 * @overload
 * This is the same as QUBE_GLOBAL_STATIC, but can take arguments that are passed
 * to the object's constructor
 *
 * @param TYPE The type of the global static object. Do not add a *.
 * @param NAME The name of the function to get a pointer to the global static object.
 * @param ARGS the list of arguments, between brackets
 *
 * Example:
 * @code
 * class A
 * {
 * public:
 *     A(const char *s, int i);
 *     ...
 * };
 *
 * QUBE_GLOBAL_STATIC_WITH_ARGS(A, globalA, ("foo", 0))
 * // The above creates a new globally static variable named 'globalA' which you
 * // can use as a pointer to an instance of A.
 *
 * void doSomething()
 * {
 *     //  The first time you access globalA a new instance of A will be created automatically.
 *     A *a = globalA;
 *     ...
 * }
 * @endcode
 *
 * @ingroup Macros
 */
#define QUBE_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ARGS)                                      \
static QBasicAtomicPointer<TYPE > _q_static_##NAME = Q_BASIC_ATOMIC_INITIALIZER(0);         \
static bool _q_static_##NAME##_destroyed;                                                   \
static struct QUBE_GLOBAL_STATIC_STRUCT_NAME(NAME)                                          \
{                                                                                           \
    inline bool isDestroyed() const                                                         \
    {                                                                                       \
        return _q_static_##NAME##_destroyed;                                                \
    }                                                                                       \
    inline bool exists() const                                                              \
    {                                                                                       \
        return _q_static_##NAME != 0;                                                       \
    }                                                                                       \
    inline operator TYPE*()                                                                 \
    {                                                                                       \
        return operator->();                                                                \
    }                                                                                       \
    inline TYPE *operator->()                                                               \
    {                                                                                       \
        if (!_q_static_##NAME) {                                                            \
            if (isDestroyed()) {                                                            \
                qFatal("Fatal Error: Accessed global static '%s *%s()' after destruction. " \
                       "Defined at %s:%d", #TYPE, #NAME, __FILE__, __LINE__);               \
            }                                                                               \
            TYPE *x = new TYPE ARGS;                                                        \
            if (!_q_static_##NAME.testAndSetOrdered(0, x)                                   \
                && _q_static_##NAME != x ) {                                                \
                delete x;                                                                   \
            } else                                                                          \
                static Qube::Core::CleanUpGlobalStatic cleanUpObject = { destroy };         \
        }                                                                                   \
        return _q_static_##NAME;                                                            \
    }                                                                                       \
    inline TYPE &operator*()                                                                \
    {                                                                                       \
        return *operator->();                                                               \
    }                                                                                       \
    static void destroy()                                                                   \
    {                                                                                       \
        _q_static_##NAME##_destroyed = true;                                                \
        TYPE *x = _q_static_##NAME;                                                         \
        _q_static_##NAME = 0;                                                               \
        delete x;                                                                           \
    }                                                                                       \
} NAME;

#endif // QUBE_GLOBAL_H
