ASKAP EPICS Logging 
===================

.. _log4cxx: http://logging.apache.org/log4cxx/
.. _Ganymede: http://ganymede.sourceforge.net/
.. _log4j-viewer: http://code.google.com/p/log4j-viewer/

ASKAP EPICS logging makes use of log4cxx_ as a more configurable
logging framework for adding log information to ASKAP EPICS IOCs
The major benefits are:

* separates ASKAP driver logging from core EPICS logging
* provides a rich set of tools for sending output to different
  destinations and in different formats
* good performance
* configurable at runtime

modules:

Code/Base/ioclog
  log4cxx_ wrapper

Code/Base/EPICS/support/logging
  support for controlling logging levels at runtime
  through EPICS records

Logging API
-----------

.. function:: askap::ioclog::log_init(configFile)

    initialises logging from the supplied configuration file configFile

.. function:: CREATE_LOGGER(logName)

    creates a logger instance called logName. ASKAP_PACKAGE_NAME must be defined before
    including ioclog/Logging.h

.. function:: CREATE_IOC_LOGGER(logName)

    create a logger instance class member.  For the default logger, the
    member must be called **LOGGER**

.. function:: CREATE_NAMED_LOGGER(logger, logName)

    used when you want multiple loggers in a module

.. function:: LOG_<LEVEL>(fmt, ...)

   logs a message to the default logging at the level specified

.. function:: LOG_<LEVEL>2(LOGGER, fmt, ...)

   logs a message to the logger instance LOGGER at the level specified

The following logging levels are defined in order of increasing verbosity

#. OFF
#. FATAL
#. ERROR
#. WARN 
#. INFO 
#. DEBUG
#. TRACE

.. function:: LOG_PUSH_CONTEXT(context)

    for C & SNL, pushes the string _context_ onto the logging context stack for the current thread

.. function:: LOG_POP_CONTEXT

    for C & SNL, pops the last logging context off the stack for the current thread

.. function:: LogContext(context)

    for C++, Auto scoped Logging Context class. Context will put put on stack on creation of class instance.
    On deletion of instance, context is automatically popped.

Asyn Driver Logging
-------------------

* use LOG_<LEVEL> macros instead of using asynPrintf or errlogPrintf
* for multiple device logging, create a logger for each port instance

aSub logging
------------

* use C or C++ interface to log asub calls
* use LogContext class or LOG_PUSH/POP_CONTEXT to log which PV is being processed

SNL logging
-----------

* using C interface can use logging macros to get location info into log
* using LOG_PUSH/POP_CONTEXT to log context info about which state set & state clause is running

Logging Heirarchy
-----------------

log4cxx implements a logging heirarchy were a logger's effective level will be the 
first set level encountered in the heirarchy.

The heirarchy is determined based on the dotted names given to the loggers

Logging Initalisation & Configuration
-------------------------------------

see bmf/current/bmfApp/src/bmfMain.cpp example

need to call **askap::ioclog::log_init** with name of config file.  For deployed system we use a single
config file for all IOCs which the IOC gets via the IOC_LOG_CONFIG environment variable.

The deployed log configuration will create a log file in ``/askapdata/var/log/<iocname>.log``

When running and IOC from the development environment, the IOC opens ``ioc.log_cfg`` from the current
directory if IOC_LOG_CONFIG is not defined, see below for code snippet to put in iocMain

Log Viewers
-----------

There are some good GUI tools for viewing log4j logs.  log4j-viewer_ is an Eclipse plugin that can
display logs via the SocketAppender.  It is a fork of the original plugin Ganymede_ (which hasn't
been updated in a long time).  Follow the install instructions and add the following appender to
your ioc.log_cfg

::

    log4j.rootLogger=DEBUG, file, stdout, CSS
    ..
    ..
    log4j.appender.CSS=org.apache.log4j.net.SocketAppender
    log4j.appender.CSS.remoteHost=<hostname>
    log4j.appender.CSS.port=4445
    log4j.appender.CSS.locationInfo=true

Integration Steps
-----------------

replace bmf below with your IOC name

#. add dependencies

    ::

        l4cpp=3rdParty/log4cxx/log4cxx-0.10.0 
        ioclog=Code/Base/ioclog/current 
        logging=Code/Components/EPICS/support/logging/current 


#. add_support in build.py

    ::

        # logging
        builder.add_support("l4cpp")
        builder.add_support('ioclog')
        builder.add_support('logging')


#. add entries to iocApp/src/Makefile

    ::

        # logging
        bmf_LIBS += log4cxx
        bmf_LIBS += askap_ioclog
        bmf_LIBS += logging
        log4cxx_DIR = $(L4CPP)/lib
        askap_ioclog_DIR = $(IOCLOG)/lib

