TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
USR_CXXFLAGS_DEFAULT += -fno-operator-names
#Build the IOC application
LIBRARY_IOC = logging

# installed DBDs
DBD += logging.dbd
logging_DBD += base.dbd
logging_DBD += logSubroutines.dbd

# Add all the support libraries needed by this IOC
logging_LIBS += log4cxx
log4cxx_DIR = $(L4CPP)/lib

ifdef IOCLOG
logging_LIBS += askap_ioclog
askap_ioclog_DIR = $(IOCLOG)/lib
else
USR_CPPFLAGS += -DSTANDALONE_BUILD
logging_SRCS += Logging.cc
endif

logging_SRCS += logSubroutines.cpp
logging_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

