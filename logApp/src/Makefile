TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
USR_CXXFLAGS_DEFAULT += -fno-operator-names
# Build the IOC application

LIBRARY_IOC = logging
# log.dbd will be created and installed
DBD += logging.dbd

# log.dbd will be mlog up from these files:
logging_DBD += logSubroutines.dbd

# Add all the support libraries needed by this IOC
logging_LIBS += boost_system
logging_LIBS += askap_adbe
logging_LIBS += log4cxx
askap_adbe_DIR = $(ADBE)/lib
log4cxx_DIR = $(LOG)/lib
boost_system_DIR = $(BOOST)/lib
expat_DIR = $(EXPAT)/lib

logging_SRCS += logSubroutines.cpp
logging_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

