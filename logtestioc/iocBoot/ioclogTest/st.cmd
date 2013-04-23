#!../../bin/linux-x86/logTest
< envPaths

cd ${TOP}

# log4epicxx

# define IOC name for log config filename
# environment variables can be passed to
# log config file this way
epicsEnvSet("IOC_LOG_FILENAME", "${TOP}/iocBoot/ioclogTest/logtestioc.log")

# set log config file location
# omitting this will try to load ioc.log_cfg from
# current directory
epicsEnvSet("IOC_LOG_CONFIG", "${TOP}/iocBoot/ioclogTest/ioc.log_cfg")

log4epicxx_init()

# log4epicxx

# Register all support components
dbLoadDatabase "dbd/logTest.dbd"
logTest_registerRecordDeviceDriver pdbbase

# Load record instances
dbLoadRecords "db/dbExample1.db", "prefix=logtest"
dbLoadRecords "db/dbSubExample.db", "prefix=logtest"

# log4epicxx records
dbLoadRecords "db/logging.db", "logprefix=logtest:logging:"
# log4epicxx records

cd ${TOP}/iocBoot/${IOC}
iocInit

# Start any sequence programs
seq sncExample, "prefix=logtest"
