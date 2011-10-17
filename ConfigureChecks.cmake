include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckPrototypeExists)
include(CheckTypeSize)
include(CheckStructMember)
include(CheckCXXSourceCompiles)

check_include_files(stdio.h       HAVE_STDIO_H)                        # various
check_include_files(stdlib.h      HAVE_STDLIB_H)                       # various
check_include_files(string.h      HAVE_STRING_H)                       # various
check_include_files(strings.h     HAVE_STRINGS_H)                      # various
check_include_files(malloc.h      HAVE_MALLOC_H)                       # khtml
check_include_files(sys/time.h    TIME_WITH_SYS_TIME)                  # kdecore, kioslave
check_include_files(crt_externs.h HAVE_CRT_EXTERNS_H)                  # kinit, config.h

check_include_files(alloca.h      HAVE_ALLOCA_H)                       # kdecore, khtml
check_include_files(fstab.h       HAVE_FSTAB_H)                        # kio, kdecore
check_include_files(limits.h      HAVE_LIMITS_H)                       # various
check_include_files(mntent.h      HAVE_MNTENT_H)                       # solid, kio, kdecore
check_include_files(sysent.h      HAVE_SYSENT_H)                       # kdecore
check_include_files("sys/types.h;sys/mman.h" HAVE_SYS_MMAN_H)          # kdecore
check_include_files(sys/stat.h    HAVE_SYS_STAT_H)                     # various
check_include_files(sys/ucred.h   HAVE_SYS_UCRED_H)                    # kio
check_include_files(sys/types.h   HAVE_SYS_TYPES_H)                    # various
check_include_files(sys/select.h  HAVE_SYS_SELECT_H)                   # various
check_include_files(sys/param.h   HAVE_SYS_PARAM_H)                    # various
check_include_files("stdio.h;sys/mnttab.h"  HAVE_SYS_MNTTAB_H)         # kio, kdecore
check_include_files(sys/mntent.h  HAVE_SYS_MNTENT_H)                   # solid, kio, kdecore
check_include_files("sys/param.h;sys/mount.h"  HAVE_SYS_MOUNT_H)       # kio, kdecore
check_include_files(unistd.h      HAVE_UNISTD_H)                       # various
check_include_files(stdint.h      HAVE_STDINT_H)                       # various
check_include_files("sys/types.h;netinet/in.h"  HAVE_NETINET_IN_H)     # kio
check_include_files(paths.h       HAVE_PATHS_H)                        # kdecore, kio

check_include_files(errno.h       HAVE_ERRNO_H)                        # kjs, errno.h is used in many places, but only guarded in kjs/
check_include_files(sys/time.h    HAVE_SYS_TIME_H)                     # various
check_include_files(valgrind/memcheck.h   HAVE_VALGRIND_MEMCHECK_H)    # khtml
check_include_files(crtdbg.h      HAVE_CRTDBG_H)                       # kjs
check_include_files(langinfo.h    HAVE_LANGINFO_H)                     # kdecore

check_include_files(arpa/nameser_compat.h HAVE_ARPA_NAMESER_COMPAT_H) # kio
check_include_files(arpa/nameser8_compat.h HAVE_ARPA_NAMESER8_COMPAT_H) # kio

# Use check_symbol_exists to check for symbols in a reliable
# cross-platform manner.  It accounts for different calling
# conventions and the possibility that the symbol is defined as a
# macro.  Note that some symbols require multiple includes in a
# specific order.  Refer to the man page for each symbol for which a
# check is to be added to get the proper set of headers.
check_symbol_exists(strcmp          "string.h"                 HAVE_STRCMP)      # libltdl
check_symbol_exists(strrchr         "string.h"                 HAVE_STRRCHR)     # libltdl
check_symbol_exists(strtoll         "stdlib.h"                 HAVE_STRTOLL)     # kioslave
check_symbol_exists(S_ISSOCK        "sys/stat.h"               HAVE_S_ISSOCK)    # config.h
check_symbol_exists(vsnprintf       "stdio.h"                  HAVE_VSNPRINTF)   # config.h
check_symbol_exists(posix_madvise   "sys/mman.h"               HAVE_MADVISE)     # kdecore, kdeui
check_symbol_exists(getgrouplist    "unistd.h;grp.h"           HAVE_GETGROUPLIST)# kdecore/fakes.c

check_function_exists(backtrace        HAVE_BACKTRACE)                # kdecore, kio
check_function_exists(getpagesize      HAVE_GETPAGESIZE)              # khtml
# This is broken on OSX 10.6 (succeeds but shouldn't do) and doesn't exist
# on previous versions so don't do the check on APPLE.
if(NOT APPLE)
   check_function_exists(fdatasync     HAVE_FDATASYNC)                # kdecore, kate
endif(NOT APPLE)
check_function_exists(mmap             HAVE_MMAP)                     # kdecore, khtml

