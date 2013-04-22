#!../../bin/linux-x86/logTest
epicsEnvSet("ARCH","linux-x86")
epicsEnvSet("IOC","ioclogTest")
epicsEnvSet("TOP","/home/user/src/epics/iocs/logtest")
epicsEnvSet("MODULES","/home/user/src/epics/base-3.14.12.3/../modules")
epicsEnvSet("EPICS_BASE","/home/user/src/epics/base-3.14.12.3")
epicsEnvSet("SNCSEQ","/home/user/src/epics/base-3.14.12.3/../modules/seq-2.1.12")
epicsEnvSet("LOGGING","/home/user/src/epics/base-3.14.12.3/../modules/log4epicxx/install")

## define IOC name for log config
epicsEnvSet("IOC_NAME", "logtesting")

## initialise log4epicxx
log4epicxx_init()

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/logTest.dbd"
logTest_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords "db/dbExample1.db", "prefix=logtest"
dbLoadRecords "db/dbSubExample.db", "prefix=logtest"
dbLoadRecords "db/logging.db", "logprefix=logtest:logging:"

## Set this to see messages from mySub
#var mySubDebug 1

## Run this to trace the stages of iocInit
#traceIocInit

cd ${TOP}/iocBoot/${IOC}
iocInit

## Start any sequence programs
seq sncExample, "prefix=logtest"
