.. _log4cxx: http://logging.apache.org/log4cxx/
.. _Ganymede: http://ganymede.sourceforge.net/
.. _log4j-viewer: http://code.google.com/p/log4j-viewer/

Introduction
------------

log4epicxx is a logging framework for EPICS based on log4cxx_
The major benefits over existing EPICS logging are:

* separates IOC application logging from core EPICS logging
* provides a rich set of tools for sending output to different
  destinations and in different formats
* good performance
* configurable at runtime

Building
--------

#. requires a log4cxx development package installed or built from scratch
#. put this distribtion in you EPIC's module directory
#. set EPICS_BASE in configure/RELEASE
#. set L4CPP in configure/RELEASE to point to you log4cxx installation
#. make

.. _ioc-integration: 

Integration with IOC
--------------------

#. set LOGGING path in configure/RELEASE to the install directory of the log4epicsxx release
#. add the logging EPICS records to your DB Makefile::

    DB_INSTALLS += $(LOGGING)/db/logging.db

#. add the following to the IOC Makefile

    .. literalinclude:: ../logtestioc/logTestApp/src/Makefile
       :start-after: # log4epicxx
       :end-before: # log4epicxx

#. add epicxx_log_init() call to your st.cmd file

    .. literalinclude:: ../logtestioc/iocBoot/ioclogTest/st.cmd
       :start-after: # log4epicxx
       :end-before: # log4epicxx

#. load the logging records in you st.cmd file

    .. literalinclude:: ../logtestioc/iocBoot/ioclogTest/st.cmd
       :start-after: # log4epicxx records
       :end-before: # log4epicxx records

#. setup a logging configuration file, for example

    .. literalinclude:: ../logtestioc/iocBoot/ioclogTest/ioc.log_cfg

#. Add **CREATE_LOGGER** definitions to the source files

#. use **LOG_<LEVEL>** macros for logging

There is an example IOC *logtestioc* that has examples of logging from C, C++ and SNL

Logging API
-----------

.. function:: log4epicxx_init(filename) *iocsh function*

    initialises logging from the supplied configuration file *filename*.
    If filename is ommitted or is NULL, the environment variable **IOC_LOG_CONFIG**
    is used, if defined, or lastly ``ioc.log_cfg`` in the current directory.  If
    no log file can be found, a default configuration is applied.  This call should
    be placed in the IOC's ``st.cmd`` startup script

.. function:: CREATE_LOGGER(logName)

    used in C, C++ & SNL to create a logger instance called logName. **PACKAGE_NAME**
    should be defined before including ``ioclog/Logging.h``

.. function:: CREATE_IOC_LOGGER(logName)

    used in C, C++ & SNL to create a logger instance class member useful for
    multiple device asyn logging.  There must be an associated member variable::

       askap::ioclog::IocLogger   LOGGER

.. function:: CREATE_NAMED_LOGGER(logger, logName)

    used in C, C++ & SNL to create a named logger. Used when you want to create
    multiple loggers in a single module.

.. function:: LOG_<LEVEL>(fmt, ...)

   used in C, C++ & SNL.  Logs a message to the default logger at the level specified

.. function:: LOG_<LEVEL>2(LOGGER, fmt, ...)

   used in C, C++ & SNL.  Logs a message to the logger instance LOGGER at the level specified

The following logging levels are defined in order of increasing verbosity

#. OFF
#. FATAL
#. ERROR
#. WARN 
#. INFO 
#. DEBUG
#. TRACE

.. function:: LOG_PUSH_CONTEXT(context)

    for C & SNL, pushes the string *context* onto the logging context stack for the current thread

.. function:: LOG_POP_CONTEXT

    for C & SNL, pops the last logging context off the stack for the current thread

.. function:: LogContext(context)

    for C++, Auto scoped Logging Context class. Context will put put on stack on creation of class instance.
    On deletion of instance, context is automatically popped.

Asyn Driver Logging
-------------------

* use LOG_<LEVEL> macros instead of using asynPrintf or errlogPrintf
* for multiple device logging, can create a logger for each port instance

aSub logging
------------

* use C or C++ interface to log asub calls
* optionally use LogContext class or LOG_PUSH/POP_CONTEXT to log which PV is being processed

SNL logging
-----------

* using C interface can use logging macros inside SNL programs
* optionally using LOG_PUSH/POP_CONTEXT to log context info about which 
  state set & state clause is running
* a logger is defined like this in an SNL program

.. literalinclude:: ../logtestioc/logTestApp/src/sncExample.stt
   :start-after: /* log4epicxx
   :end-before: /* log4epicxx

Log Viewers
-----------

There are some good GUI tools for viewing log4j logs.  log4j-viewer_ is an Eclipse plugin that can
display logs via the SocketAppender.  It is a fork of the original plugin Ganymede_ (which hasn't
been updated in a long time).  Follow the install instructions and add the following appender to
your ioc.log_cfg::

    log4j.rootLogger=DEBUG, file, stdout, CSS
    ..
    ..
    log4j.appender.CSS=org.apache.log4j.net.SocketAppender
    log4j.appender.CSS.remoteHost=<hostname>
    log4j.appender.CSS.port=4445
    log4j.appender.CSS.locationInfo=true

Logging Control
---------------

The log4epicxx module provides a set
of EPICS records for controlling the logging
levels of each logger at runtime see :doc:`records_common`

Command Line Tool
-----------------

A command line tools is provided to set logger levels at runtime::

    scripts/set_logging <pv prefix to logging records>

will run a console which lists available loggers.  A command prompt allows input
to set logging levels.  The command syntax is::

    regex level

where:

regex
    a regular expression to match against a logger name

level
    a logging level, one of off, fatal, error, warn, info, debug or trace
