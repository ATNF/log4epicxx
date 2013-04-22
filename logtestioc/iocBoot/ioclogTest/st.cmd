#!../../bin/linux-x86/logTest

## You may have to change logTest to something else
## everywhere it appears in this file

< envPaths

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