if(NOT WIN32)
  # we don't have it on windows but need to export it to be backward compatible
  # can be removed when 4.1 is out
  check_function_exists(readdir_r     HAVE_READDIR_R)                 # kio
endif(NOT WIN32)
check_function_exists(sendfile        HAVE_SENDFILE)                  # kioslave
check_function_exists(srandom         HAVE_SRANDOM)                   # config.h
check_function_exists(_NSGetEnviron   HAVE_NSGETENVIRON)              # kinit, config.h
check_function_exists(gettimeofday    HAVE_GETTIMEOFDAY)              # testkjs
check_function_exists(getgrouplist    HAVE_GETGROUPLIST)              # kio

check_library_exists(volmgt volmgt_running "" HAVE_VOLMGT)            # various

# Check for libresolv
# e.g. on slackware 9.1 res_init() is only a define for __res_init, so we check both, Alex
set(HAVE_RESOLV_LIBRARY FALSE)                                        # kdecore, kdecore/network, kpac
check_library_exists(resolv res_init "" HAVE_RES_INIT_IN_RESOLV_LIBRARY)
check_library_exists(resolv __res_init "" HAVE___RES_INIT_IN_RESOLV_LIBRARY)
if (HAVE___RES_INIT_IN_RESOLV_LIBRARY OR HAVE_RES_INIT_IN_RESOLV_LIBRARY)
   set(HAVE_RESOLV_LIBRARY TRUE)
endif (HAVE___RES_INIT_IN_RESOLV_LIBRARY OR HAVE_RES_INIT_IN_RESOLV_LIBRARY)

check_function_exists(getmntinfo HAVE_GETMNTINFO)        # kdecore, kio
check_function_exists(initgroups HAVE_INITGROUPS)        # kde3support/k3process, kdesu
check_function_exists(mkstemps   HAVE_MKSTEMPS)          # dcop, kdecore/fakes.c
check_function_exists(mkstemp    HAVE_MKSTEMP)           # kdecore/fakes.c
check_function_exists(mkdtemp    HAVE_MKDTEMP)           # kdecore/fakes.c
check_function_exists(random     HAVE_RANDOM)            # kdecore/fakes.c
check_function_exists(strlcpy    HAVE_STRLCPY)           # kdecore/fakes.c
check_function_exists(strlcat    HAVE_STRLCAT)           # kdecore/fakes.c

check_function_exists(setenv     HAVE_SETENV)            # kdecore/fakes.c
check_function_exists(seteuid    HAVE_SETEUID)           # kdecore/fakes.c
check_function_exists(setmntent  HAVE_SETMNTENT)         # solid, kio, kdecore
check_function_exists(unsetenv   HAVE_UNSETENV)          # kdecore/fakes.c
check_function_exists(usleep     HAVE_USLEEP)            # kdecore/fakes.c, kdeui/qxembed

# check for prototypes [for functions provided by kdefakes when not available]

check_prototype_exists(mkstemps "stdlib.h;unistd.h" HAVE_MKSTEMPS_PROTO)
check_prototype_exists(mkdtemp "stdlib.h;unistd.h"  HAVE_MKDTEMP_PROTO)
check_prototype_exists(mkstemp "stdlib.h;unistd.h"  HAVE_MKSTEMP_PROTO)
check_prototype_exists(strlcat string.h             HAVE_STRLCAT_PROTO)
check_prototype_exists(strlcpy string.h             HAVE_STRLCPY_PROTO)
check_prototype_exists(random stdlib.h              HAVE_RANDOM_PROTO)
check_prototype_exists(res_init "sys/types.h;netinet/in.h;arpa/nameser.h;resolv.h" HAVE_RES_INIT_PROTO)
check_prototype_exists(setenv stdlib.h              HAVE_SETENV_PROTO)
check_prototype_exists(srandom stdlib.h             HAVE_SRANDOM_PROTO)
check_prototype_exists(unsetenv stdlib.h            HAVE_UNSETENV_PROTO)
check_prototype_exists(usleep unistd.h              HAVE_USLEEP_PROTO)
check_prototype_exists(initgroups "unistd.h;sys/types.h;unistd.h;grp.h" HAVE_INITGROUPS_PROTO)
check_prototype_exists(setreuid unistd.h            HAVE_SETREUID_PROTO)
check_prototype_exists(seteuid unistd.h             HAVE_SETEUID_PROTO)
check_prototype_exists(trunc math.h                 HAVE_TRUNC)

# check for existing datatypes

check_cxx_source_compiles("
  #include <sys/types.h>
  #include <sys/statvfs.h>
  int main(){
    struct statvfs *mntbufp;
    int flags;
    return getmntinfo(&mntbufp, flags);
  }
" GETMNTINFO_USES_STATVFS )

check_struct_member(dirent d_type dirent.h HAVE_DIRENT_D_TYPE) # kdecore, kioslave/file
