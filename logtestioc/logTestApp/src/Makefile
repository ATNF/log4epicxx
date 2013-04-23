TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build the IOC support library

LIBRARY_IOC += logTestSupport

# Link locally-provided code into the support library,
# rather than directly into the IOC application.
logTestSupport_SRCS += dbSubExample.c
logTestSupport_SRCS += logTestHello.c
logTestSupport_SRCS += initTrace.c

logTestSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = logTest
# logTest.dbd will be created and installed
DBD += logTest.dbd

# logTest.dbd will be made up from these files:
logTest_DBD += base.dbd
logTest_DBD += dbSubExample.dbd
logTest_DBD += logTestHello.dbd
logTest_DBD += initTrace.dbd

# logTest_registerRecordDeviceDriver.cpp derives from logTest.dbd
logTest_SRCS += logTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
logTest_SRCS_DEFAULT += logTestMain.cpp
logTest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#logTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link in the code from the support library
logTest_LIBS += logTestSupport

# log4epicxx
USR_CPPFLAGS += -DLOGGER_PREFIX=\"mysite.\"
USR_CPPFLAGS += -DPACKAGE_NAME=\"logTest\"
logTest_LIBS += logging
logTest_DBD += logging.dbd
# log4epicxx

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

ifneq ($(SNCSEQ),)
    sncExample_SNCFLAGS += +r
    logTest_DBD += sncExample.dbd
    logTestSupport_SRCS += sncExample.stt
    logTestSupport_LIBS += seq pv
    logTest_LIBS += seq pv
endif

ifneq ($(ASYN),)
    # build asynPortDriver example
endif

# Finally link to the EPICS Base libraries
logTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

