/*
  ql_sysconf.cpp

  Qore Programming Language

  Copyright 2003 - 2011 Petr Vanek, Qore Technologies

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


/** @mainpage %Qore %Sysconf Module

    Contents of this documentation:
    - @ref intro
    - @ref functions
    - @ref constants

    @section intro Introduction

    The sysconf module provides Qore the possibility to obtain runtime system infomation from the underlying operating system.
    The functionality is provided by standard C library.

    This module is released under the <a href="http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html">LGPL 2.1</a>
    and is tagged as such in the module's header (meaning it can be loaded unconditionally regardless of how
    the %Qore library was initialized). This version of the module requires Qore 0.8.0+ to compile and run.

    To use the module in a %Qore script, use the \c %%requires directive as follows:
    @code %requires sysconf @endcode

    @section functions Functions
    
    - int \b sysconf ( softint )
    
    - hash \b sysconf_constants ( )

    
    @section constants Constants
*/

#include <qore/Qore.h>
#include <errno.h>


QoreStringNode *sysconf_module_init();                                                                                
void sysconf_module_ns_init(QoreNamespace *rns, QoreNamespace *qns);                                                  
void sysconf_module_delete();                                                                                         

// qore module symbols                                                                                              
DLLEXPORT char qore_module_name[] = "sysconf";                                                                        
DLLEXPORT char qore_module_version[] = PACKAGE_VERSION;                                                             
DLLEXPORT char qore_module_description[] = "Sysconf library wrapper";                                                          
DLLEXPORT char qore_module_author[] = "Petr Vanek";                                                                 
DLLEXPORT char qore_module_url[] = "http://qore.org";                                                               
DLLEXPORT int qore_module_api_major = QORE_MODULE_API_MAJOR;                                                        
DLLEXPORT int qore_module_api_minor = QORE_MODULE_API_MINOR;                                                        
DLLEXPORT qore_module_init_t qore_module_init = sysconf_module_init;                                                  
DLLEXPORT qore_module_ns_init_t qore_module_ns_init = sysconf_module_ns_init;                                         
DLLEXPORT qore_module_delete_t qore_module_delete = sysconf_module_delete;                                            
DLLEXPORT qore_license_t qore_module_license = QL_LGPL;                                                              


/** @namespace Sysconf
    
    \brief All constants from this module are members of the Sysconf namespace.
    
    Functions provided by this module are located in Qore main namespace.
    See Qore documentation for namespace members access and handling.
*/
QoreNamespace SysconfNS("Sysconf");
QoreNamespace ConfstrNS("Confstr");


static AbstractQoreNode *f_sysconf(const QoreListNode *params, ExceptionSink *xsink) {
#ifdef HAVE_SYSCONF
    int param = (int)HARD_QORE_INT(params, 0);

    errno = 0;
    int ret = sysconf(param);

    if (errno != 0) {
       xsink->raiseErrnoException("SYSCONF-ERROR", errno, "sysconf(%d) failed", param);
       return 0;
    }

    return new QoreBigIntNode(ret);
#else
    return missing_function_error("sysconf", xsink);
#endif
}


static AbstractQoreNode *f_sysconf_constants(const QoreListNode *params, ExceptionSink *xsink) {
    return SysconfNS.getConstantInfo();
}


static AbstractQoreNode *f_confstr(const QoreListNode *params, ExceptionSink *xsink) {
#ifdef HAVE_CONFSTR
    int param = (int)HARD_QORE_INT(params, 0);

    errno = 0;
    size_t len = confstr(param, NULL, 0);
    if (!len || errno) {
       xsink->raiseErrnoException("CONFSTR-ERROR", errno, "confstr(%d) failed", param);
       return 0;
    }

    SimpleRefHolder<QoreStringNode> rv(new QoreStringNode);
    // this reserves len + 1 bytes in the string
    rv->reserve(len);
    // theoretically there should be no error here, but just in case we have additional error handling
    errno = 0;
    if (!confstr(param, (char*)rv->getBuffer(), len + 1) || errno) {
       xsink->raiseErrnoException("CONFSTR-ERROR", errno, "confstr(%d) failed", param);
       return 0;
    }

    rv->terminate(len);
    return rv.release();
#else
    return missing_function_error("confstr", xsink);
#endif
}


static AbstractQoreNode *f_confstr_constants(const QoreListNode *params, ExceptionSink *xsink) {
    return ConfstrNS.getConstantInfo();
}


