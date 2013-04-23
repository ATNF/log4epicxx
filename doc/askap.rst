ASKAP Specific Build Instructions
=================================

Integration Steps
-----------------

#. add dependencies to dependencies.default

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
        bmf_DBD += logging.dbd
        bmf_LIBS += log4cxx
        bmf_LIBS += askap_ioclog
        bmf_LIBS += logging
        log4cxx_DIR = $(L4CPP)/lib
        askap_ioclog_DIR = $(IOCLOG)/lib

#. include ASKAP_PACKAGE_NAME (automatically generated
   from builder in iocname_version.h) before including
   the logging, eg in a ioc common header file

    ::

        #include "youriocname_version.h"
        #include "ioclog/Logging.h" 

#. follow standard log4epicxx :ref:`ioc-integration`