#. include ASKAP_PACKAGE_NAME (automatically generated
   from builder in iocname_version.h) before including
   the logging, eg in a ioc common header file

    ::

        #include "bmf_version.h"
        #include "ioclog/Logging.h" 

#. add log_init call to main

    ::

        #include "bmfCommon.h"

        using namespace askap;
        using namespace ioclog;

        CREATE_LOGGER(".main");

        int main(int argc,char *argv[])
        {
            char *logConfig = getenv("IOC_LOG_CONFIG");

            if (logConfig) {
                log_init(getenv("IOC_LOG_CONFIG"));
            }
            else {
                log_init("ioc.log_cfg");
            }

            if(argc>=2) {
                iocsh(argv[argc - 1]);
                epicsThreadSleep(0.2);
            }
            LOG_WARN("Starting Beamformer IOC");
            iocsh(NULL);
            epicsExit(0);
            return(0);
        }

#. add logging.db to your IOC common database

    ::

        file logging.template
        {
            {prefix=ma, antid=01:, ss=bmf:}
        }

#. and a dependency to the Db makefile

    ::
    
        bmf_DEPENDS = logging.db

#. add an ioc.log_cfg file to files/ioc<app> dir (only used when running from dev environment)

    ::

        # Set root logger level to DEBUG and add a couple of appenders
        log4j.rootLogger=WARN, file, stdout

        #enable logging to CSS via Ganymede log4j plugin
        #log4j.rootLogger=WARN, file, stdout, CSS

        # log to file
        log4j.appender.file=org.apache.log4j.RollingFileAppender
        log4j.appender.file.File=bmf.log
        log4j.appender.file.MaxFileSize=10MB
        log4j.appender.file.MaxBackupIndex=3
        log4j.appender.file.layout=org.apache.log4j.PatternLayout
        log4j.appender.file.layout.ConversionPattern=%d %-5p [%16t] %-20c %l - %m%n

        # log to stdout (but only WARN and higher)
        log4j.appender.stdout=org.apache.log4j.ConsoleAppender
        log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
        log4j.appender.stdout.layout.ConversionPattern=%-5p [%16t] %-20c %x %l - %m%n
        log4j.appender.stdout.Threshold=WARN

        log4j.appender.CSS=org.apache.log4j.net.SocketAppender
        log4j.appender.CSS.remoteHost=metropolis
        log4j.appender.CSS.port=4445
        log4j.appender.CSS.locationInfo=true

        #define the top level debugger
        log4j.logger.askap.bmf=DEBUG

        #only enable DEBUG for a single card
        log4j.lgger.askap.bmf.card01=DEBUG

Integration Steps for LogArchiver ICE Appender
----------------------------------------------

#. dependencies

    ::

        ice=3rdParty/Ice/Ice-3.4.1;Ice IceUtil
        logappenders=Code/Base/logappenders/current
        cppiceinterfaces=Code/Interfaces/cpp/current
        bdb=3rdParty/Berkeley-DB/db-4.8.30.NC

#. build.py

    ::

        # logging ice appender
        builder.add_support('bdb')
        builder.add_support('ice')
        builder.add_support('cppiceinterfaces')
        builder.add_support('logappenders')

#. Makefile

    ::

        # logging ice appender
        bmf_LIBS += db
        bmf_LIBS += Ice IceUtil
        bmf_LIBS += askap_iceinterfaces
        bmf_LIBS += askap_logappenders
        db_DIR = $(BDB)/lib
        Ice_DIR = $(ICE)/lib
        IceUtil_DIR = $(ICE)/lib
        askap_iceinterfaces_DIR = $(CPPICEINTERFACES)/lib
        askap_logappenders_DIR = $(LOGAPPENDERS)/lib

#. Logging Config

    ::

        log4j.rootLogger=INFO, file, stdout, REMOTE

        # log to LogArchiver (only WARNING and higher)
        log4j.appender.REMOTE=askap.IceAppender
        log4j.appender.REMOTE.locator_host=${LOG_HOST}
        log4j.appender.REMOTE.locator_port=4061
        log4j.appender.REMOTE.topic=logger
        log4j.appender.REMOTE.tag=epics
        log4j.appender.REMOTE.Threshold=WARN
    
Logging Control
---------------

This EPICS library IOC provides a set
of records for controlling the logging
levels of each logger at runtime see :doc:`records_common`

Command Line Tool
-----------------

A command line tools is provided to set logger levels at runtime::

    scripts/set_logging <pv prefix>

will run a console which lists available loggers.  A command prompt allows input
to set logging levels.  The command syntax is::

    regex level

where:

regex
    a regular expression to match against a logger name

level
    a logging level, one of off, fatal, error, warn, info, debug or trace