QoreStringNode * sysconf_module_init() {

    SysconfNS.addConstant("SC_NO_LIMIT", new QoreBigIntNode(-1), bigIntTypeInfo);
    
#ifdef HAVE_SYSCONF
    // for code generator see below...
    /* configurable system variables */
    // #ifdef _SC_ARG_MAX    1
    #ifdef _SC_ARG_MAX
    SysconfNS.addConstant("SC_ARG_MAX", new QoreBigIntNode(_SC_ARG_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_CHILD_MAX    2
    #ifdef _SC_CHILD_MAX
    SysconfNS.addConstant("SC_CHILD_MAX", new QoreBigIntNode(_SC_CHILD_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_CLK_TCK    3
    #ifdef _SC_CLK_TCK
    SysconfNS.addConstant("SC_CLK_TCK", new QoreBigIntNode(_SC_CLK_TCK), bigIntTypeInfo);
    #endif
    // #ifdef _SC_NGROUPS_MAX    4
    #ifdef _SC_NGROUPS_MAX
    SysconfNS.addConstant("SC_NGROUPS_MAX", new QoreBigIntNode(_SC_NGROUPS_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_OPEN_MAX    5
    #ifdef _SC_OPEN_MAX
    SysconfNS.addConstant("SC_OPEN_MAX", new QoreBigIntNode(_SC_OPEN_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_JOB_CONTROL    6
    #ifdef _SC_JOB_CONTROL
    SysconfNS.addConstant("SC_JOB_CONTROL", new QoreBigIntNode(_SC_JOB_CONTROL), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SAVED_IDS    7
    #ifdef _SC_SAVED_IDS
    SysconfNS.addConstant("SC_SAVED_IDS", new QoreBigIntNode(_SC_SAVED_IDS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_VERSION    8
    #ifdef _SC_VERSION
    SysconfNS.addConstant("SC_VERSION", new QoreBigIntNode(_SC_VERSION), bigIntTypeInfo);
    #endif
    // #ifdef _SC_BC_BASE_MAX    9
    #ifdef _SC_BC_BASE_MAX
    SysconfNS.addConstant("SC_BC_BASE_MAX", new QoreBigIntNode(_SC_BC_BASE_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_BC_DIM_MAX   10
    #ifdef _SC_BC_DIM_MAX
    SysconfNS.addConstant("SC_BC_DIM_MAX", new QoreBigIntNode(_SC_BC_DIM_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_BC_SCALE_MAX  11
    #ifdef _SC_BC_SCALE_MAX
    SysconfNS.addConstant("SC_BC_SCALE_MAX", new QoreBigIntNode(_SC_BC_SCALE_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_BC_STRING_MAX  12
    #ifdef _SC_BC_STRING_MAX
    SysconfNS.addConstant("SC_BC_STRING_MAX", new QoreBigIntNode(_SC_BC_STRING_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_COLL_WEIGHTS_MAX  13
    #ifdef _SC_COLL_WEIGHTS_MAX
    SysconfNS.addConstant("SC_COLL_WEIGHTS_MAX", new QoreBigIntNode(_SC_COLL_WEIGHTS_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_EXPR_NEST_MAX  14
    #ifdef _SC_EXPR_NEST_MAX
    SysconfNS.addConstant("SC_EXPR_NEST_MAX", new QoreBigIntNode(_SC_EXPR_NEST_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_LINE_MAX   15
    #ifdef _SC_LINE_MAX
    SysconfNS.addConstant("SC_LINE_MAX", new QoreBigIntNode(_SC_LINE_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_RE_DUP_MAX   16
    #ifdef _SC_RE_DUP_MAX
    SysconfNS.addConstant("SC_RE_DUP_MAX", new QoreBigIntNode(_SC_RE_DUP_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_VERSION   17
    #ifdef _SC_2_VERSION
    SysconfNS.addConstant("SC_2_VERSION", new QoreBigIntNode(_SC_2_VERSION), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_C_BIND   18
    #ifdef _SC_2_C_BIND
    SysconfNS.addConstant("SC_2_C_BIND", new QoreBigIntNode(_SC_2_C_BIND), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_C_DEV   19
    #ifdef _SC_2_C_DEV
    SysconfNS.addConstant("SC_2_C_DEV", new QoreBigIntNode(_SC_2_C_DEV), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_CHAR_TERM   20
    #ifdef _SC_2_CHAR_TERM
    SysconfNS.addConstant("SC_2_CHAR_TERM", new QoreBigIntNode(_SC_2_CHAR_TERM), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_FORT_DEV   21
    #ifdef _SC_2_FORT_DEV
    SysconfNS.addConstant("SC_2_FORT_DEV", new QoreBigIntNode(_SC_2_FORT_DEV), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_FORT_RUN   22
    #ifdef _SC_2_FORT_RUN
    SysconfNS.addConstant("SC_2_FORT_RUN", new QoreBigIntNode(_SC_2_FORT_RUN), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_LOCALEDEF   23
    #ifdef _SC_2_LOCALEDEF
    SysconfNS.addConstant("SC_2_LOCALEDEF", new QoreBigIntNode(_SC_2_LOCALEDEF), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_SW_DEV   24
    #ifdef _SC_2_SW_DEV
    SysconfNS.addConstant("SC_2_SW_DEV", new QoreBigIntNode(_SC_2_SW_DEV), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_UPE   25
    #ifdef _SC_2_UPE
    SysconfNS.addConstant("SC_2_UPE", new QoreBigIntNode(_SC_2_UPE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_STREAM_MAX   26
    #ifdef _SC_STREAM_MAX
    SysconfNS.addConstant("SC_STREAM_MAX", new QoreBigIntNode(_SC_STREAM_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TZNAME_MAX   27
    #ifdef _SC_TZNAME_MAX
    SysconfNS.addConstant("SC_TZNAME_MAX", new QoreBigIntNode(_SC_TZNAME_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_ASYNCHRONOUS_IO  28
    #ifdef _SC_ASYNCHRONOUS_IO
    SysconfNS.addConstant("SC_ASYNCHRONOUS_IO", new QoreBigIntNode(_SC_ASYNCHRONOUS_IO), bigIntTypeInfo);
    #endif
    // #ifdef _SC_PAGESIZE   29
    #ifdef _SC_PAGESIZE
    SysconfNS.addConstant("SC_PAGESIZE", new QoreBigIntNode(_SC_PAGESIZE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MEMLOCK   30
    #ifdef _SC_MEMLOCK
    SysconfNS.addConstant("SC_MEMLOCK", new QoreBigIntNode(_SC_MEMLOCK), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MEMLOCK_RANGE  31
    #ifdef _SC_MEMLOCK_RANGE
    SysconfNS.addConstant("SC_MEMLOCK_RANGE", new QoreBigIntNode(_SC_MEMLOCK_RANGE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MEMORY_PROTECTION  32
    #ifdef _SC_MEMORY_PROTECTION
    SysconfNS.addConstant("SC_MEMORY_PROTECTION", new QoreBigIntNode(_SC_MEMORY_PROTECTION), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MESSAGE_PASSING  33
    #ifdef _SC_MESSAGE_PASSING
    SysconfNS.addConstant("SC_MESSAGE_PASSING", new QoreBigIntNode(_SC_MESSAGE_PASSING), bigIntTypeInfo);
    #endif
    // #ifdef _SC_PRIORITIZED_IO  34
    #ifdef _SC_PRIORITIZED_IO
    SysconfNS.addConstant("SC_PRIORITIZED_IO", new QoreBigIntNode(_SC_PRIORITIZED_IO), bigIntTypeInfo);
    #endif
    // #ifdef _SC_PRIORITY_SCHEDULING  35
    #ifdef _SC_PRIORITY_SCHEDULING
    SysconfNS.addConstant("SC_PRIORITY_SCHEDULING", new QoreBigIntNode(_SC_PRIORITY_SCHEDULING), bigIntTypeInfo);
    #endif
    // #ifdef _SC_REALTIME_SIGNALS  36
    #ifdef _SC_REALTIME_SIGNALS
    SysconfNS.addConstant("SC_REALTIME_SIGNALS", new QoreBigIntNode(_SC_REALTIME_SIGNALS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SEMAPHORES   37
    #ifdef _SC_SEMAPHORES
    SysconfNS.addConstant("SC_SEMAPHORES", new QoreBigIntNode(_SC_SEMAPHORES), bigIntTypeInfo);
    #endif
    // #ifdef _SC_FSYNC   38
    #ifdef _SC_FSYNC
    SysconfNS.addConstant("SC_FSYNC", new QoreBigIntNode(_SC_FSYNC), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SHARED_MEMORY_OBJECTS  39
    #ifdef _SC_SHARED_MEMORY_OBJECTS
    SysconfNS.addConstant("SC_SHARED_MEMORY_OBJECTS", new QoreBigIntNode(_SC_SHARED_MEMORY_OBJECTS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SYNCHRONIZED_IO  40
    #ifdef _SC_SYNCHRONIZED_IO
    SysconfNS.addConstant("SC_SYNCHRONIZED_IO", new QoreBigIntNode(_SC_SYNCHRONIZED_IO), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TIMERS   41
    #ifdef _SC_TIMERS
    SysconfNS.addConstant("SC_TIMERS", new QoreBigIntNode(_SC_TIMERS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_AIO_LISTIO_MAX  42
    #ifdef _SC_AIO_LISTIO_MAX
    SysconfNS.addConstant("SC_AIO_LISTIO_MAX", new QoreBigIntNode(_SC_AIO_LISTIO_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_AIO_MAX   43
    #ifdef _SC_AIO_MAX
    SysconfNS.addConstant("SC_AIO_MAX", new QoreBigIntNode(_SC_AIO_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_AIO_PRIO_DELTA_MAX  44
    #ifdef _SC_AIO_PRIO_DELTA_MAX
    SysconfNS.addConstant("SC_AIO_PRIO_DELTA_MAX", new QoreBigIntNode(_SC_AIO_PRIO_DELTA_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_DELAYTIMER_MAX  45
    #ifdef _SC_DELAYTIMER_MAX
    SysconfNS.addConstant("SC_DELAYTIMER_MAX", new QoreBigIntNode(_SC_DELAYTIMER_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MQ_OPEN_MAX   46
    #ifdef _SC_MQ_OPEN_MAX
    SysconfNS.addConstant("SC_MQ_OPEN_MAX", new QoreBigIntNode(_SC_MQ_OPEN_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MAPPED_FILES  47 /* swap _SC_PAGESIZE vs. BSD */
    #ifdef _SC_MAPPED_FILES
    SysconfNS.addConstant("SC_MAPPED_FILES", new QoreBigIntNode(_SC_MAPPED_FILES), bigIntTypeInfo);
    #endif
    // #ifdef _SC_RTSIG_MAX   48
    #ifdef _SC_RTSIG_MAX
    SysconfNS.addConstant("SC_RTSIG_MAX", new QoreBigIntNode(_SC_RTSIG_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SEM_NSEMS_MAX  49
    #ifdef _SC_SEM_NSEMS_MAX
    SysconfNS.addConstant("SC_SEM_NSEMS_MAX", new QoreBigIntNode(_SC_SEM_NSEMS_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SEM_VALUE_MAX  50
    #ifdef _SC_SEM_VALUE_MAX
    SysconfNS.addConstant("SC_SEM_VALUE_MAX", new QoreBigIntNode(_SC_SEM_VALUE_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SIGQUEUE_MAX  51
    #ifdef _SC_SIGQUEUE_MAX
    SysconfNS.addConstant("SC_SIGQUEUE_MAX", new QoreBigIntNode(_SC_SIGQUEUE_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TIMER_MAX   52
    #ifdef _SC_TIMER_MAX
    SysconfNS.addConstant("SC_TIMER_MAX", new QoreBigIntNode(_SC_TIMER_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_NPROCESSORS_CONF  57
    #ifdef _SC_NPROCESSORS_CONF
    SysconfNS.addConstant("SC_NPROCESSORS_CONF", new QoreBigIntNode(_SC_NPROCESSORS_CONF), bigIntTypeInfo);
    #endif
    // #ifdef _SC_NPROCESSORS_ONLN  58
    #ifdef _SC_NPROCESSORS_ONLN
    SysconfNS.addConstant("SC_NPROCESSORS_ONLN", new QoreBigIntNode(_SC_NPROCESSORS_ONLN), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_PBS   59
    #ifdef _SC_2_PBS
    SysconfNS.addConstant("SC_2_PBS", new QoreBigIntNode(_SC_2_PBS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_PBS_ACCOUNTING  60
    #ifdef _SC_2_PBS_ACCOUNTING
    SysconfNS.addConstant("SC_2_PBS_ACCOUNTING", new QoreBigIntNode(_SC_2_PBS_ACCOUNTING), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_PBS_CHECKPOINT  61
    #ifdef _SC_2_PBS_CHECKPOINT
    SysconfNS.addConstant("SC_2_PBS_CHECKPOINT", new QoreBigIntNode(_SC_2_PBS_CHECKPOINT), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_PBS_LOCATE  62
    #ifdef _SC_2_PBS_LOCATE
    SysconfNS.addConstant("SC_2_PBS_LOCATE", new QoreBigIntNode(_SC_2_PBS_LOCATE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_PBS_MESSAGE  63
    #ifdef _SC_2_PBS_MESSAGE
    SysconfNS.addConstant("SC_2_PBS_MESSAGE", new QoreBigIntNode(_SC_2_PBS_MESSAGE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_2_PBS_TRACK   64
    #ifdef _SC_2_PBS_TRACK
    SysconfNS.addConstant("SC_2_PBS_TRACK", new QoreBigIntNode(_SC_2_PBS_TRACK), bigIntTypeInfo);
    #endif
    // #ifdef _SC_ADVISORY_INFO  65
    #ifdef _SC_ADVISORY_INFO
    SysconfNS.addConstant("SC_ADVISORY_INFO", new QoreBigIntNode(_SC_ADVISORY_INFO), bigIntTypeInfo);
    #endif
    // #ifdef _SC_BARRIERS   66
    #ifdef _SC_BARRIERS
    SysconfNS.addConstant("SC_BARRIERS", new QoreBigIntNode(_SC_BARRIERS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_CLOCK_SELECTION  67
    #ifdef _SC_CLOCK_SELECTION
    SysconfNS.addConstant("SC_CLOCK_SELECTION", new QoreBigIntNode(_SC_CLOCK_SELECTION), bigIntTypeInfo);
    #endif
    // #ifdef _SC_CPUTIME   68
    #ifdef _SC_CPUTIME
    SysconfNS.addConstant("SC_CPUTIME", new QoreBigIntNode(_SC_CPUTIME), bigIntTypeInfo);
    #endif
    // #ifdef _SC_FILE_LOCKING  69
    #ifdef _SC_FILE_LOCKING
    SysconfNS.addConstant("SC_FILE_LOCKING", new QoreBigIntNode(_SC_FILE_LOCKING), bigIntTypeInfo);
    #endif
    // #ifdef _SC_GETGR_R_SIZE_MAX  70
    #ifdef _SC_GETGR_R_SIZE_MAX
    SysconfNS.addConstant("SC_GETGR_R_SIZE_MAX", new QoreBigIntNode(_SC_GETGR_R_SIZE_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_GETPW_R_SIZE_MAX  71
    #ifdef _SC_GETPW_R_SIZE_MAX
    SysconfNS.addConstant("SC_GETPW_R_SIZE_MAX", new QoreBigIntNode(_SC_GETPW_R_SIZE_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_HOST_NAME_MAX  72
    #ifdef _SC_HOST_NAME_MAX
    SysconfNS.addConstant("SC_HOST_NAME_MAX", new QoreBigIntNode(_SC_HOST_NAME_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_LOGIN_NAME_MAX  73
    #ifdef _SC_LOGIN_NAME_MAX
    SysconfNS.addConstant("SC_LOGIN_NAME_MAX", new QoreBigIntNode(_SC_LOGIN_NAME_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MONOTONIC_CLOCK  74
    #ifdef _SC_MONOTONIC_CLOCK
    SysconfNS.addConstant("SC_MONOTONIC_CLOCK", new QoreBigIntNode(_SC_MONOTONIC_CLOCK), bigIntTypeInfo);
    #endif
    // #ifdef _SC_MQ_PRIO_MAX   75
    #ifdef _SC_MQ_PRIO_MAX
    SysconfNS.addConstant("SC_MQ_PRIO_MAX", new QoreBigIntNode(_SC_MQ_PRIO_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_READER_WRITER_LOCKS  76
    #ifdef _SC_READER_WRITER_LOCKS
    SysconfNS.addConstant("SC_READER_WRITER_LOCKS", new QoreBigIntNode(_SC_READER_WRITER_LOCKS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_REGEXP   77
    #ifdef _SC_REGEXP
    SysconfNS.addConstant("SC_REGEXP", new QoreBigIntNode(_SC_REGEXP), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SHELL   78
    #ifdef _SC_SHELL
    SysconfNS.addConstant("SC_SHELL", new QoreBigIntNode(_SC_SHELL), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SPAWN   79
    #ifdef _SC_SPAWN
    SysconfNS.addConstant("SC_SPAWN", new QoreBigIntNode(_SC_SPAWN), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SPIN_LOCKS   80
    #ifdef _SC_SPIN_LOCKS
    SysconfNS.addConstant("SC_SPIN_LOCKS", new QoreBigIntNode(_SC_SPIN_LOCKS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SPORADIC_SERVER  81
    #ifdef _SC_SPORADIC_SERVER
    SysconfNS.addConstant("SC_SPORADIC_SERVER", new QoreBigIntNode(_SC_SPORADIC_SERVER), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_ATTR_STACKADDR 82
    #ifdef _SC_THREAD_ATTR_STACKADDR
    SysconfNS.addConstant("SC_THREAD_ATTR_STACKADDR", new QoreBigIntNode(_SC_THREAD_ATTR_STACKADDR), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_ATTR_STACKSIZE 83
    #ifdef _SC_THREAD_ATTR_STACKSIZE
    SysconfNS.addConstant("SC_THREAD_ATTR_STACKSIZE", new QoreBigIntNode(_SC_THREAD_ATTR_STACKSIZE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_CPUTIME  84
    #ifdef _SC_THREAD_CPUTIME
    SysconfNS.addConstant("SC_THREAD_CPUTIME", new QoreBigIntNode(_SC_THREAD_CPUTIME), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_DESTRUCTOR_ITERATIONS 85
    #ifdef _SC_THREAD_DESTRUCTOR_ITERATIONS
    SysconfNS.addConstant("SC_THREAD_DESTRUCTOR_ITERATIONS", new QoreBigIntNode(_SC_THREAD_DESTRUCTOR_ITERATIONS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_KEYS_MAX  86
    #ifdef _SC_THREAD_KEYS_MAX
    SysconfNS.addConstant("SC_THREAD_KEYS_MAX", new QoreBigIntNode(_SC_THREAD_KEYS_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_PRIO_INHERIT  87
    #ifdef _SC_THREAD_PRIO_INHERIT
    SysconfNS.addConstant("SC_THREAD_PRIO_INHERIT", new QoreBigIntNode(_SC_THREAD_PRIO_INHERIT), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_PRIO_PROTECT  88
    #ifdef _SC_THREAD_PRIO_PROTECT
    SysconfNS.addConstant("SC_THREAD_PRIO_PROTECT", new QoreBigIntNode(_SC_THREAD_PRIO_PROTECT), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_PRIORITY_SCHEDULING 89
    #ifdef _SC_THREAD_PRIORITY_SCHEDULING
    SysconfNS.addConstant("SC_THREAD_PRIORITY_SCHEDULING", new QoreBigIntNode(_SC_THREAD_PRIORITY_SCHEDULING), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_PROCESS_SHARED 90
    #ifdef _SC_THREAD_PROCESS_SHARED
    SysconfNS.addConstant("SC_THREAD_PROCESS_SHARED", new QoreBigIntNode(_SC_THREAD_PROCESS_SHARED), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_SAFE_FUNCTIONS 91
    #ifdef _SC_THREAD_SAFE_FUNCTIONS
    SysconfNS.addConstant("SC_THREAD_SAFE_FUNCTIONS", new QoreBigIntNode(_SC_THREAD_SAFE_FUNCTIONS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_SPORADIC_SERVER 92
    #ifdef _SC_THREAD_SPORADIC_SERVER
    SysconfNS.addConstant("SC_THREAD_SPORADIC_SERVER", new QoreBigIntNode(_SC_THREAD_SPORADIC_SERVER), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_STACK_MIN  93
    #ifdef _SC_THREAD_STACK_MIN
    SysconfNS.addConstant("SC_THREAD_STACK_MIN", new QoreBigIntNode(_SC_THREAD_STACK_MIN), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREAD_THREADS_MAX  94
    #ifdef _SC_THREAD_THREADS_MAX
    SysconfNS.addConstant("SC_THREAD_THREADS_MAX", new QoreBigIntNode(_SC_THREAD_THREADS_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TIMEOUTS   95
    #ifdef _SC_TIMEOUTS
    SysconfNS.addConstant("SC_TIMEOUTS", new QoreBigIntNode(_SC_TIMEOUTS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_THREADS   96
    #ifdef _SC_THREADS
    SysconfNS.addConstant("SC_THREADS", new QoreBigIntNode(_SC_THREADS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE   97
    #ifdef _SC_TRACE
    SysconfNS.addConstant("SC_TRACE", new QoreBigIntNode(_SC_TRACE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE_EVENT_FILTER  98
    #ifdef _SC_TRACE_EVENT_FILTER
    SysconfNS.addConstant("SC_TRACE_EVENT_FILTER", new QoreBigIntNode(_SC_TRACE_EVENT_FILTER), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE_INHERIT  99
    #ifdef _SC_TRACE_INHERIT
    SysconfNS.addConstant("SC_TRACE_INHERIT", new QoreBigIntNode(_SC_TRACE_INHERIT), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE_LOG   100
    #ifdef _SC_TRACE_LOG
    SysconfNS.addConstant("SC_TRACE_LOG", new QoreBigIntNode(_SC_TRACE_LOG), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TTY_NAME_MAX  101
    #ifdef _SC_TTY_NAME_MAX
    SysconfNS.addConstant("SC_TTY_NAME_MAX", new QoreBigIntNode(_SC_TTY_NAME_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TYPED_MEMORY_OBJECTS 102
    #ifdef _SC_TYPED_MEMORY_OBJECTS
    SysconfNS.addConstant("SC_TYPED_MEMORY_OBJECTS", new QoreBigIntNode(_SC_TYPED_MEMORY_OBJECTS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_V6_ILP32_OFF32  103
    #ifdef _SC_V6_ILP32_OFF32
    SysconfNS.addConstant("SC_V6_ILP32_OFF32", new QoreBigIntNode(_SC_V6_ILP32_OFF32), bigIntTypeInfo);
    #endif
    // #ifdef _SC_V6_ILP32_OFFBIG  104
    #ifdef _SC_V6_ILP32_OFFBIG
    SysconfNS.addConstant("SC_V6_ILP32_OFFBIG", new QoreBigIntNode(_SC_V6_ILP32_OFFBIG), bigIntTypeInfo);
    #endif
    // #ifdef _SC_V6_LP64_OFF64  105
    #ifdef _SC_V6_LP64_OFF64
    SysconfNS.addConstant("SC_V6_LP64_OFF64", new QoreBigIntNode(_SC_V6_LP64_OFF64), bigIntTypeInfo);
    #endif
    // #ifdef _SC_V6_LPBIG_OFFBIG  106
    #ifdef _SC_V6_LPBIG_OFFBIG
    SysconfNS.addConstant("SC_V6_LPBIG_OFFBIG", new QoreBigIntNode(_SC_V6_LPBIG_OFFBIG), bigIntTypeInfo);
    #endif
    // #ifdef _SC_IPV6   118
    #ifdef _SC_IPV6
    SysconfNS.addConstant("SC_IPV6", new QoreBigIntNode(_SC_IPV6), bigIntTypeInfo);
    #endif
    // #ifdef _SC_RAW_SOCKETS   119
    #ifdef _SC_RAW_SOCKETS
    SysconfNS.addConstant("SC_RAW_SOCKETS", new QoreBigIntNode(_SC_RAW_SOCKETS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SYMLOOP_MAX   120
    #ifdef _SC_SYMLOOP_MAX
    SysconfNS.addConstant("SC_SYMLOOP_MAX", new QoreBigIntNode(_SC_SYMLOOP_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_ATEXIT_MAX   107
    #ifdef _SC_ATEXIT_MAX
    SysconfNS.addConstant("SC_ATEXIT_MAX", new QoreBigIntNode(_SC_ATEXIT_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_IOV_MAX   56
    #ifdef _SC_IOV_MAX
    SysconfNS.addConstant("SC_IOV_MAX", new QoreBigIntNode(_SC_IOV_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_PAGE_SIZE   _SC_PAGESIZE
    #ifdef _SC_PAGE_SIZE
    SysconfNS.addConstant("SC_PAGE_SIZE", new QoreBigIntNode(_SC_PAGE_SIZE), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_CRYPT   108
    #ifdef _SC_XOPEN_CRYPT
    SysconfNS.addConstant("SC_XOPEN_CRYPT", new QoreBigIntNode(_SC_XOPEN_CRYPT), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_ENH_I18N  109
    #ifdef _SC_XOPEN_ENH_I18N
    SysconfNS.addConstant("SC_XOPEN_ENH_I18N", new QoreBigIntNode(_SC_XOPEN_ENH_I18N), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_LEGACY  110
    #ifdef _SC_XOPEN_LEGACY
    SysconfNS.addConstant("SC_XOPEN_LEGACY", new QoreBigIntNode(_SC_XOPEN_LEGACY), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_REALTIME  111
    #ifdef _SC_XOPEN_REALTIME
    SysconfNS.addConstant("SC_XOPEN_REALTIME", new QoreBigIntNode(_SC_XOPEN_REALTIME), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_REALTIME_THREADS 112
    #ifdef _SC_XOPEN_REALTIME_THREADS
    SysconfNS.addConstant("SC_XOPEN_REALTIME_THREADS", new QoreBigIntNode(_SC_XOPEN_REALTIME_THREADS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_SHM   113
    #ifdef _SC_XOPEN_SHM
    SysconfNS.addConstant("SC_XOPEN_SHM", new QoreBigIntNode(_SC_XOPEN_SHM), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_STREAMS  114
    #ifdef _SC_XOPEN_STREAMS
    SysconfNS.addConstant("SC_XOPEN_STREAMS", new QoreBigIntNode(_SC_XOPEN_STREAMS), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_UNIX   115
    #ifdef _SC_XOPEN_UNIX
    SysconfNS.addConstant("SC_XOPEN_UNIX", new QoreBigIntNode(_SC_XOPEN_UNIX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_VERSION  116
    #ifdef _SC_XOPEN_VERSION
    SysconfNS.addConstant("SC_XOPEN_VERSION", new QoreBigIntNode(_SC_XOPEN_VERSION), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XOPEN_XCU_VERSION  121
    #ifdef _SC_XOPEN_XCU_VERSION
    SysconfNS.addConstant("SC_XOPEN_XCU_VERSION", new QoreBigIntNode(_SC_XOPEN_XCU_VERSION), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XBS5_ILP32_OFF32  122
    #ifdef _SC_XBS5_ILP32_OFF32
    SysconfNS.addConstant("SC_XBS5_ILP32_OFF32", new QoreBigIntNode(_SC_XBS5_ILP32_OFF32), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XBS5_ILP32_OFFBIG  123
    #ifdef _SC_XBS5_ILP32_OFFBIG
    SysconfNS.addConstant("SC_XBS5_ILP32_OFFBIG", new QoreBigIntNode(_SC_XBS5_ILP32_OFFBIG), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XBS5_LP64_OFF64  124
    #ifdef _SC_XBS5_LP64_OFF64
    SysconfNS.addConstant("SC_XBS5_LP64_OFF64", new QoreBigIntNode(_SC_XBS5_LP64_OFF64), bigIntTypeInfo);
    #endif
    // #ifdef _SC_XBS5_LPBIG_OFFBIG  125
    #ifdef _SC_XBS5_LPBIG_OFFBIG
    SysconfNS.addConstant("SC_XBS5_LPBIG_OFFBIG", new QoreBigIntNode(_SC_XBS5_LPBIG_OFFBIG), bigIntTypeInfo);
    #endif
    // #ifdef _SC_SS_REPL_MAX   126
    #ifdef _SC_SS_REPL_MAX
    SysconfNS.addConstant("SC_SS_REPL_MAX", new QoreBigIntNode(_SC_SS_REPL_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE_EVENT_NAME_MAX 127
    #ifdef _SC_TRACE_EVENT_NAME_MAX
    SysconfNS.addConstant("SC_TRACE_EVENT_NAME_MAX", new QoreBigIntNode(_SC_TRACE_EVENT_NAME_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE_NAME_MAX  128
    #ifdef _SC_TRACE_NAME_MAX
    SysconfNS.addConstant("SC_TRACE_NAME_MAX", new QoreBigIntNode(_SC_TRACE_NAME_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE_SYS_MAX  129
    #ifdef _SC_TRACE_SYS_MAX
    SysconfNS.addConstant("SC_TRACE_SYS_MAX", new QoreBigIntNode(_SC_TRACE_SYS_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_TRACE_USER_EVENT_MAX 130
    #ifdef _SC_TRACE_USER_EVENT_MAX
    SysconfNS.addConstant("SC_TRACE_USER_EVENT_MAX", new QoreBigIntNode(_SC_TRACE_USER_EVENT_MAX), bigIntTypeInfo);
    #endif
    // #ifdef _SC_PASS_MAX   131
    #ifdef _SC_PASS_MAX
    SysconfNS.addConstant("SC_PASS_MAX", new QoreBigIntNode(_SC_PASS_MAX), bigIntTypeInfo);
    #endif

#endif

#ifdef HAVE_CONFSTR
    // #define _CS_PATH                                1
    #ifdef _CS_PATH
    ConfstrNS.addConstant("CS_PATH", new QoreBigIntNode(_CS_PATH), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_ILP32_OFF32_CFLAGS         2
    #ifdef _CS_POSIX_V6_ILP32_OFF32_CFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_ILP32_OFF32_CFLAGS", new QoreBigIntNode(_CS_POSIX_V6_ILP32_OFF32_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_ILP32_OFF32_LDFLAGS        3
    #ifdef _CS_POSIX_V6_ILP32_OFF32_LDFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_ILP32_OFF32_LDFLAGS", new QoreBigIntNode(_CS_POSIX_V6_ILP32_OFF32_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_ILP32_OFF32_LIBS           4
    #ifdef _CS_POSIX_V6_ILP32_OFF32_LIBS
    ConfstrNS.addConstant("CS_POSIX_V6_ILP32_OFF32_LIBS", new QoreBigIntNode(_CS_POSIX_V6_ILP32_OFF32_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS        5
    #ifdef _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_ILP32_OFFBIG_CFLAGS", new QoreBigIntNode(_CS_POSIX_V6_ILP32_OFFBIG_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS       6
    #ifdef _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS", new QoreBigIntNode(_CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_ILP32_OFFBIG_LIBS          7
    #ifdef _CS_POSIX_V6_ILP32_OFFBIG_LIBS
    ConfstrNS.addConstant("CS_POSIX_V6_ILP32_OFFBIG_LIBS", new QoreBigIntNode(_CS_POSIX_V6_ILP32_OFFBIG_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_LP64_OFF64_CFLAGS          8
    #ifdef _CS_POSIX_V6_LP64_OFF64_CFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_LP64_OFF64_CFLAGS", new QoreBigIntNode(_CS_POSIX_V6_LP64_OFF64_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_LP64_OFF64_LDFLAGS         9
    #ifdef _CS_POSIX_V6_LP64_OFF64_LDFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_LP64_OFF64_LDFLAGS", new QoreBigIntNode(_CS_POSIX_V6_LP64_OFF64_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_LP64_OFF64_LIBS            10
    #ifdef _CS_POSIX_V6_LP64_OFF64_LIBS
    ConfstrNS.addConstant("CS_POSIX_V6_LP64_OFF64_LIBS", new QoreBigIntNode(_CS_POSIX_V6_LP64_OFF64_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS        11
    #ifdef _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS", new QoreBigIntNode(_CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS       12
    #ifdef _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS
    ConfstrNS.addConstant("CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS", new QoreBigIntNode(_CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_LPBIG_OFFBIG_LIBS          13
    #ifdef _CS_POSIX_V6_LPBIG_OFFBIG_LIBS
    ConfstrNS.addConstant("CS_POSIX_V6_LPBIG_OFFBIG_LIBS", new QoreBigIntNode(_CS_POSIX_V6_LPBIG_OFFBIG_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_POSIX_V6_WIDTH_RESTRICTED_ENVS      14
    #ifdef _CS_POSIX_V6_WIDTH_RESTRICTED_ENVS
    ConfstrNS.addConstant("CS_POSIX_V6_WIDTH_RESTRICTED_ENVS", new QoreBigIntNode(_CS_POSIX_V6_WIDTH_RESTRICTED_ENVS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFF32_CFLAGS             20
    #ifdef _CS_XBS5_ILP32_OFF32_CFLAGS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFF32_CFLAGS", new QoreBigIntNode(_CS_XBS5_ILP32_OFF32_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFF32_LDFLAGS            21
    #ifdef _CS_XBS5_ILP32_OFF32_LDFLAGS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFF32_LDFLAGS", new QoreBigIntNode(_CS_XBS5_ILP32_OFF32_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFF32_LIBS               22
    #ifdef _CS_XBS5_ILP32_OFF32_LIBS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFF32_LIBS", new QoreBigIntNode(_CS_XBS5_ILP32_OFF32_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFF32_LINTFLAGS          23
    #ifdef _CS_XBS5_ILP32_OFF32_LINTFLAGS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFF32_LINTFLAGS", new QoreBigIntNode(_CS_XBS5_ILP32_OFF32_LINTFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFFBIG_CFLAGS            24
    #ifdef _CS_XBS5_ILP32_OFFBIG_CFLAGS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFFBIG_CFLAGS", new QoreBigIntNode(_CS_XBS5_ILP32_OFFBIG_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFFBIG_LDFLAGS           25
    #ifdef _CS_XBS5_ILP32_OFFBIG_LDFLAGS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFFBIG_LDFLAGS", new QoreBigIntNode(_CS_XBS5_ILP32_OFFBIG_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFFBIG_LIBS              26
    #ifdef _CS_XBS5_ILP32_OFFBIG_LIBS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFFBIG_LIBS", new QoreBigIntNode(_CS_XBS5_ILP32_OFFBIG_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_ILP32_OFFBIG_LINTFLAGS         27
    #ifdef _CS_XBS5_ILP32_OFFBIG_LINTFLAGS
    ConfstrNS.addConstant("CS_XBS5_ILP32_OFFBIG_LINTFLAGS", new QoreBigIntNode(_CS_XBS5_ILP32_OFFBIG_LINTFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LP64_OFF64_CFLAGS              28
    #ifdef _CS_XBS5_LP64_OFF64_CFLAGS
    ConfstrNS.addConstant("CS_XBS5_LP64_OFF64_CFLAGS", new QoreBigIntNode(_CS_XBS5_LP64_OFF64_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LP64_OFF64_LDFLAGS             29
    #ifdef _CS_XBS5_LP64_OFF64_LDFLAGS
    ConfstrNS.addConstant("CS_XBS5_LP64_OFF64_LDFLAGS", new QoreBigIntNode(_CS_XBS5_LP64_OFF64_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LP64_OFF64_LIBS                30
    #ifdef _CS_XBS5_LP64_OFF64_LIBS
    ConfstrNS.addConstant("CS_XBS5_LP64_OFF64_LIBS", new QoreBigIntNode(_CS_XBS5_LP64_OFF64_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LP64_OFF64_LINTFLAGS           31
    #ifdef _CS_XBS5_LP64_OFF64_LINTFLAGS
    ConfstrNS.addConstant("CS_XBS5_LP64_OFF64_LINTFLAGS", new QoreBigIntNode(_CS_XBS5_LP64_OFF64_LINTFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LPBIG_OFFBIG_CFLAGS            32
    #ifdef _CS_XBS5_LPBIG_OFFBIG_CFLAGS
    ConfstrNS.addConstant("CS_XBS5_LPBIG_OFFBIG_CFLAGS", new QoreBigIntNode(_CS_XBS5_LPBIG_OFFBIG_CFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LPBIG_OFFBIG_LDFLAGS           33
    #ifdef _CS_XBS5_LPBIG_OFFBIG_LDFLAGS
    ConfstrNS.addConstant("CS_XBS5_LPBIG_OFFBIG_LDFLAGS", new QoreBigIntNode(_CS_XBS5_LPBIG_OFFBIG_LDFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LPBIG_OFFBIG_LIBS              34
    #ifdef _CS_XBS5_LPBIG_OFFBIG_LIBS
    ConfstrNS.addConstant("CS_XBS5_LPBIG_OFFBIG_LIBS", new QoreBigIntNode(_CS_XBS5_LPBIG_OFFBIG_LIBS), bigIntTypeInfo);
    #endif
    // #define _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS         35
    #ifdef _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS
    ConfstrNS.addConstant("CS_XBS5_LPBIG_OFFBIG_LINTFLAGS", new QoreBigIntNode(_CS_XBS5_LPBIG_OFFBIG_LINTFLAGS), bigIntTypeInfo);
    #endif
    // #define _CS_DARWIN_USER_DIR                     65536
    #ifdef _CS_DARWIN_USER_DIR
    ConfstrNS.addConstant("CS_DARWIN_USER_DIR", new QoreBigIntNode(_CS_DARWIN_USER_DIR), bigIntTypeInfo);
    #endif
    // #define _CS_DARWIN_USER_TEMP_DIR                65537
    #ifdef _CS_DARWIN_USER_TEMP_DIR
    ConfstrNS.addConstant("CS_DARWIN_USER_TEMP_DIR", new QoreBigIntNode(_CS_DARWIN_USER_TEMP_DIR), bigIntTypeInfo);
    #endif
    // #define _CS_DARWIN_USER_CACHE_DIR               65538
    #ifdef _CS_DARWIN_USER_CACHE_DIR
    ConfstrNS.addConstant("CS_DARWIN_USER_CACHE_DIR", new QoreBigIntNode(_CS_DARWIN_USER_CACHE_DIR), bigIntTypeInfo);
    #endif

#endif

    builtinFunctions.add2("sysconf", f_sysconf, QC_CONSTANT, QDOM_DEFAULT, bigIntTypeInfo, 1, softBigIntTypeInfo, QORE_PARAM_NO_ARG);
    builtinFunctions.add2("sysconf_constants", f_sysconf_constants, QC_CONSTANT, QDOM_DEFAULT, hashTypeInfo);
    builtinFunctions.add2("confstr", f_confstr, QC_CONSTANT, QDOM_DEFAULT, stringTypeInfo, 1, softBigIntTypeInfo, QORE_PARAM_NO_ARG);
    builtinFunctions.add2("confstr_constants", f_confstr_constants, QC_CONSTANT, QDOM_DEFAULT, hashTypeInfo);

    return 0;                                                                                                        
}                                                                                                                   

void sysconf_module_ns_init(QoreNamespace *rns, QoreNamespace *qns) {                                                 
    qns->addNamespace(SysconfNS.copy());  
    qns->addNamespace(ConfstrNS.copy());
}                                                                                                                   

void sysconf_module_delete() {                                                                                        
   // nothing to do here in this case                                                                               
}    

/*
# a sample script to create constants from unistd.h file
my string $str = "
#ifdef _SC_ARG_MAX    1
#ifdef _SC_CHILD_MAX    2
...
#ifdef _SC_TRACE_USER_EVENT_MAX 130
#ifdef _SC_PASS_MAX   131
";

my list $l = split("\n", replace($str, "\t", " "));

foreach my $i in ($l) {
    my list $j = split(" ", $i);

    if (!elements $j)
        continue;
    
    my string $def = $j[1];
    my string $qore = substr($def, 1);

    printf("    // %s\n", $i);
    printf("    #ifdef %s\n", $def);
    printf("    SysconfNS.addConstant(\"%s\", new QoreBigIntNode(%s), bigIntTypeInfo);\n", $qore, $def);
    printf("    #endif\n");
}
*/

